// ============================================================================
// IMPLEMENTASI: server/dhcp.c — Layanan Peminjaman IP Otomatis
// ============================================================================
#include "dhcp.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

// Struktur Paket DHCP RFC 2131 (Packed: 236 bytes header + 312 bytes options)
#pragma pack(push, 1)
typedef struct {
    uint8_t  op;           // 1 = BOOTREQUEST, 2 = BOOTREPLY
    uint8_t  htype;        // 1 = Ethernet 10Mb
    uint8_t  hlen;         // 6 = Panjang alamat MAC
    uint8_t  hops;         // 0
    uint32_t xid;          // Transaction ID acak dari client
    uint16_t secs;         // Detik sejak inisiasi
    uint16_t flags;        // Flags (Bit 0 = Broadcast flag)
    uint32_t ciaddr;       // Client IP address saat ini
    uint32_t yiaddr;       // 'Your' (Client) IP address yang ditawarkan server
    uint32_t siaddr;       // Server IP address berikutnya
    uint32_t giaddr;       // Relay agent gateway IP
    uint8_t  chaddr[16];   // Client Hardware Address (MAC)
    uint8_t  sname[64];    // Server host name opsional
    uint8_t  file[128];    // Boot file name opsional
    uint8_t  options[312]; // Opsi DHCP (Magic Cookie + Tag TLV)
} dhcp_pkt_t;
#pragma pack(pop)

static int g_sock = -1;
static volatile int g_running = 0;
static uint32_t g_next_ip = 100; // Mulai meminjamkan dari 192.168.1.100

/*
 * === FUNGSI: handle_packet() ===
 * Tujuan   : Memproses pesan DHCP DISCOVER / REQUEST dan merakit balasan OFFER / ACK
 * Parameter: const uint8_t *data, int len, uint8_t *resp, uint32_t server_ip
 * Return   : Panjang paket balasan dalam byte (>0), atau -1 jika diabaikan
 */
static int handle_packet(const uint8_t *data, int len, uint8_t *resp, uint32_t server_ip) {
    if (len < (int)sizeof(dhcp_pkt_t)) return -1;
    const dhcp_pkt_t *req = (const dhcp_pkt_t*)data;
    dhcp_pkt_t *rep = (dhcp_pkt_t*)resp;

    // Periksa apakah ini BOOTREQUEST (op = 1)
    if (req->op != 1) return -1;

    // Periksa Magic Cookie DHCP (0x63, 0x82, 0x53, 0x63) di awal options
    if (req->options[0] != 0x63 || req->options[1] != 0x82 ||
        req->options[2] != 0x53 || req->options[3] != 0x63) return -1;

    // Cari Option 53 (DHCP Message Type)
    uint8_t msg_type = 0;
    int opt_idx = 4;
    while (opt_idx < 308 && req->options[opt_idx] != 255) {
        if (req->options[opt_idx] == 53) {
            msg_type = req->options[opt_idx + 2];
            break;
        }
        opt_idx += 2 + req->options[opt_idx + 1];
    }

    // Siapkan template dasar BOOTREPLY
    memset(rep, 0, sizeof(dhcp_pkt_t));
    rep->op = 2; // BOOTREPLY
    rep->htype = 1;
    rep->hlen = 6;
    rep->xid = req->xid; // Cocokkan Transaction ID
    memcpy(rep->chaddr, req->chaddr, 16);

    // Tetapkan IP untuk client (192.168.1.X)
    uint32_t assigned_ip = inet_addr("192.168.1.0") | htonl(g_next_ip);
    rep->yiaddr = assigned_ip;
    rep->siaddr = server_ip;

    // Pasang Magic Cookie
    rep->options[0] = 0x63; rep->options[1] = 0x82;
    rep->options[2] = 0x53; rep->options[3] = 0x63;
    uint8_t *p = &rep->options[4];

    if (msg_type == DHCP_DISCOVER) {
        // Balas dengan DHCP OFFER (Type 2)
        *p++ = 53; *p++ = 1; *p++ = DHCP_OFFER;
    } else if (msg_type == DHCP_REQUEST) {
        // Balas dengan DHCP ACK (Type 5)
        *p++ = 53; *p++ = 1; *p++ = DHCP_ACK;
        g_next_ip = (g_next_ip + 1 > 200) ? 100 : g_next_ip + 1; // Putar pool IP 100-200
    } else {
        return -1;
    }

    // Option 1: Subnet Mask (255.255.255.0)
    *p++ = 1; *p++ = 4;
    uint32_t mask = inet_addr("255.255.255.0");
    memcpy(p, &mask, 4); p += 4;

    // Option 3: Default Gateway / Router (192.168.1.1)
    *p++ = 3; *p++ = 4;
    memcpy(p, &server_ip, 4); p += 4;

    // Option 6: DNS Server (Arahkan ke 192.168.1.1 untuk DNS Spoofing!)
    *p++ = 6; *p++ = 4;
    memcpy(p, &server_ip, 4); p += 4;

    // Option 51: Lease Time (7200 detik = 2 jam)
    *p++ = 51; *p++ = 4;
    uint32_t lease = htonl(7200);
    memcpy(p, &lease, 4); p += 4;

    // Option 255: End of Options
    *p++ = 255;

    return sizeof(dhcp_pkt_t);
}

/*
 * === FUNGSI: dhcp_thread() ===
 * Tujuan   : Thread UDP server port 67 pengelola permintaan DHCP
 */
static void *dhcp_thread(void *arg) {
    (void)arg;
    struct sockaddr_in srv, cl;
    socklen_t cl_len = sizeof(cl);
    uint8_t rx[1024], tx[1024];

    g_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sock < 0) { g_running = 0; return NULL; }

    int opt = 1;
    setsockopt(g_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(g_sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_port = htons(DHCP_SERVER_PORT);

    if (bind(g_sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        close(g_sock); g_sock = -1; g_running = 0;
        perror("bind dhcp"); return NULL;
    }

    log_event(LOG_INFO, "DHCP Server aktif di UDP port %d", DHCP_SERVER_PORT);
    uint32_t my_ip = inet_addr("192.168.1.1");

    while (g_running) {
        ssize_t n = recvfrom(g_sock, rx, sizeof(rx), 0, (struct sockaddr*)&cl, &cl_len);
        if (n <= 0) continue;

        int rep_len = handle_packet(rx, (int)n, tx, my_ip);
        if (rep_len > 0) {
            // Kirim kembali ke broadcast port 68 client
            struct sockaddr_in bcast;
            memset(&bcast, 0, sizeof(bcast));
            bcast.sin_family = AF_INET;
            bcast.sin_addr.s_addr = INADDR_BROADCAST;
            bcast.sin_port = htons(DHCP_CLIENT_PORT);
            sendto(g_sock, tx, rep_len, 0, (struct sockaddr*)&bcast, sizeof(bcast));
        }
    }
    return NULL;
}

/*
 * === FUNGSI: dhcp_start() ===
 * Tujuan   : Meluncurkan layanan server DHCP di latar belakang
 */
int dhcp_start(ap_config_t *cfg) {
    if (g_running) return 0;
    g_running = 1;
    pthread_t tid;
    if (pthread_create(&tid, NULL, dhcp_thread, cfg) != 0) {
        g_running = 0;
        log_event(LOG_ERROR, "Gagal membuat thread DHCP");
        return -1;
    }
    pthread_detach(tid);
    return 0;
}

/*
 * === FUNGSI: dhcp_stop() ===
 * Tujuan   : Menghentikan server DHCP
 */
int dhcp_stop(void) {
    g_running = 0;
    if (g_sock >= 0) { close(g_sock); g_sock = -1; }
    return 0;
}

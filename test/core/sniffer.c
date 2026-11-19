// ============================================================================
// IMPLEMENTASI: core/sniffer.c — Mesin Penyalur Frame (Packet Dispatcher)
// ============================================================================
#include "sniffer.h"
#include "../core/rogue_ap.h"
#include "../attack/karma.h"
#include "../capture/handshake.h"
#include "../capture/client_db.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>

static volatile bool g_sniffer_running = false;
static pthread_t g_sniffer_tid;
static int g_sniffer_sock = -1;

/*
 * === FUNGSI: sniffer_is_running() ===
 * Tujuan: Mengembalikan status apakah thread sniffer sedang aktif
 */
bool sniffer_is_running(void) {
    return g_sniffer_running;
}

/*
 * === FUNGSI: dispatch_frame() ===
 * Tujuan: Menganalisis frame 802.11 dan menyalurkannya ke modul yang tepat
 */
static void dispatch_frame(ap_config_t *cfg, uint8_t *buf, int len) {
    if (len < 24) return; // Abaikan paket di bawah batas minimal header MAC

    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint16_t fc = ntohs(hdr->frame_control);
    uint8_t type = (fc >> 2) & 0x03;
    uint8_t subtype = (fc >> 4) & 0x0F;

    // 1. Catat keberadaan client pengirim ke Client Database
    if (type == IEEE80211_FTYPE_MGMT || type == IEEE80211_FTYPE_DATA) {
        client_db_add_or_update(hdr->addr2, NULL, false);
    }

    // 2. Filter Berdasarkan Subtype
    if (type == IEEE80211_FTYPE_MGMT) {
        switch (subtype) {
            case IEEE80211_STYPE_PROBE_REQ:
                // Jika fitur Karma aktif, tanggapi probe request ini
                if (cfg->karma_enabled) {
                    karma_handle_probe_request(cfg, buf, len);
                }
                break;

            case IEEE80211_STYPE_AUTH:
                // Balas permintaan autentikasi jika ditujukan ke BSSID AP kita
                if (memcmp(hdr->addr1, cfg->bssid, 6) == 0) {
                    send_auth_response(cfg, hdr->addr2);
                }
                break;

            case IEEE80211_STYPE_ASSOC_REQ:
                // Balas permintaan asosiasi jika ditujukan ke BSSID AP kita
                if (memcmp(hdr->addr1, cfg->bssid, 6) == 0) {
                    send_assoc_response(cfg, hdr->addr2);
                    client_db_add_or_update(hdr->addr2, NULL, true); // Tandai status tersambung
                }
                break;

            default:
                break;
        }
    } else if (type == IEEE80211_FTYPE_DATA) {
        // 3. Tangkap 4-Way Handshake EAPOL jika fitur capture aktif
        if (cfg->handshake_capture_enabled) {
            handshake_process_packet(buf, len, cfg->bssid);
        }
    }
}

/*
 * === FUNGSI: sniffer_worker() ===
 * Tujuan: Thread loop penangkap frame dari kartu Wi-Fi monitor
 */
static void *sniffer_worker(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    struct sockaddr_ll sll;
    
    g_sniffer_sock = open_raw_socket(cfg->monitor_iface, &sll);
    if (g_sniffer_sock < 0) {
        log_event(LOG_ERROR, "Sniffer gagal membuka raw socket pada '%s'", cfg->monitor_iface);
        g_sniffer_running = false;
        return NULL;
    }

    log_event(LOG_INFO, "Sniffer aktif mendengarkan di interface '%s'", cfg->monitor_iface);

    uint8_t rx_buf[2048];
    struct pollfd pfd = {
        .fd = g_sniffer_sock,
        .events = POLLIN
    };

    while (g_sniffer_running) {
        int pr = poll(&pfd, 1, 500); // Polling dengan timeout 500 ms agar loop responsif
        if (pr > 0 && (pfd.revents & POLLIN)) {
            ssize_t n = recvfrom(g_sniffer_sock, rx_buf, sizeof(rx_buf), 0, NULL, NULL);
            if (n > 0) {
                dispatch_frame(cfg, rx_buf, (int)n);
            }
        }
    }

    close(g_sniffer_sock);
    g_sniffer_sock = -1;
    log_event(LOG_INFO, "Sniffer thread berhenti");
    return NULL;
}

/*
 * === FUNGSI: sniffer_start() ===
 * Tujuan: Memulai thread sniffer sentral
 */
int sniffer_start(ap_config_t *cfg) {
    if (g_sniffer_running) return 0;
    g_sniffer_running = true;
    if (pthread_create(&g_sniffer_tid, NULL, sniffer_worker, cfg) != 0) {
        log_event(LOG_ERROR, "Gagal membuat pthread sniffer");
        g_sniffer_running = false;
        return -1;
    }
    return 0;
}

/*
 * === FUNGSI: sniffer_stop() ===
 * Tujuan: Menghentikan thread sniffer dan menunggu pembersihan socket
 */
void sniffer_stop(void) {
    if (!g_sniffer_running) return;
    g_sniffer_running = false;
    pthread_join(g_sniffer_tid, NULL);
}

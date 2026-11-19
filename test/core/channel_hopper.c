// ============================================================================
// IMPLEMENTASI: core/channel_hopper.c — Mesin Pemindah Kanal Nirkabel
// ============================================================================
#include "channel_hopper.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <unistd.h>
#include <pthread.h>

static volatile int g_running = 0;
static pthread_t g_tid;

/*
 * === FUNGSI: hopper_thread() ===
 * Tujuan   : Thread loop yang memutar frekuensi radio kartu monitor
 */
static void *hopper_thread(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    // Daftar kanal 2.4 GHz non-overlapping dan kanal 5 GHz populer
    const int ch[] = {1, 6, 11, 36, 40, 44, 48, 149, 153, 157, 161};
    int n = sizeof(ch) / sizeof(ch[0]);
    int i = 0;
    log_event(LOG_INFO, "Channel Hopper aktif pada interface '%s'", cfg->monitor_iface);

    while (g_running) {
        set_interface_channel(cfg->monitor_iface, ch[i]);
        i = (i + 1) % n; // Pindah ke kanal berikutnya
        usleep(cfg->channel_hop_interval_ms * 1000); // Istirahat sesuai interval (default 500ms)
    }

    log_event(LOG_INFO, "Channel Hopper berhenti");
    return NULL;
}

/*
 * === FUNGSI: channel_hopper_start() ===
 * Tujuan   : Meluncurkan thread rotasi kanal radio
 * Catatan  : Mematuhi Fix #4
 */
int channel_hopper_start(ap_config_t *cfg) {
    if (g_running) return 0;
    g_running = 1;
    // FIX #4: Periksa nilai return pthread_create
    if (pthread_create(&g_tid, NULL, hopper_thread, cfg) != 0) {
        g_running = 0;
        log_event(LOG_ERROR, "Gagal membuat thread channel hopper");
        return -1;
    }
    return 0;
}

/*
 * === FUNGSI: channel_hopper_stop() ===
 * Tujuan   : Menghentikan rotasi kanal dan menunggu thread selesai
 */
int channel_hopper_stop(void) {
    if (!g_running) return 0;
    g_running = 0;
    pthread_join(g_tid, NULL);
    return 0;
}

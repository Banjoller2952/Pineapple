// ============================================================================
// IMPLEMENTASI: capture/client_db.c — Manajemen Tabel Client di RAM
// ============================================================================
#include "client_db.h"
#include <string.h>
#include <pthread.h>

static client_t g_clients[MAX_CLIENTS];
static int g_num_clients = 0;
static pthread_mutex_t g_client_mutex = PTHREAD_MUTEX_INITIALIZER;

int client_db_init(void) {
    pthread_mutex_lock(&g_client_mutex);
    memset(g_clients, 0, sizeof(g_clients));
    g_num_clients = 0;
    pthread_mutex_unlock(&g_client_mutex);
    return 0;
}

void client_db_cleanup(void) {
    pthread_mutex_lock(&g_client_mutex);
    g_num_clients = 0;
    pthread_mutex_unlock(&g_client_mutex);
}

int client_db_add_or_update(const uint8_t *mac, const char *ip, bool connected) {
    pthread_mutex_lock(&g_client_mutex);
    time_t now = time(NULL);

    // Cari apakah MAC client sudah ada di database
    for (int i = 0; i < g_num_clients; i++) {
        if (memcmp(g_clients[i].mac, mac, 6) == 0) {
            g_clients[i].last_seen = now;
            g_clients[i].is_connected = connected;
            if (ip && strlen(ip) > 0) {
                strncpy(g_clients[i].ip, ip, sizeof(g_clients[i].ip) - 1);
            }
            pthread_mutex_unlock(&g_client_mutex);
            return i;
        }
    }

    // Jika belum ada dan kapasitas masih tersedia, tambahkan client baru
    if (g_num_clients < MAX_CLIENTS) {
        int idx = g_num_clients++;
        memcpy(g_clients[idx].mac, mac, 6);
        if (ip) strncpy(g_clients[idx].ip, ip, sizeof(g_clients[idx].ip) - 1);
        g_clients[idx].first_seen = now;
        g_clients[idx].last_seen = now;
        g_clients[idx].is_connected = connected;
        pthread_mutex_unlock(&g_client_mutex);
        return idx;
    }

    pthread_mutex_unlock(&g_client_mutex);
    return -1; // Database penuh
}

client_t *client_db_get(const uint8_t *mac) {
    pthread_mutex_lock(&g_client_mutex);
    for (int i = 0; i < g_num_clients; i++) {
        if (memcmp(g_clients[i].mac, mac, 6) == 0) {
            pthread_mutex_unlock(&g_client_mutex);
            return &g_clients[i];
        }
    }
    pthread_mutex_unlock(&g_client_mutex);
    return NULL;
}

int client_db_list(client_t *out_list, int max_clients) {
    pthread_mutex_lock(&g_client_mutex);
    int count = g_num_clients < max_clients ? g_num_clients : max_clients;
    memcpy(out_list, g_clients, count * sizeof(client_t));
    pthread_mutex_unlock(&g_client_mutex);
    return count;
}

int client_db_count(void) {
    pthread_mutex_lock(&g_client_mutex);
    int c = g_num_clients;
    pthread_mutex_unlock(&g_client_mutex);
    return c;
}

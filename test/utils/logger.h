// ============================================================================
// HEADER: utils/logger.h — Antarmuka Pencatatan Peristiwa Thread-Safe
// ============================================================================
#ifndef LOGGER_H
#define LOGGER_H

#include "../config.h" // Butuh tipe credential_t, handshake_t, session_cookie_t

// Level urgensi pencatatan log
typedef enum { 
    LOG_DEBUG, // Informasi detail diagnosa internal
    LOG_INFO,  // Informasi operasional normal
    LOG_WARN,  // Peringatan kondisi tidak biasa
    LOG_ERROR, // Kesalahan operasi / syscall gagal
    LOG_ALERT  // Notifikasi kritis keamanan (Handshake/Kredensial tertangkap)
} log_level_t;

// API Publik Logger
int logger_init(const char *log_file);
void logger_close(void);
void log_event(log_level_t level, const char *format, ...);
void log_credential(const credential_t *cred);
void log_handshake(const handshake_t *hs);
void log_cookie(const session_cookie_t *cookie);

#endif // LOGGER_H

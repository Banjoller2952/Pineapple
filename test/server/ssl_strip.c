// ============================================================================
// IMPLEMENTASI: server/ssl_strip.c — Pengganti String https:// Menjadi http://
// ============================================================================
#include "ssl_strip.h"
#include <string.h>

int ssl_strip_init(void) { return 0; }
void ssl_strip_cleanup(void) {}

/*
 * === FUNGSI: ssl_strip_process() ===
 * Tujuan   : Menghapus huruf 's' dari seluruh tautan "https://" di respon halaman web
 */
int ssl_strip_process(const uint8_t *in, int in_len, uint8_t *out, int *out_len) {
    int i = 0, j = 0;
    while (i < in_len && j < 32000) {
        if (i + 8 <= in_len && memcmp(&in[i], "https://", 8) == 0) {
            memcpy(&out[j], "http://", 7); // Ganti dengan http:// biasa
            i += 8;
            j += 7;
        } else {
            out[j++] = in[i++];
        }
    }
    *out_len = j;
    return 0;
}

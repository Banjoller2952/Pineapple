// ============================================================================
// HEADER: server/ssl_strip.h — Mesin Downgrade Enkripsi HTTPS ke HTTP
// ============================================================================
#ifndef SSL_STRIP_H
#define SSL_STRIP_H

#include "../config.h"

int ssl_strip_init(void);
void ssl_strip_cleanup(void);
int ssl_strip_process(const uint8_t *in, int in_len, uint8_t *out, int *out_len);

#endif // SSL_STRIP_H

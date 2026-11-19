// ============================================================================
// HEADER: attack/karma.h — Mesin Penyerapan Probe Request Otomatis (Karma)
// ============================================================================
#ifndef KARMA_H
#define KARMA_H

#include "../config.h"

int karma_init(void);
void karma_cleanup(void);
void karma_add_ssid(const char *ssid);
bool karma_is_targeted(const char *ssid);
int karma_handle_probe_request(ap_config_t *cfg, uint8_t *frame, int len);

#endif // KARMA_H

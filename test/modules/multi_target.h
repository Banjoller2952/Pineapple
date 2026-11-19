// ============================================================================
// HEADER: modules/multi_target.h — Antrian Serangan Multi-Sasaran Massal
// ============================================================================
#ifndef MULTI_TARGET_H
#define MULTI_TARGET_H

#include "../config.h"

int multi_target_add(const uint8_t *mac);
int multi_target_clear(void);
int multi_target_attack_all(ap_config_t *cfg);
void multi_target_print(void);

#endif // MULTI_TARGET_H

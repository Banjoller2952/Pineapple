// ============================================================================
// HEADER: attack/wps_attack.h — Mesin Audit Kelemahan PIN Wi-Fi Protected Setup
// ============================================================================
#ifndef WPS_ATTACK_H
#define WPS_ATTACK_H

#include "../config.h"

int wps_attack_start(const char *iface, const uint8_t *bssid, int channel);

#endif // WPS_ATTACK_H

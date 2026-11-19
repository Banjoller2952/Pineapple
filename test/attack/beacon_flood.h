// ============================================================================
// HEADER: attack/beacon_flood.h — Pembanjir Ribuan SSID Palsu di Udara
// ============================================================================
#ifndef BEACON_FLOOD_H
#define BEACON_FLOOD_H

#include "../config.h"

int beacon_flood_start(const char *iface, int channel, int count);
void beacon_flood_stop(void);

#endif // BEACON_FLOOD_H

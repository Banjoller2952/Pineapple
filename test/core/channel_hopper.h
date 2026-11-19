// ============================================================================
// HEADER: core/channel_hopper.h — Rotasi Kanal Radio Frekuensi Otomatis
// ============================================================================
#ifndef CHANNEL_HOPPER_H
#define CHANNEL_HOPPER_H

#include "../config.h"

int channel_hopper_start(ap_config_t *cfg);
int channel_hopper_stop(void);

#endif // CHANNEL_HOPPER_H

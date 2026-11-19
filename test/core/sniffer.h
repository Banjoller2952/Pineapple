// ============================================================================
// HEADER: core/sniffer.h — Dispatcher Sentral Penyaring Frame Radio
// ============================================================================
#ifndef SNIFFER_H
#define SNIFFER_H

#include "../config.h"

// API Kontrol Sniffer
int sniffer_start(ap_config_t *cfg);
void sniffer_stop(void);
bool sniffer_is_running(void); // FIX #15: Fungsi accessor status berjalan

#endif // SNIFFER_H

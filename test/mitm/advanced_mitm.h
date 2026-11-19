// ============================================================================
// HEADER: mitm/advanced_mitm.h — Pengendali Rantai Serangan MITM Lanjutan
// ============================================================================
#ifndef ADVANCED_MITM_H
#define ADVANCED_MITM_H

#include "../config.h"

int advanced_mitm_init(ap_config_t *cfg);
void advanced_mitm_cleanup(void);

#endif // ADVANCED_MITM_H

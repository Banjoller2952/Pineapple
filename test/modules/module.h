// ============================================================================
// HEADER: modules/module.h — Arsitektur Plugin Dinamis (Shared Libraries .so)
// ============================================================================
#ifndef MODULE_H
#define MODULE_H

#include "../config.h"

int module_init(void);
int module_load(const char *path);
void module_list(void);
void module_cleanup(void); // FIX #21: Deklarasi resmi pembersih plugin

#endif // MODULE_H

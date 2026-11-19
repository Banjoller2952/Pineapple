// ============================================================================
// HEADER: filter/filter.h — Firewall Aturan Penyaringan Konten MITM
// ============================================================================
#ifndef FILTER_H
#define FILTER_H

#include "../config.h"

int filter_init(void);
void filter_cleanup(void);
int filter_add_rule(const char *pattern, int type, bool block);
int filter_match(const char *host);

#endif // FILTER_H

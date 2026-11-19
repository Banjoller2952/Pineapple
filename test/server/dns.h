// ============================================================================
// HEADER: server/dns.h — Server DNS Spoofer Pembelok Domain
// ============================================================================
#ifndef DNS_H
#define DNS_H

#include "../config.h"

// API Kontrol DNS Spoofer
int dns_start(ap_config_t *cfg);
int dns_stop(void);
int dns_add_spoof(const char *domain, const char *ip);
void dns_clear_spoofs(void);

#endif // DNS_H

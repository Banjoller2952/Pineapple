// ============================================================================
// HEADER: mitm/proxy.h — Server Proxy Transparan HTTP Interceptor
// ============================================================================
#ifndef PROXY_H
#define PROXY_H

#include "../config.h"

int proxy_start(ap_config_t *cfg);
int proxy_stop(void);

#endif // PROXY_H

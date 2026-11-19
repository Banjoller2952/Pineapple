// ============================================================================
// HEADER: server/evil_portal.h — Web Server Captive Portal Phishing
// ============================================================================
#ifndef EVIL_PORTAL_H
#define EVIL_PORTAL_H

#include "../config.h"

int evil_portal_start(ap_config_t *cfg);
int evil_portal_stop(void);
int evil_portal_set_template(const char *html);

#endif // EVIL_PORTAL_H

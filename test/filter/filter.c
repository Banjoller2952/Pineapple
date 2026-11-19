// ============================================================================
// IMPLEMENTASI: filter/filter.c — Pemeriksa Daftar Hitam/Putih Domain
// ============================================================================
#include "filter.h"
#include <string.h>

static filter_rule_t g_rules[MAX_FILTER_RULES];
static int g_rule_count = 0;

int filter_init(void) { g_rule_count = 0; return 0; }
void filter_cleanup(void) { g_rule_count = 0; }

int filter_add_rule(const char *pattern, int type, bool block) {
    if (g_rule_count >= MAX_FILTER_RULES) return -1;
    strncpy(g_rules[g_rule_count].pattern, pattern, sizeof(g_rules[g_rule_count].pattern) - 1);
    g_rules[g_rule_count].type = type;
    g_rules[g_rule_count].block = block;
    g_rule_count++;
    return 0;
}

int filter_match(const char *host) {
    for (int i = 0; i < g_rule_count; i++) {
        if (strstr(host, g_rules[i].pattern) != NULL) {
            return g_rules[i].block ? 1 : 0; // 1 = Blokir, 0 = Izinkan
        }
    }
    return 0; // Default izinkan jika tidak ada kecocokan
}

// ============================================================================
// IMPLEMENTASI: modules/module.c — Loader Modul Dinamis POSIX dlopen
// ============================================================================
#include "module.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h> // Header sistem Linux untuk dlopen, dlclose, dlerror

typedef struct { 
    char name[64]; 
    void *handle; 
} entry_t;

static entry_t g_mods[MAX_MODULES];
static int g_count = 0;

int module_init(void) { 
    memset(g_mods, 0, sizeof(g_mods)); 
    g_count = 0; 
    return 0; 
}

/*
 * === FUNGSI: module_load() ===
 * Tujuan   : Memuat file plugin .so ke ruang memori proses
 */
int module_load(const char *path) {
    if (g_count >= MAX_MODULES) return -1;
    void *h = dlopen(path, RTLD_NOW);
    if (!h) {
        log_event(LOG_ERROR, "Gagal memuat plugin %s: %s", path, dlerror());
        return -1;
    }
    entry_t *m = &g_mods[g_count++];
    const char *n = strrchr(path, '/');
    strncpy(m->name, n ? n + 1 : path, 63);
    m->handle = h;
    log_event(LOG_INFO, "Plugin dinamis berhasil dimuat: %s", m->name);
    return 0;
}

void module_list(void) {
    printf("\n=== DAFTAR PLUGIN AKTIF (%d) ===\n", g_count);
    for (int i = 0; i < g_count; i++) {
        printf("[%d] %s\n", i + 1, g_mods[i].name);
    }
    printf("================================\n\n");
}

/*
 * === FUNGSI: module_cleanup() ===
 * Tujuan   : Menutup seluruh plugin yang dimuat untuk mencegah memory leak
 * Catatan  : Mematuhi Fix #21
 */
void module_cleanup(void) {
    for (int i = 0; i < g_count; i++) {
        if (g_mods[i].handle) {
            dlclose(g_mods[i].handle);
            g_mods[i].handle = NULL;
        }
    }
    g_count = 0;
}

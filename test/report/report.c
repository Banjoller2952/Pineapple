// ============================================================================
// IMPLEMENTASI: report/report.c — Penyusun Berkas Ringkasan Temuan Audit
// ============================================================================
#include "report.h"
#include "../utils/utils.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <time.h>

static void copy_file(FILE *out, const char *fname, const char *label) {
    FILE *in = fopen(fname, "r");
    fprintf(out, "\n--- %s ---\n", label);
    if (!in) { 
        fprintf(out, "(Berkas %s belum ada / belum ada data)\n", fname); 
        return; 
    }
    char line[512]; 
    int n = 0;
    while (fgets(line, sizeof(line), in)) { 
        fprintf(out, "%s", line); 
        n++; 
    }
    fclose(in);
    if (n == 0) fprintf(out, "(Data kosong)\n");
    else fprintf(out, "(Total: %d baris temuan)\n", n);
}

/*
 * === FUNGSI: report_generate() ===
 * Tujuan   : Menggabungkan seluruh data bukti perkara menjadi satu laporan resmi
 */
int report_generate(const char *path) {
    FILE *fp = fopen(path, "w");
    if (!fp) return -1;

    time_t now = time(NULL);
    char ts[64];
    format_timestamp(now, ts, sizeof(ts));

    fprintf(fp, "=======================================================\n");
    fprintf(fp, "        LAPORAN AUDIT KEAMANAN NIRKABEL WIFI PINEAPPLE \n");
    fprintf(fp, "=======================================================\n");
    fprintf(fp, "Waktu Pembuatan : %s\n", ts);
    fprintf(fp, "Versi Engine    : v15.4 (Enterprise Pure C System)\n");

    copy_file(fp, "credentials.txt", "KREDENSIAL FORMULIR TERTANGKAP");
    copy_file(fp, "cookies.txt", "SESSION COOKIE HTTP TERTANGKAP");
    copy_file(fp, "clients.txt", "DAFTAR KLIEN TERDETEKSI");

    fprintf(fp, "\n--- BUKTI HANDSHAKE WPA ---\n");
    FILE *hs = fopen("handshake.cap", "rb");
    if (hs) {
        fseek(hs, 0, SEEK_END);
        fprintf(fp, "handshake.cap: %ld bytes (Format PCAP Wireshark)\n", ftell(hs));
        fclose(hs);
    } else {
        fprintf(fp, "(Belum ada handshake yang tertangkap)\n");
    }

    fprintf(fp, "\n--- REKOMENDASI PENGAMANAN INFRASTRUKTUR ---\n");
    fprintf(fp, "1. Migrasi seluruh Access Point ke WPA3-SAE dengan enkripsi GCMP.\n");
    fprintf(fp, "2. Wajib aktifkan IEEE 802.11w / PMF (Protected Management Frames) untuk menangkal Deauth.\n");
    fprintf(fp, "3. Matikan fitur Wi-Fi Protected Setup (WPS) secara permanen di router.\n");
    fprintf(fp, "4. Pasang Wireless Intrusion Prevention System (WIPS) untuk mendeteksi Rogue AP.\n");
    fprintf(fp, "\n==================== AKHIR LAPORAN ====================\n");
    
    fclose(fp);
    log_event(LOG_INFO, "Laporan audit keamanan berhasil digenerate ke '%s'", path);
    return 0;
}

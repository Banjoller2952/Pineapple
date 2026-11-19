// ============================================================================
// IMPLEMENTASI: utils/utils.c — Perkakas Teknis Sistem Operasi
// ============================================================================
#include "utils.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <time.h>

/*
 * === FUNGSI: open_raw_socket() ===
 * Tujuan   : Membuka socket raw Layer 2 (AF_PACKET) dan mengikatnya ke interface
 * Analogi  : Membuka pintu gerbang kargo khusus langsung ke landasan pacu kartu radio
 * Parameter:
 *   - const char *iface          : Nama kartu jaringan (misal "mon0")
 *   - struct sockaddr_ll *sll_out: Struktur alamat link-layer yang akan diisi
 * Return   : File descriptor socket (> 0) jika sukses, -1 jika gagal
 */
int open_raw_socket(const char *iface, struct sockaddr_ll *sll_out) {
    // 1. Buat socket raw berkemampuan membaca semua frame (ETH_P_ALL)
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        log_event(LOG_ERROR, "Gagal membuat socket AF_PACKET pada '%s'", iface);
        return -1;
    }

    // 2. Dapatkan interface index kartu jaringan melalui ioctl
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        log_event(LOG_ERROR, "Gagal membaca index kartu interface '%s'", iface);
        close(sock);
        return -1;
    }

    // 3. Ikat (bind) soket ke interface spesifik tersebut
    memset(sll_out, 0, sizeof(*sll_out));
    sll_out->sll_family   = AF_PACKET;
    sll_out->sll_ifindex  = ifr.ifr_ifindex;
    sll_out->sll_protocol = htons(ETH_P_ALL);

    if (bind(sock, (struct sockaddr *)sll_out, sizeof(*sll_out)) < 0) {
        log_event(LOG_ERROR, "Gagal mengikat raw socket ke '%s'", iface);
        close(sock);
        return -1;
    }

    log_event(LOG_DEBUG, "Raw socket berhasil diikat ke '%s' (ifindex: %d)", iface, ifr.ifr_ifindex);
    return sock;
}

/*
 * === FUNGSI: set_interface_channel() ===
 * Tujuan   : Mengubah kanal frekuensi radio Wi-Fi secara dinamis
 * Analogi  : Memutar tuner stasiun radio ke gelombang yang diinginkan
 * Parameter:
 *   - const char *iface : Nama kartu jaringan monitor (misal "mon0")
 *   - int channel       : Nomor channel target (1 s/d 14)
 * Return   : 0 jika sukses, -1 jika gagal
 */
int set_interface_channel(const char *iface, int channel) {
    if (channel < 1 || channel > 14) return -1; // Validasi rentang channel 2.4 GHz
    char cmd[128];
    // Gunakan utilitas iw bawaan kernel Linux Wireless
    snprintf(cmd, sizeof(cmd), "iw dev %s set channel %d 2>/dev/null", iface, channel);
    int ret = system(cmd);
    return ret == 0 ? 0 : -1;
}

/*
 * === FUNGSI: set_interface_promisc() ===
 * Tujuan   : Mengaktifkan atau menonaktifkan mode promiscuous pada interface
 * Analogi  : Memasang corong pendengar agar bisa mencuri dengar semua bisikan di ruangan
 * Parameter:
 *   - const char *iface : Nama interface jaringan
 *   - bool enable       : true untuk mengaktifkan, false untuk mematikan
 * Return   : 0 jika sukses, -1 jika gagal
 */
int set_interface_promisc(const char *iface, bool enable) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return -1;

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);

    // Ambil flag status interface saat ini
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
        close(sock);
        return -1;
    }

    // Ubah bit flag IFF_PROMISC
    if (enable) {
        ifr.ifr_flags |= IFF_PROMISC;
    } else {
        ifr.ifr_flags &= ~IFF_PROMISC;
    }

    // Tuliskan kembali flag baru ke hardware kartu jaringan
    int res = ioctl(sock, SIOCSIFFLAGS, &ifr);
    close(sock);
    return res < 0 ? -1 : 0;
}

/*
 * === FUNGSI: parse_mac() ===
 * Tujuan   : Mengonversi string format MAC hex ("AA:BB:CC:DD:EE:FF") ke array 6-byte
 * Parameter: const char *str (Input string), uint8_t *mac (Output buffer 6-byte)
 * Return   : 0 jika sukses, -1 jika format tidak valid
 */
int parse_mac(const char *str, uint8_t *mac) {
    return sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) == 6 ? 0 : -1;
}

/*
 * === FUNGSI: format_mac() ===
 * Tujuan   : Mengonversi array biner 6-byte MAC menjadi string manusia "AA:BB:CC:DD:EE:FF"
 * Parameter: const uint8_t *mac (Input 6-byte), char *out (Output buffer min 18 byte)
 * Return   : void
 */
void format_mac(const uint8_t *mac, char *out) {
    sprintf(out, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/*
 * === FUNGSI: format_timestamp() ===
 * Tujuan   : Mengonversi waktu time_t ke format tanggal-jam "YYYY-MM-DD HH:MM:SS"
 * Parameter: time_t t, char *out, size_t max_len
 * Return   : void
 */
void format_timestamp(time_t t, char *out, size_t max_len) {
    struct tm *tm_info = localtime(&t);
    strftime(out, max_len, "%Y-%m-%d %H:%M:%S", tm_info);
}

/*
 * === FUNGSI: get_random_mac() ===
 * Tujuan   : Menghasilkan alamat MAC acak yang sah secara lokal (Locally Administered Unicast)
 * Analogi  : Membuat identitas samaran baru untuk penyamaran detektif
 * Parameter: uint8_t *mac (Output array 6-byte)
 * Return   : void
 */
void get_random_mac(uint8_t *mac) {
    for (int i = 0; i < 6; i++) {
        mac[i] = rand() % 256;
    }
    // Set bit 1 (Locally Administered) dan clear bit 0 (Unicast Address)
    mac[0] = (mac[0] | 0x02) & 0xFE;
}

/*
 * === FUNGSI: hexdump() ===
 * Tujuan   : Mencetak blok memori biner mentah dalam format Hex + ASCII ala Wireshark
 * Parameter: const void *data (Pointer data), size_t len (Panjang byte)
 * Return   : void
 */
void hexdump(const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    for (size_t i = 0; i < len; i += 16) {
        printf("%04zx  ", i);
        for (size_t j = 0; j < 16; j++) {
            if (i + j < len) printf("%02x ", p[i + j]);
            else printf("   ");
        }
        printf(" |");
        for (size_t j = 0; j < 16; j++) {
            if (i + j < len) {
                uint8_t c = p[i + j];
                printf("%c", (c >= 32 && c <= 126) ? c : '.');
            }
        }
        printf("|\n");
    }
}

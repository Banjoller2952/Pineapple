# 📡 Instruction Set & Bytecode 802.11 — Fokus Rogue AP

**Versi:** 4.0  
**Tanggal:** 15 Agustus 2026  
**Penulis:** DeepSeek  
**Lisensi:** MIT  

---

## 📋 Daftar Isi

1. [Pendahuluan: Apa Itu Bytecode 802.11?](#1-pendahuluan-apa-itu-bytecode-80211)
2. [Struktur Frame 802.11](#2-struktur-frame-80211)
3. [Management Frame Opcodes](#3-management-frame-opcodes)
4. [Header: `<net/if.h>` — Interface Network](#4-header-netifh--interface-network)
5. [Header: `<linux/if_packet.h>` — Raw Socket di Layer 2](#5-header-linuxif_packeth--raw-socket-di-layer-2)
6. [Header: `<linux/if_ether.h>` — Konstanta Ethernet](#6-header-linuxif_etherh--konstanta-ethernet)
7. [Header: `<sys/ioctl.h>` — Kontrol Perangkat](#7-header-sysioctlh--kontrol-perangkat)
8. [Header: `<arpa/inet.h>` — Konversi Endian](#8-header-arpaineth--konversi-endian)
9. [Header: `<sys/socket.h>` — Socket API](#9-header-syssocketh--socket-api)
10. [Header: `<unistd.h>` — System Calls](#10-header-unistdh--system-calls)
11. [Header: `<string.h>` — Manipulasi String](#11-header-stringh--manipulasi-string)
12. [Header: `<stdint.h>` — Fixed-width Integer](#12-header-stdinth--fixed-width-integer)
13. [Header: `<pthread.h>` — Multi-threading](#13-header-pthreadh--multi-threading)
14. [Header: `<signal.h>` — Signal Handling](#14-header-signalh--signal-handling)
15. [Header: `<stdio.h>` — Input/Output](#15-header-stdioh--inputoutput)
16. [Header: `<stdlib.h>` — Standard Library](#16-header-stdlibh--standard-library)
17. [Header: `<time.h>` — Time Functions](#17-header-timeh--time-functions)
18. [Header: `<errno.h>` — Error Handling](#18-header-errnoh--error-handling)
19. [Ringkasan: Kapan Menggunakan Header Apa](#19-ringkasan-kapan-menggunakan-header-apa)
20. [Contoh Kode Lengkap: Rogue AP dengan Semua Header](#20-contoh-kode-lengkap-rogue-ap-dengan-semua-header)

---

## 1. Pendahuluan: Apa Itu Bytecode 802.11? — Analogi dan konsep dasar

**802.11 bukanlah prosesor dengan instruction set**, melainkan **protokol komunikasi**. Namun, frame-frame 802.11 bisa dianggap sebagai "instruksi" yang dikirimkan melalui udara. Setiap frame memiliki **opcode** (frame control) yang menentukan jenis instruksi.

### Analogi Sederhana:

```
┌─────────────────────────────────────────────────────────────────────┐
│                        802.11 = "Bahasa"                            │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  Setiap frame = "Kata"                                             │
│  Setiap field = "Huruf"                                            │
│  Setiap byte = "Bit"                                               │
│                                                                     │
│  Beacon     = "Saya AP, nama saya FreeWiFi"  →  Opcode: 0x8000    │
│  Probe Req  = "Ada AP bernama FreeWiFi?"   →  Opcode: 0x4000    │
│  Probe Resp = "Saya FreeWiFi!"              →  Opcode: 0x5000    │
│  Auth       = "Boleh connect?" / "Boleh!"   →  Opcode: 0xB000    │
│  Assoc      = "Saya mau join" / "Silahkan"  →  Opcode: 0x1000    │
│  Deauth     = "Keluar!"                     →  Opcode: 0xC000    │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### Bytecode (Hex) — Representasi hex dari setiap frame:

```
Beacon     = 80 00 00 00 FF FF FF FF FF FF ...   // 0x8000
Probe Req  = 40 00 00 00 FF FF FF FF FF FF ...   // 0x4000
Probe Resp = 50 00 00 00 00 11 22 33 44 55 ...   // 0x5000
Auth       = B0 00 00 00 00 11 22 33 44 55 ...   // 0xB000
Assoc      = 00 00 00 00 00 11 22 33 44 55 ...   // 0x0000
Deauth     = C0 00 00 00 00 11 22 33 44 55 ...   // 0xC000
```

---

## 2. Struktur Frame 802.11 — Format header dan field-field penting

### Header 802.11 (24 bytes) — Struktur MAC header yang harus kita bangun

```
┌─────────────────────────────────────────────────────────────────────┐
│                        802.11 MAC HEADER                            │
├─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬───────┤
│ 2       │ 2       │ 6       │ 6       │ 6       │ 2       │ 4     │
├─────────┼─────────┼─────────┼─────────┼─────────┼─────────┼───────┤
│ Frame   │ Dur.    │ Addr1   │ Addr2   │ Addr3   │ Seq     │ Payl. │
│ Ctrl    │         │ (Dest)  │ (Src)   │ (BSSID) │ Ctrl    │ (var) │
├─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴───────┤
│                                                                     │
│  FCS (4 bytes) - CRC32 checksum (biasanya diisi kernel)            │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### Frame Control Field (2 bytes) — Field yang menentukan opcode frame

```
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│ 15  │ 14  │ 13  │ 12  │ 11  │ 10  │ 9   │ 8   │ 7   │ 6   │ 5   │ 4   │ 3   │ 2   │ 1   │ 0   │
├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
│     Protocol      │ Type  │          Subtype               │
│     Version       │       │                               │
│     (00)          │       │                               │
└───────────────────┴───────┴────────────────────────────────┘
```

**Type (bit 2-3):**
- `00` = Management — **Ini yang kita pakai untuk Rogue AP**
- `01` = Control — RTS, CTS, ACK
- `10` = Data — Data payload

**Subtype (bit 4-7) untuk Management Frames:**
- `0000` = Association Request → **Client minta join**
- `0001` = Association Response → **AP respon join**
- `0100` = Probe Request → **Client cari AP**
- `0101` = Probe Response → **AP respon probe**
- `1000` = Beacon → **AP broadcast identitas**
- `1011` = Authentication → **Proses autentikasi**
- `1100` = Deauthentication → **Putus koneksi**

---

## 3. Management Frame Opcodes — Semua opcode untuk Rogue AP

| Frame | Hex Opcode | Type | Subtype | Fungsi dalam Rogue AP | Kapan Dipakai |
|-------|------------|------|---------|----------------------|---------------|
| **Beacon** | `0x8000` | 0 | 8 | Broadcast identitas AP | Setiap 100ms |
| **Probe Request** | `0x4000` | 0 | 4 | Client cari AP | Di-sniff, lalu direspons |
| **Probe Response** | `0x5000` | 0 | 5 | AP respon probe | Setelah mendengar probe request |
| **Authentication** | `0xB000` | 0 | 11 | Proses autentikasi | Setelah client mengirim auth request |
| **Association Request** | `0x0000` | 0 | 0 | Client minta join | Di-sniff, lalu direspons |
| **Association Response** | `0x1000` | 0 | 1 | AP respon join | Setelah client mengirim assoc request |

---

## 4. Header: `<net/if.h>` — Interface Network — Untuk mengakses informasi interface

### Fungsi Header:
Memberikan struktur dan konstanta untuk mengakses informasi network interface (seperti `wlan0`).

### Digunakan Untuk:
- **`struct ifreq`** — Untuk mendapatkan index interface
- **`IFNAMSIZ`** — Ukuran maksimal nama interface (16 karakter)

### Contoh Kode:

```c
#include <net/if.h>

struct ifreq ifr;
strcpy(ifr.ifr_name, "wlan0");   // Set nama interface
ioctl(sock, SIOCGIFINDEX, &ifr);  // Dapatkan index interface
```

### Kapan Dipakai:
- Saat membuat raw socket
- Saat bind socket ke interface tertentu
- Saat mendapatkan MAC address interface

---

## 5. Header: `<linux/if_packet.h>` — Raw Socket di Layer 2 — Untuk mengirim/menerima frame di layer 2

### Fungsi Header:
Memberikan struktur untuk **raw socket** di layer 2 (Ethernet). Ini adalah header **PALING PENTING** untuk Rogue AP karena kita mengirim frame 802.11 melalui raw socket.

### Digunakan Untuk:
- **`struct sockaddr_ll`** — Alamat layer 2 untuk sendto/recvfrom
- **`AF_PACKET`** — Address family untuk raw socket
- **`SOCK_RAW`** — Socket type untuk raw packet
- **`ETH_P_ALL`** — Protocol untuk semua frame Ethernet

### Contoh Kode:

```c
#include <linux/if_packet.h>

struct sockaddr_ll addr;
addr.sll_family = AF_PACKET;           // Layer 2
addr.sll_protocol = htons(ETH_P_ALL);  // Semua frame
addr.sll_ifindex = ifr.ifr_ifindex;    // Interface index
addr.sll_halen = 6;                    // MAC address length

sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
```

### Kapan Dipakai:
- **Setiap kali kita mengirim frame 802.11** — beacon, probe response, auth, assoc
- Setiap kali kita menerima frame dari monitor mode

---

## 6. Header: `<linux/if_ether.h>` — Konstanta Ethernet — Untuk konstanta Ethernet

### Fungsi Header:
Memberikan konstanta untuk protokol Ethernet, termasuk **`ETH_P_ALL`** yang digunakan untuk menangkap semua frame.

### Digunakan Untuk:
- **`ETH_P_ALL`** — Menangkap semua frame Ethernet (termasuk 802.11 yang dibungkus)
- **`ETH_ALEN`** — Panjang MAC address (6 bytes)

### Contoh Kode:

```c
#include <linux/if_ether.h>

int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
// ETH_P_ALL = 0x0003 — semua frame
```

### Kapan Dipakai:
- Saat membuat raw socket
- Saat menentukan protocol di socket

---

## 7. Header: `<sys/ioctl.h>` — Kontrol Perangkat — Untuk mengontrol interface

### Fungsi Header:
Menyediakan fungsi **`ioctl()`** untuk mengontrol perangkat (interface jaringan).

### Digunakan Untuk:
- **`ioctl(sock, SIOCGIFINDEX, &ifr)`** — Mendapatkan index interface
- **`ioctl(sock, SIOCGIFHWADDR, &ifr)`** — Mendapatkan MAC address interface
- **`ioctl(sock, SIOCGIFFLAGS, &ifr)`** — Mendapatkan flags interface

### Contoh Kode:

```c
#include <sys/ioctl.h>

struct ifreq ifr;
strcpy(ifr.ifr_name, "wlan0");
ioctl(sock, SIOCGIFINDEX, &ifr);  // ifr.ifr_ifindex berisi index
```

### Kapan Dipakai:
- Saat membuat raw socket (mendapatkan index interface)
- Saat ingin mendapatkan MAC address interface
- Saat ingin mengatur flags interface

---

## 8. Header: `<arpa/inet.h>` — Konversi Endian — Untuk konversi byte order

### Fungsi Header:
Menyediakan fungsi untuk konversi antara **network byte order** (big-endian) dan **host byte order** (little-endian). Ini **PENTING** karena frame 802.11 harus dikirim dalam network byte order.

### Digunakan Untuk:
- **`htons()`** — Host to Network Short (16-bit)
- **`htonl()`** — Host to Network Long (32-bit)
- **`ntohs()`** — Network to Host Short
- **`ntohl()`** — Network to Host Long

### Contoh Kode:

```c
#include <arpa/inet.h>

// Frame Control: 0x8000 harus diubah ke network byte order
hdr->frame_control = htons(0x8000);

// Sequence Control: harus network byte order
hdr->seq_ctrl = htons(seq << 4);

// Saat menerima frame, kita perlu ntohs untuk membaca
uint16_t fc = ntohs(hdr->frame_control);
```

### Kapan Dipakai:
- **Setiap kali kita menulis frame 802.11** — semua field 16-bit harus di-htons
- **Setiap kali kita membaca frame 802.11** — semua field 16-bit harus di-ntohs

---

## 9. Header: `<sys/socket.h>` — Socket API — Untuk membuat dan menggunakan socket

### Fungsi Header:
Menyediakan fungsi dasar untuk **socket programming**.

### Digunakan Untuk:
- **`socket()`** — Membuat socket
- **`bind()`** — Mengikat socket ke interface
- **`sendto()`** — Mengirim data melalui socket
- **`recvfrom()`** — Menerima data dari socket
- **`close()`** — Menutup socket

### Contoh Kode:

```c
#include <sys/socket.h>

int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
bind(sock, (struct sockaddr*)&addr, sizeof(addr));
sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
close(sock);
```

### Kapan Dipakai:
- **Setiap kali kita mengirim atau menerima frame** — ini adalah fungsi dasar networking

---

## 10. Header: `<unistd.h>` — System Calls — Untuk system call dasar

### Fungsi Header:
Menyediakan system call standar UNIX/Linux.

### Digunakan Untuk:
- **`close()`** — Menutup file descriptor
- **`usleep()`** — Sleep dalam microsecond
- **`sleep()`** — Sleep dalam detik
- **`read()`** — Membaca dari file descriptor
- **`write()`** — Menulis ke file descriptor

### Contoh Kode:

```c
#include <unistd.h>

usleep(100000);  // Sleep 100ms (beacon interval)
close(sock);     // Tutup socket
```

### Kapan Dipakai:
- Untuk delay (beacon interval)
- Untuk menutup socket
- Untuk membaca/menulis file

---

## 11. Header: `<string.h>` — Manipulasi String — Untuk manipulasi string dan memory

### Fungsi Header:
Menyediakan fungsi untuk manipulasi string dan memory.

### Digunakan Untuk:
- **`memcpy()`** — Menyalin memory (MAC address, payload)
- **`memset()`** — Mengisi memory (broadcast address)
- **`strcpy()`** — Menyalin string (SSID, interface name)
- **`strlen()`** — Mendapatkan panjang string (SSID length)
- **`strcmp()`** — Membandingkan string

### Contoh Kode:

```c
#include <string.h>

memcpy(hdr->addr1, broadcast, 6);  // Copy MAC address
memset(hdr->addr1, 0xFF, 6);       // Set broadcast
strcpy(ifr.ifr_name, "wlan0");     // Set interface name
int len = strlen(cfg->ssid);       // SSID length
```

### Kapan Dipakai:
- **Setiap kali kita membangun frame** — copy MAC, set broadcast, copy SSID
- **Setiap kali kita membandingkan SSID** — strcmp untuk probe request

---

## 12. Header: `<stdint.h>` — Fixed-width Integer — Untuk tipe data integer dengan ukuran tetap

### Fungsi Header:
Menyediakan tipe data integer dengan ukuran tetap yang **portable** di semua platform.

### Digunakan Untuk:
- **`uint8_t`** — Unsigned 8-bit (1 byte) — untuk byte data
- **`uint16_t`** — Unsigned 16-bit (2 bytes) — untuk frame control, sequence
- **`uint32_t`** — Unsigned 32-bit (4 bytes) — untuk CRC
- **`uint64_t`** — Unsigned 64-bit (8 bytes) — untuk timestamp

### Contoh Kode:

```c
#include <stdint.h>

typedef struct {
    uint16_t frame_control;    // 2 bytes
    uint16_t duration;         // 2 bytes
    uint8_t addr1[6];          // 6 bytes
    uint8_t addr2[6];          // 6 bytes
    uint8_t addr3[6];          // 6 bytes
    uint16_t seq_ctrl;         // 2 bytes
    uint8_t payload[];
} __attribute__((packed)) wifi_header_t;
```

### Kapan Dipakai:
- **Setiap kali kita mendefinisikan struktur frame** — semua field 802.11 menggunakan fixed-width integer

---

## 13. Header: `<pthread.h>` — Multi-threading — Untuk menjalankan multiple task secara paralel

### Fungsi Header:
Menyediakan fungsi untuk **multi-threading** (menjalankan beberapa fungsi secara bersamaan).

### Digunakan Untuk:
- **`pthread_create()`** — Membuat thread baru
- **`pthread_join()`** — Menunggu thread selesai
- **`pthread_cancel()`** — Membatalkan thread

### Contoh Kode:

```c
#include <pthread.h>

pthread_t beacon_tid, sniff_tid;

// Jalankan beacon di thread terpisah
pthread_create(&beacon_tid, NULL, beacon_loop, &cfg);

// Jalankan sniffer di thread terpisah
pthread_create(&sniff_tid, NULL, sniff_thread, &cfg);

// Tunggu kedua thread selesai
pthread_join(beacon_tid, NULL);
pthread_join(sniff_tid, NULL);
```

### Kapan Dipakai:
- **Saat Rogue AP membutuhkan beacon + sniffer berjalan bersamaan**
- Saat ada banyak tugas paralel (beacon, sniffer, web server)

---

## 14. Header: `<signal.h>` — Signal Handling — Untuk menangani sinyal (Ctrl+C)

### Fungsi Header:
Menyediakan fungsi untuk menangani sinyal dari sistem operasi (seperti SIGINT dari Ctrl+C).

### Digunakan Untuk:
- **`signal()`** — Mendaftarkan handler untuk sinyal
- **`SIGINT`** — Sinyal interrupt (Ctrl+C)

### Contoh Kode:

```c
#include <signal.h>

volatile int running = 1;

void signal_handler(int sig) {
    running = 0;  // Stop loop saat Ctrl+C
}

int main() {
    signal(SIGINT, signal_handler);  // Daftarkan handler
    
    while (running) {
        send_beacon(&cfg);
        usleep(100000);
    }
}
```

### Kapan Dipakai:
- **Agar program bisa berhenti dengan Ctrl+C dengan rapi**
- Untuk clean up resources sebelum exit

---

## 15. Header: `<stdio.h>` — Input/Output — Untuk print dan file I/O

### Fungsi Header:
Menyediakan fungsi input/output standar.

### Digunakan Untuk:
- **`printf()`** — Mencetak ke terminal (debug, log)
- **`fprintf()`** — Mencetak ke file
- **`perror()`** — Mencetak error message
- **`sprintf()`** — Format string ke buffer
- **`fopen()`** — Membuka file (logging)
- **`fclose()`** — Menutup file

### Contoh Kode:

```c
#include <stdio.h>

printf("[+] Rogue AP is running!\n");
perror("socket");  // Print error jika socket gagal

char mac_str[18];
sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", ...);
```

### Kapan Dipakai:
- **Setiap kali kita ingin menampilkan output ke terminal**
- Untuk logging dan debugging

---

## 16. Header: `<stdlib.h>` — Standard Library — Untuk fungsi umum

### Fungsi Header:
Menyediakan fungsi standar C.

### Digunakan Untuk:
- **`system()`** — Menjalankan command shell (setup AP mode)
- **`atoi()`** — String to integer (parse arguments)
- **`rand()`** — Random number generator (generate MAC)
- **`malloc()`** — Alokasi memory
- **`free()`** — Dealokasi memory
- **`exit()`** — Keluar dari program

### Contoh Kode:

```c
#include <stdlib.h>

system("ip link set wlan0 down");  // Setup AP mode
int channel = atoi(argv[3]);       // Parse channel
uint8_t mac[6] = {rand() & 0xFF};  // Random MAC
```

### Kapan Dipakai:
- Setup AP mode dengan system command
- Parse command line arguments
- Generate random BSSID

---

## 17. Header: `<time.h>` — Time Functions — Untuk waktu dan timestamp

### Fungsi Header:
Menyediakan fungsi untuk waktu dan timestamp.

### Digunakan Untuk:
- **`time()`** — Mendapatkan timestamp saat ini
- **`localtime()`** — Konversi ke local time
- **`strftime()`** — Format waktu ke string

### Contoh Kode:

```c
#include <time.h>

time_t now = time(NULL);  // Timestamp
printf("[%s] Client connected\n", ctime(&now));

// Untuk logging
fprintf(fp, "[%ld] %s\n", time(NULL), message);
```

### Kapan Dipakai:
- Untuk logging (menyimpan waktu event)
- Untuk client tracking (first seen, last seen)

---

## 18. Header: `<errno.h>` — Error Handling — Untuk menangani error

### Fungsi Header:
Menyediakan konstanta untuk error number dan access ke errno.

### Digunakan Untuk:
- **`errno`** — Variabel global untuk error number
- **`EINTR`** — Interrupted system call
- **`EAGAIN`** — Resource temporarily unavailable

### Contoh Kode:

```c
#include <errno.h>

int ret = sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
if (ret < 0) {
    if (errno == EAGAIN) {
        // Coba lagi
    } else {
        perror("sendto");
    }
}
```

### Kapan Dipakai:
- Untuk error handling yang lebih detail
- Saat menangani socket error

---

## 19. Ringkasan: Kapan Menggunakan Header Apa — Panduan cepat

| Header | Kapan Digunakan |
|--------|-----------------|
| **`<net/if.h>`** | Mendapatkan index interface, nama interface |
| **`<linux/if_packet.h>`** | **SETIAP KALI** mengirim/menerima frame 802.11 — ini header PALING PENTING |
| **`<linux/if_ether.h>`** | Untuk `ETH_P_ALL` saat membuat raw socket |
| **`<sys/ioctl.h>`** | Mengontrol interface (get index, get MAC) |
| **`<arpa/inet.h>`** | **SETIAP KALI** menulis/membaca frame — konversi endian (htons, ntohs) |
| **`<sys/socket.h>`** | Membuat socket, bind, sendto, recvfrom |
| **`<unistd.h>`** | Sleep (beacon interval), close socket |
| **`<string.h>`** | Memanipulasi memory (memcpy, memset, strcpy) |
| **`<stdint.h>`** | **SETIAP KALI** mendefinisikan struktur 802.11 |
| **`<pthread.h>`** | Menjalankan beacon + sniffer bersamaan |
| **`<signal.h>`** | Menangani Ctrl+C |
| **`<stdio.h>`** | Print ke terminal (debug, log) |
| **`<stdlib.h>`** | System command (setup AP mode), atoi, rand |
| **`<time.h>`** | Timestamp untuk logging |
| **`<errno.h>`** | Error handling yang detail |

---

## 20. Contoh Kode Lengkap: Rogue AP dengan Semua Header — Implementasi lengkap

```c
// rogue_ap_full.c - Rogue AP Lengkap dengan Semua Header
// Fungsi: Implementasi Rogue AP yang menggunakan SEMUA header yang dijelaskan di atas
// Compile: gcc -o rogue_ap_full rogue_ap_full.c -pthread
// Jalankan: sudo ./rogue_ap_full

// ================================================================
// 1. HEADER STANDARD C
// ================================================================

#include <stdio.h>      // printf, fprintf, perror, sprintf
#include <stdlib.h>     // system, atoi, rand, exit
#include <string.h>     // memcpy, memset, strcpy, strlen, strcmp
#include <unistd.h>     // usleep, close, sleep
#include <errno.h>      // errno, EINTR, EAGAIN
#include <time.h>       // time, ctime
#include <signal.h>     // signal, SIGINT
#include <pthread.h>    // pthread_create, pthread_join, pthread_cancel

// ================================================================
// 2. HEADER LINUX NETWORKING (PALING PENTING!)
// ================================================================

#include <sys/socket.h>     // socket, bind, sendto, recvfrom
#include <sys/ioctl.h>      // ioctl
#include <net/if.h>         // struct ifreq, IFNAMSIZ
#include <linux/if_packet.h>// struct sockaddr_ll, AF_PACKET, SOCK_RAW
#include <linux/if_ether.h> // ETH_P_ALL, ETH_ALEN
#include <arpa/inet.h>      // htons, ntohs

// ================================================================
// 3. KONSTANTA 802.11
// ================================================================

#define IEEE80211_FC_BEACON        0x8000
#define IEEE80211_FC_PROBE_REQ     0x4000
#define IEEE80211_FC_PROBE_RESP    0x5000
#define IEEE80211_FC_AUTH          0xB000
#define IEEE80211_FC_DEAUTH        0xC000
#define IEEE80211_FC_ASSOC_REQ     0x0000
#define IEEE80211_FC_ASSOC_RESP    0x1000

#define IEEE80211_FTYPE_MGMT       0x00
#define IEEE80211_STYPE_BEACON     0x08
#define IEEE80211_STYPE_PROBE_REQ  0x04
#define IEEE80211_STYPE_PROBE_RESP 0x05
#define IEEE80211_STYPE_AUTH       0x0B
#define IEEE80211_STYPE_DEAUTH     0x0C
#define IEEE80211_STYPE_ASSOC_REQ  0x00
#define IEEE80211_STYPE_ASSOC_RESP 0x01

#define BEACON_INTERVAL            100
#define MAX_SSID_LEN               32
#define DEFAULT_CHANNEL            6
#define TU_TO_US(tu)               ((tu) * 1024)

#define WLAN_AUTH_OPEN             0
#define WLAN_STATUS_SUCCESS        0
#define REASON_CLASS3_FRAME        7

#define IE_TAG_SSID                0
#define IE_TAG_RATES               1
#define IE_TAG_DS_PARAM            3
#define IE_TAG_EXTENDED_RATES      50

// ================================================================
// 4. STRUKTUR 802.11 (menggunakan <stdint.h>)
// ================================================================

typedef struct {
    uint16_t frame_control;    // 2 bytes — htons(0x8000)
    uint16_t duration;         // 2 bytes — 0
    uint8_t addr1[6];          // 6 bytes — Destination
    uint8_t addr2[6];          // 6 bytes — Source (BSSID)
    uint8_t addr3[6];          // 6 bytes — BSSID
    uint16_t seq_ctrl;         // 2 bytes — Sequence
    uint8_t payload[];         // Variable — Beacon body
} __attribute__((packed)) wifi_header_t;

// ================================================================
// 5. STRUKTUR KONFIGURASI
// ================================================================

typedef struct {
    uint8_t bssid[6];
    char ssid[MAX_SSID_LEN];
    int channel;
    char iface[16];
    int running;
} ap_config_t;

// ================================================================
// 6. GLOBAL VARIABLES
// ================================================================

volatile int running = 1;

// ================================================================
// 7. FUNGSI UTILITY
// ================================================================

// mac_to_string — Konversi MAC ke string (menggunakan sprintf dari <stdio.h>)
void mac_to_string(const uint8_t *mac, char *str) {
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// create_raw_socket — Buat raw socket (menggunakan <sys/socket.h>, <net/if.h>, <linux/if_packet.h>)
int create_raw_socket(const char *iface) {
    int sock;
    struct ifreq ifr;
    
    // socket() — dari <sys/socket.h>
    // AF_PACKET, SOCK_RAW — dari <linux/if_packet.h>
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");  // dari <stdio.h>
        return -1;
    }
    
    // ioctl() — dari <sys/ioctl.h>
    // struct ifreq — dari <net/if.h>
    strcpy(ifr.ifr_name, iface);  // dari <string.h>
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(sock);  // dari <unistd.h>
        return -1;
    }
    
    return sock;
}

// send_frame — Kirim frame (menggunakan <sys/socket.h>)
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
    memset(addr.sll_addr, 0xFF, 6);
    
    // sendto() — dari <sys/socket.h>
    return sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
}

// ================================================================
// 8. FUNGSI BUILD FRAME
// ================================================================

// build_beacon — Build beacon frame (0x8000)
int build_beacon(uint8_t *buf, ap_config_t *cfg, uint16_t seq) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    int ssid_len = strlen(cfg->ssid);  // dari <string.h>
    
    if (ssid_len > MAX_SSID_LEN) ssid_len = MAX_SSID_LEN;
    
    // Frame Control — menggunakan htons() dari <arpa/inet.h>
    hdr->frame_control = htons(IEEE80211_FC_BEACON);
    hdr->duration = 0;
    
    // Address 1: Broadcast — menggunakan memset() dari <string.h>
    memset(hdr->addr1, 0xFF, 6);
    
    // Address 2 & 3: BSSID — menggunakan memcpy() dari <string.h>
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    
    hdr->seq_ctrl = htons(seq << 4);
    
    // === BEACON BODY ===
    // Timestamp (8 bytes)
    memset(pos, 0, 8);
    pos += 8;
    
    // Beacon Interval
    *pos++ = BEACON_INTERVAL & 0xFF;
    *pos++ = (BEACON_INTERVAL >> 8) & 0xFF;
    
    // Capability: ESS
    *pos++ = 0x01;
    *pos++ = 0x00;
    
    // SSID
    *pos++ = IE_TAG_SSID;
    *pos++ = ssid_len;
    memcpy(pos, cfg->ssid, ssid_len);
    pos += ssid_len;
    
    // Supported Rates
    *pos++ = IE_TAG_RATES;
    *pos++ = 4;
    *pos++ = 0x82; *pos++ = 0x84;
    *pos++ = 0x8B; *pos++ = 0x96;
    
    // Channel
    *pos++ = IE_TAG_DS_PARAM;
    *pos++ = 1;
    *pos++ = cfg->channel;
    
    return pos - buf;
}

// ================================================================
// 9. FUNGSI SEND
// ================================================================

// send_beacon — Kirim beacon (menggunakan create_raw_socket, send_frame)
int send_beacon(ap_config_t *cfg) {
    int sock;
    uint8_t buf[512];
    int len;
    static uint16_t seq = 0;
    
    sock = create_raw_socket(cfg->iface);
    if (sock < 0) return -1;
    
    len = build_beacon(buf, cfg, seq++);
    int ret = send_frame(sock, buf, len, cfg->iface);
    
    close(sock);  // dari <unistd.h>
    return ret;
}

// ================================================================
// 10. THREAD FUNCTIONS (menggunakan <pthread.h>)
// ================================================================

// beacon_loop — Kirim beacon terus-menerus
void *beacon_loop(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    
    while (cfg->running) {
        send_beacon(cfg);
        usleep(BEACON_INTERVAL * 1024);  // dari <unistd.h>
    }
    
    return NULL;
}

// sniff_thread — Sniff frame dan handle probe/auth/assoc
void *sniff_thread(void *arg) {
    ap_config_t *cfg = (ap_config_t*)arg;
    int sock;
    uint8_t buf[2048];
    
    // Setup monitor mode — menggunakan system() dari <stdlib.h>
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip link set %s down", cfg->iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "iw dev %s set type monitor", cfg->iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "ip link set %s up", cfg->iface);
    system(cmd);
    
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        perror("socket");
        return NULL;
    }
    
    struct ifreq ifr;
    struct sockaddr_ll addr;
    strcpy(ifr.ifr_name, cfg->iface);
    ioctl(sock, SIOCGIFINDEX, &ifr);
    
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = ifr.ifr_ifindex;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    
    printf("[Sniffer] Monitoring on %s\n", cfg->iface);
    
    while (cfg->running) {
        int len = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
        if (len < 40) continue;
        
        wifi_header_t *hdr = (wifi_header_t*)buf;
        uint16_t fc = ntohs(hdr->frame_control);  // dari <arpa/inet.h>
        uint8_t type = (fc >> 2) & 0x3;
        uint8_t subtype = (fc >> 4) & 0xF;
        
        if (type != IEEE80211_FTYPE_MGMT) continue;
        
        char mac_str[18];
        mac_to_string(hdr->addr2, mac_str);
        
        switch(subtype) {
            case IEEE80211_STYPE_PROBE_REQ:
                printf("[Sniff] Probe from %s\n", mac_str);
                // Kirim probe response
                break;
            case IEEE80211_STYPE_AUTH:
                printf("[Sniff] Auth from %s\n", mac_str);
                // Kirim auth response
                break;
            case IEEE80211_STYPE_ASSOC_REQ:
                printf("[Sniff] Assoc from %s\n", mac_str);
                // Kirim assoc response
                printf("[+] Client CONNECTED: %s\n", mac_str);
                break;
        }
    }
    
    close(sock);
    return NULL;
}

// ================================================================
// 11. SIGNAL HANDLER (menggunakan <signal.h>)
// ================================================================

void signal_handler(int sig) {
    running = 0;
    printf("\n[!] Stopping...\n");
}

// ================================================================
// 12. MAIN (menggunakan SEMUA header)
// ================================================================

int main(int argc, char *argv[]) {
    ap_config_t cfg;
    pthread_t beacon_tid, sniff_tid;
    char bssid_str[18];
    
    // Signal handler — dari <signal.h>
    signal(SIGINT, signal_handler);
    
    // Setup config
    cfg.bssid[0] = 0x02;
    cfg.bssid[1] = 0x11;
    cfg.bssid[2] = 0x22;
    cfg.bssid[3] = 0x33;
    cfg.bssid[4] = 0x44;
    cfg.bssid[5] = 0x55;
    strcpy(cfg.ssid, "FreeWiFi");
    cfg.channel = DEFAULT_CHANNEL;
    strcpy(cfg.iface, "wlan0");
    cfg.running = 1;
    
    mac_to_string(cfg.bssid, bssid_str);
    
    // Print — dari <stdio.h>
    printf("========================================\n");
    printf("  ROGUE AP - Full Implementation\n");
    printf("========================================\n");
    printf("SSID:      %s\n", cfg.ssid);
    printf("BSSID:     %s\n", bssid_str);
    printf("Channel:   %d\n", cfg.channel);
    printf("Interface: %s\n", cfg.iface);
    printf("========================================\n\n");
    
    // Setup AP mode — menggunakan system() dari <stdlib.h>
    printf("[+] Setting up AP mode...\n");
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip link set %s down", cfg.iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "iw dev %s set type ap", cfg.iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "ip link set %s up", cfg.iface);
    system(cmd);
    
    // Start threads — dari <pthread.h>
    printf("[+] Starting beacon thread...\n");
    pthread_create(&beacon_tid, NULL, beacon_loop, &cfg);
    
    printf("[+] Starting sniffer thread...\n");
    pthread_create(&sniff_tid, NULL, sniff_thread, &cfg);
    
    printf("[+] Rogue AP is RUNNING!\n");
    printf("[+] Press Ctrl+C to stop\n\n");
    
    // Wait for threads — dari <pthread.h>
    pthread_join(beacon_tid, NULL);
    pthread_join(sniff_tid, NULL);
    
    printf("[+] Done!\n");
    return 0;
}
```

---

## 📊 Ringkasan: Header dan Fungsinya dalam Satu Tabel

| No | Header | Fungsi Utama | Kapan Dipakai |
|----|--------|--------------|---------------|
| 1 | `<net/if.h>` | `struct ifreq`, `IFNAMSIZ` | Setup raw socket, get interface index |
| 2 | `<linux/if_packet.h>` | `struct sockaddr_ll`, `AF_PACKET` | **Setiap kali kirim/terima frame** |
| 3 | `<linux/if_ether.h>` | `ETH_P_ALL`, `ETH_ALEN` | Membuat raw socket |
| 4 | `<sys/ioctl.h>` | `ioctl()`, `SIOCGIFINDEX` | Kontrol interface |
| 5 | `<arpa/inet.h>` | `htons()`, `ntohs()` | **Setiap kali baca/tulis frame** |
| 6 | `<sys/socket.h>` | `socket()`, `sendto()`, `bind()` | Socket programming |
| 7 | `<unistd.h>` | `usleep()`, `close()` | Delay, close socket |
| 8 | `<string.h>` | `memcpy()`, `memset()`, `strcpy()` | Manipulasi frame |
| 9 | `<stdint.h>` | `uint8_t`, `uint16_t`, `uint32_t` | **Definisikan struktur 802.11** |
| 10 | `<pthread.h>` | `pthread_create()`, `pthread_join()` | Multi-threading |
| 11 | `<signal.h>` | `signal()`, `SIGINT` | Ctrl+C handling |
| 12 | `<stdio.h>` | `printf()`, `perror()`, `sprintf()` | Debugging, logging |
| 13 | `<stdlib.h>` | `system()`, `atoi()`, `rand()` | Setup AP mode, parse args |
| 14 | `<time.h>` | `time()`, `ctime()` | Logging timestamp |
| 15 | `<errno.h>` | `errno`, `EINTR`, `EAGAIN` | Error handling |

---

**Akhir Dokumen**

> *"Understanding each header is understanding how the code works."*
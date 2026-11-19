# 📡 Instruction Set & Bytecode 802.11 — Petualangan Membangun "Bahasa" WiFi!

**Versi:** 5.2 (Enhanced Audited Edition with Complete Source Comments & Deep-Dive Headers)  
**Tanggal:** 17 November 2026  
**Lisensi:** MIT

---

> 🧭 **Trilogi Belajar WiFi & Network System Programming:**
> 1. 📡 **[Dokumen 1: IEEE 802.11 — Petualangan WiFi dari Nol sampai Hero!](01_802.11.md)**: Teori protokol, arsitektur, anatomi frame, dan evolusi keamanan.
> 2. 🔤 **[Dokumen 2: Instruction Set & Bytecode 802.11](02_instruction_set.md)** *(Dokumen ini)*: "Bahasa" dan opcode WiFi, anatomi bit/byte, serta 15 Header Linux untuk raw socket.
> 3. 🍍 **[Dokumen 3: WiFi Pineapple — Dari Nol sampai Legendaris](03_wifi_pineapple.md)**: Implementasi penuh 12 fase C system programming, lab audit keamanan, dan pertahanan jaringan.

---

## 🎯 Apa yang Akan Kamu Pelajari?

Bayangkan kamu sedang belajar bahasa asing. Sebelum bisa ngobrol, kamu harus tahu:
- 📝 **Alfabet** → Di 802.11 ini adalah **byte**
- 🔤 **Kata** → Di 802.11 ini adalah **frame**
- 💬 **Kalimat** → Di 802.11 ini adalah **opcode**

Nah, dokumen ini akan mengajarkan kamu "bahasa" WiFi dari nol! Kita akan belajar:
- 🧠 **Konsep bytecode 802.11** — Apa itu "opcode" WiFi?
- 📦 **Struktur frame** — Bagaimana "kalimat" WiFi disusun
- 🔧 **15 Header Linux** — "Kamus" yang kita butuhkan untuk ngobrol dengan WiFi
- 💻 **Source code lengkap** — Contoh nyata "percakapan" dengan WiFi

---

## 📋 Daftar Isi

1. [Prolog: WiFi itu Bahasa!](#1-prolog-wifi-itu-bahasa)
2. [Bab 1: Apa Itu Bytecode 802.11?](#2-bab-1-apa-itu-bytecode-80211)
3. [Bab 2: Struktur Frame 802.11](#3-bab-2-struktur-frame-80211)
4. [Bab 3: Management Frame Opcodes](#4-bab-3-management-frame-opcodes)
5. [Bab 4: 15 Header Linux yang WAJIB Kamu Tahu](#5-bab-4-15-header-linux-yang-wajib-kamu-tahu)
6. [Bab 5: Source Code Lengkap — Rogue AP dengan Semua Header](#6-bab-5-source-code-lengkap--rogue-ap-dengan-semua-header)
7. [Bab 6: Ringkasan Header & Kapan Memakainya](#7-bab-6-ringkasan-header--kapan-memakainya)
8. [Bab 7: Jembatan Menuju WiFi Pineapple](#8-bab-7-jembatan-menuju-wifi-pineapple)
9. [Bab 8: Kurikulum Belajar 4 Tahap: Menguasai Bytecode 802.11 & Linux Raw Socket Programming](#9-bab-8-kurikulum-belajar-4-tahap-menguasai-bytecode-80211--linux-raw-socket-programming-)
10. [Bab 9: Lampiran: Cheat Sheet & Fun Facts](#10-bab-9-lampiran-cheat-sheet--fun-facts-)

---

## 1. Prolog: WiFi itu Bahasa! 🗣️

Bayangkan kamu pergi ke negara asing. Kamu tidak bisa ngobrol karena tidak tahu bahasanya. Tapi kalau kamu belajar alfabet, kata-kata, dan tata bahasanya, tiba-tiba kamu bisa berkomunikasi!

**WiFi juga sama!** 

WiFi punya "bahasa" sendiri yang disebut **802.11 protocol**. Dan di dokumen ini, kita akan belajar "bahasa" itu dari nol:

```
┌─────────────────────────────────────────────────────────────┐
│                    BAHASA WiFi = 802.11                     │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  📝 Alfabet = Byte (0x00 - 0xFF)                           │
│  🔤 Kata = Frame (Beacon, Probe, Auth, dll)                │
│  💬 Kalimat = Opcode (0x8000, 0x4000, 0xB000, dll)         │
│  🗣️ Percakapan = Komunikasi client ↔ AP                    │
│                                                             │
│  Contoh "Percakapan":                                      │
│  📱 Client: "Ada FreeWiFi tidak?" (Probe Request 0x4000)   │
│  📡 AP: "Ada! Saya FreeWiFi!" (Probe Response 0x5000)      │
│  📱 Client: "Boleh connect?" (Auth Request 0xB000)         │
│  📡 AP: "Boleh!" (Auth Response 0xB000)                    │
│  📱 Client: "Saya mau join!" (Assoc Request 0x0000)        │
│  📡 AP: "Silahkan!" (Assoc Response 0x1000)                │
│  📱 Client: "CONNECTED!" ✅                                │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

**Kenapa kita harus belajar "bahasa" ini?**

Karena kalau kamu paham "bahasa" WiFi, kamu bisa:
- 🎭 **Menyamar** jadi AP (Rogue AP)
- 🧲 **Memancing** client (Karma Attack)
- 🚪 **Mengusir** client dari AP asli (Deauth Attack)
- 👂 **Menguping** percakapan (MITM)
- 🔑 **Mencuri** password (Credential Capture)

**Tapi ingat:** Ilmu ini untuk **MELINDUNGI**, bukan merusak! 🛡️

---

## 2. Bab 1: Apa Itu Bytecode 802.11? 🧠

### 2.1. Konsep Dasar: Frame = Instruksi

802.11 bukanlah prosesor dengan *instruction set* seperti CPU. Tapi frame-frame 802.11 bisa dianggap sebagai **"instruksi"** yang dikirimkan melalui udara.

Setiap frame punya **opcode** (frame control) yang menentukan jenis instruksi.

**Analogi Sederhana:**

Bayangkan kamu lagi di restoran:

```
┌─────────────────────────────────────────────────────────────┐
│                    RESTORAN WiFi                            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  📡 AP = Pelayan                                           │
│  📱 Client = Pelanggan                                     │
│  📦 Frame = Pesanan                                        │
│  🔢 Opcode = Kode Pesanan                                  │
│                                                             │
│  Contoh "Pesanan":                                         │
│  📱 Pelanggan: "Mau pesan nasi goreng" (Opcode: 0x4000)   │
│  📡 Pelayan: "Siap! Nasi goreng coming soon!" (0x5000)    │
│  📱 Pelanggan: "Mau bayar" (Opcode: 0xB000)               │
│  📡 Pelayan: "Oke, ini tagihannya" (Opcode: 0xB000)       │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 2.2. Bytecode (Hex) — Representasi Hex dari Setiap Frame

Setiap frame punya representasi hex yang unik:

| Frame | Hex Opcode | Analogi | Kapan Dipakai |
|-------|------------|---------|---------------|
| **Beacon** | `0x8000` | "Halo! Saya AP bernama FreeWiFi!" | Setiap 100ms (broadcast) |
| **Probe Request** | `0x4000` | "Ada AP bernama FreeWiFi tidak?" | Client cari AP |
| **Probe Response** | `0x5000` | "Ada! Saya FreeWiFi!" | AP jawab probe |
| **Authentication** | `0xB000` | "Boleh connect?" / "Boleh!" | Proses autentikasi |
| **Association Request** | `0x0000` | "Saya mau join!" | Client minta join |
| **Association Response** | `0x1000` | "Silahkan!" | AP terima client |
| **Deauthentication** | `0xC000` | "Keluar!" | Putus koneksi paksa |

**Contoh Bytecode dalam Hex Dump:**

```text
Beacon:     80 00 00 00 FF FF FF FF FF FF ...   // 0x8000
Probe Req:  40 00 00 00 FF FF FF FF FF FF ...   // 0x4000
Probe Resp: 50 00 00 00 00 11 22 33 44 55 ...   // 0x5000
Auth:       B0 00 00 00 00 11 22 33 44 55 ...   // 0xB000
Assoc:      00 00 00 00 00 11 22 33 44 55 ...   // 0x0000
Deauth:     C0 00 00 00 00 11 22 33 44 55 ...   // 0xC000
```

> 🎓 **Fakta Menarik:** Dua byte pertama (`80 00`, `40 00`, dll) adalah **Frame Control** yang menentukan jenis frame. Ini seperti "kode pesanan" di restoran!

---

## 3. Bab 2: Struktur Frame 802.11 📦

### 3.1. Header 802.11 (24 bytes) — "Amplop Surat" WiFi

Setiap frame 802.11 punya struktur MAC header yang harus kita bangun secara manual:

```
┌─────────────────────────────────────────────────────────────┐
│                    802.11 MAC HEADER                        │
├─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬───────┐
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

**Analogi: Amplop Surat**

Bayangkan setiap frame seperti **amplop surat**:

| Field | Ukuran | Fungsi | Analogi |
|-------|--------|--------|---------|
| **Frame Control** | 2 bytes | Tentukan jenis frame | "Jenis surat: biasa, terdaftar, atau kilat" |
| **Duration** | 2 bytes | NAV (Network Allocation Vector) | "Berapa lama surat ini berlaku" |
| **Address 1** | 6 bytes | Destination MAC | "Kepada: Budi" |
| **Address 2** | 6 bytes | Source MAC / BSSID | "Dari: Ani" |
| **Address 3** | 6 bytes | BSSID / SA | "Alamat kantor pos" |
| **Sequence Control** | 2 bytes | Nomor urut frame | "Surat ke-5 dari 10" |
| **Payload** | Variable | Data yang dikirim | "Isi surat" |
| **FCS** | 4 bytes | Frame Check Sequence | "Stempel pos: pastikan surat tidak rusak" |

### 3.2. Frame Control Field (2 bytes) — "Kepala Surat"

Frame Control adalah 16 bit (2 byte) yang menentukan **jenis frame dan perilakunya**:

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       FRAME CONTROL (16 BITS)                                          │
├────────────────────────────────────────┬───────────────────────────────────────────────────────────────┤
│            BYTE 0 (Bit 0 - 7)          │                     BYTE 1 (Bit 8 - 15)                       │
├────────────┬──────────┬────────────────┼───────┬─────────┬──────┬───────┬───────┬──────┬──────────┬────┤
│ Bit 0 - 1  │ Bit 2 - 3│   Bit 4 - 7    │ Bit 8 │  Bit 9  │Bit 10│Bit 11 │Bit 12 │Bit 13│  Bit 14  │B15 │
├────────────┼──────────┼────────────────┼───────┼─────────┼──────┼───────┼───────┼──────┼──────────┼────┤
│  Protocol  │   Type   │    Subtype     │ To DS │ From DS │ More │ Retry │ Power │ More │Protected │Ord-│
│  Version   │ (00/01/  │  (Beacon/Probe/│       │         │ Frag │       │ Mgmt  │ Data │  Frame   │ er │
│    (00)    │  10/11)  │   Auth/Assoc)  │       │         │      │       │       │      │  (WPA)   │    │
└────────────┴──────────┴────────────────┴───────┴─────────┴──────┴───────┴───────┴──────┴──────────┴────┘
```

> 💡 **Kenapa Opcode Beacon Ditulis `0x8000`? (Rahasia Endianness!)**  
> Di udara (*over the air*), Byte 0 dikirim lebih dulu:
> - Subtype Beacon = `1000` (biner), Type Mgmt = `00`, Version = `00`
> - Digabung jadi Byte 0: `1000 00 00` biner = **`0x80`** hex!
> - Byte 1 semua flag 0: `0000 0000` biner = **`0x00`** hex!
> 
> Saat kita memprogram dalam bahasa C pada prosesor modern (x86/ARM little-endian), agar urutan byte di kabel/udara menjadi `[0x80, 0x00]`, kita menulis konstanta 16-bit **`0x8000`** lalu dibungkus fungsi `htons(0x8000)`. Keren, kan? 🎯

**Type (bit 2-3):**
- `00` = **Management** — Ini yang kita pakai untuk membangun Rogue AP & pertahanan
- `01` = **Control** — RTS (Request to Send), CTS (Clear to Send), ACK
- `10` = **Data** — Aliran payload data IP/TCP/UDP

**Subtype (bit 4-7) untuk Management Frames:**

| Subtype | Hex (Opcode) | Fungsi | Analogi |
|---------|--------------|--------|---------|
| `0000` | `0x0000` | Association Request | "Halo AP, saya mau bergabung ke jaringanmu!" |
| `0001` | `0x1000` | Association Response | "Selamat datang! Kamu resmi diterima!" |
| `0100` | `0x4000` | Probe Request | "Apakah ada WiFi 'FreeWiFi' di sekitar sini?" |
| `0101` | `0x5000` | Probe Response | "Ada! Saya 'FreeWiFi', ini info kapabilitas saya!" |
| `1000` | `0x8000` | Beacon | "Pemberitahuan berkala: Saya AP aktif di channel ini!" |
| `1011` | `0xB000` | Authentication | "Boleh minta izin handshake?" / "Izin diberikan!" |
| `1100` | `0xC000` | Deauthentication | "Putus hubungan! Kamu dikeluarkan dari jaringan!" |

> 🎓 **Fakta Detektif:** Frame Control adalah **2 byte pertama** dari setiap frame 802.11. Kalau kamu membuka Wireshark atau melihat raw hex dump dan melihat byte awal `80 00`, kamu langsung tahu itu adalah **Beacon Frame**!

---

## 4. Bab 3: Management Frame Opcodes 🎭

### 4.1. Semua Opcode untuk Rogue AP

Ini adalah "kamus" opcode yang kita butuhkan untuk membangun Rogue AP:

| Frame | Hex Opcode | Type | Subtype | Fungsi dalam Rogue AP | Kapan Dipakai |
|-------|------------|------|---------|-----------------------|---------------|
| **Beacon** | `0x8000` | 0 | 8 | Broadcast identitas AP | Setiap 100ms |
| **Probe Request** | `0x4000` | 0 | 4 | Client cari AP | Di-sniff, lalu direspons |
| **Probe Response** | `0x5000` | 0 | 5 | AP respon probe | Setelah mendengar probe request |
| **Authentication** | `0xB000` | 0 | 11 | Proses autentikasi | Setelah client mengirim auth request |
| **Association Request** | `0x0000` | 0 | 0 | Client minta join | Di-sniff, lalu direspons |
| **Association Response** | `0x1000` | 0 | 1 | AP respon join | Setelah client mengirim assoc request |

### 4.2. Proses Koneksi WiFi — "Percakapan" Lengkap

Mari kita lihat "percakapan" lengkap antara client dan AP:

```
┌─────────────────────────────────────────────────────────────┐
│                    PROSES KONEKSI WiFi                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  📱 Client                         📡 AP                   │
│    │                             │                         │
│    │ 1. Beacon (setiap 100ms)    │                         │
│    │ Opcode: 0x8000              │                         │
│    │ "Halo! Saya FreeWiFi!"     │                         │
│    │<────────────────────────────│                         │
│    │                             │                         │
│    │ 2. Probe Request            │                         │
│    │ Opcode: 0x4000              │                         │
│    │ "Ada FreeWiFi?"            │                         │
│    │────────────────────────────>│                         │
│    │                             │                         │
│    │ 3. Probe Response           │                         │
│    │ Opcode: 0x5000              │                         │
│    │ "Ada! Saya FreeWiFi!"      │                         │
│    │<────────────────────────────│                         │
│    │                             │                         │
│    │ 4. Authentication Request   │                         │
│    │ Opcode: 0xB000              │                         │
│    │ "Boleh connect?"           │                         │
│    │────────────────────────────>│                         │
│    │                             │                         │
│    │ 5. Authentication Response  │                         │
│    │ Opcode: 0xB000              │                         │
│    │ "Boleh!"                   │                         │
│    │<────────────────────────────│                         │
│    │                             │                         │
│    │ 6. Association Request      │                         │
│    │ Opcode: 0x0000              │                         │
│    │ "Saya mau join!"           │                         │
│    │────────────────────────────>│                         │
│    │                             │                         │
│    │ 7. Association Response     │                         │
│    │ Opcode: 0x1000              │                         │
│    │ "Silahkan, AID=1!"         │                         │
│    │<────────────────────────────│                         │
│    │                             │                         │
│    │ 8. CONNECTED ✅             │                         │
│    │                             │                         │
└─────────────────────────────────────────────────────────────┘
```

> 🎓 **Fakta Menarik:** Seluruh proses koneksi ini terjadi dalam **hitungan milidetik!** Cepat banget, kan? 🚀

---

## 5. Bab 4: 15 Header Linux yang WAJIB Kamu Tahu 🔧

### 5.1. Kenapa Kita Butuh Header?

Bayangkan kamu mau ngobrol dengan orang asing. Kamu butuh **kamus** untuk menerjemahkan kata-kata. Di dunia programming C, **file header (`.h`)** adalah "buku kontrak" dan "kamus" yang memberitahu compiler tentang:
1. **Struktur data (`struct`)**: Bagaimana susunan byte di memori.
2. **Konstanta (`#define`)**: Nama-nama panggilan untuk angka biner tertentu.
3. **Fungsi (`prototype`)**: Nama fungsi, parameter apa yang diminta, dan apa tipe nilai kembaliannya.

Untuk membangun Rogue AP dan sistem pentesting nirkabel tingkat rendah, kita membutuhkan **15 header standar** dari Linux kernel dan C Standard Library (POSIX). Mari kita bedah tuntas satu per satu beserta contoh kode beranotasi lengkap!

---

### 5.2. Header #1: `<net/if.h>` — Interface Network 🌐

**Fungsi:** Memberikan struktur data standar dan makro konstanta sistem operasi untuk berinteraksi dengan antarmuka jaringan fisik maupun virtual (seperti `eth0`, `wlan0`, `mon0`, `ap0`).

**Analogi:** Ini seperti **buku telepon resmi kantor pos** yang mencatat nama jalan, nomor gerbang, dan identitas semua pos cabang pengiriman kargo di wilayahmu.

**Digunakan Untuk:**
- `struct ifreq` — Struktur multi-guna untuk mengirim dan menerima parameter interface melalui ioctl.
- `IFNAMSIZ` — Konstanta panjang maksimal nama interface (16 bytes = 15 karakter teks + 1 null terminator `\0`).
- `IFF_UP`, `IFF_PROMISC`, `IFF_BROADCAST` — Flag status operasional kartu jaringan.

**🔬 Deep Dive: Struktur & Makro Kunci di `<net/if.h>`:**
- `struct ifreq`: Di dalamnya terdapat `union` fleksibel. Field `ifr_name` untuk memasukkan nama interface, sedangkan `ifr_ifindex` akan otomatis diisi nomor indeks kartu jaringan oleh kernel saat kita memanggil `ioctl(sock, SIOCGIFINDEX, &ifr)`.
- `if_nametoindex("mon0")`: Fungsi pembantu praktis untuk langsung mendapatkan interface index dari string nama interface tanpa perlu membuat `struct ifreq` manual.

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <net/if.h>     // Header #1: Definisi struct ifreq dan konstanta IFNAMSIZ
#include <string.h>     // Dibutuhkan untuk memset() dan strncpy()
#include <sys/ioctl.h>  // Dibutuhkan untuk fungsi ioctl()
#include <stdio.h>      // Dibutuhkan untuk printf() dan perror()

// 1. Alokasikan struktur request interface di stack memori
struct ifreq ifr;

// 2. Selalu bersihkan memori struktur dengan nol untuk mencegah memory leak / sampah memori
memset(&ifr, 0, sizeof(ifr));

// 3. Salin nama kartu interface target (gunakan strncpy dengan batas IFNAMSIZ - 1 agar aman dari overflow)
strncpy(ifr.ifr_name, "wlan0mon", IFNAMSIZ - 1);

// 4. Panggil kernel Linux via ioctl untuk mengisi ifr.ifr_ifindex dengan nomor urut kartu jaringan
if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
    // Jika nama interface salah atau belum diaktifkan, kernel akan mengembalikan nilai negatif (-1)
    perror("[-] Gagal mengambil index kartu jaringan");
} else {
    // Indeks kartu jaringan berhasil didapat (misal: ifindex = 3)
    printf("[+] Interface %s memiliki index sistem: %d\n", ifr.ifr_name, ifr.ifr_ifindex);
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Lupa `memset` ke 0:** `struct ifreq` berukuran cukup besar (~40 byte) dan berisi union. Jika tidak di-zero-kan, data sampah di stack memori bisa terbaca oleh kernel dan memicu error `EINVAL` (Invalid Argument).
- **Buffer Overflow Nama Interface:** Jangan pernah gunakan `strcpy(ifr.ifr_name, arg)`. Jika pengguna memasukkan nama interface lebih dari 15 karakter, memori program akan korup. Selalu gunakan `strncpy(ifr.ifr_name, arg, IFNAMSIZ - 1)` dan pastikan `ifr.ifr_name[IFNAMSIZ - 1] = '\0'`.

**Kapan Dipakai:**
- Menemukan interface index sebelum melakukan `bind()` pada raw socket.
- Membaca atau memalsukan MAC address hardware interface (`SIOCGIFHWADDR`).
- Mengubah status interface UP atau DOWN secara programatik.

---

### 5.3. Header #2: `<linux/if_packet.h>` — Raw Socket di Layer 2 📦

**Fungsi:** Memberikan struktur pengalamatan fisik tingkat rendah untuk socket protokol keluarga `AF_PACKET`. Header ini adalah **KUNCI UTAMA** pembuatan Rogue AP, Deauthentication Injector, dan Sniffer, karena memungkinkan transmisi paket mentah Layer 2 (Data Link) langsung menembus driver Wi-Fi tanpa diproses atau dimodifikasi oleh TCP/IP stack kernel Linux.

**Analogi:** Ini seperti **pintu kargo khusus di bagian belakang bandara** — kamu bisa langsung memasukkan paket custom rancanganmu sendiri ke dalam lambung pesawat kargo tanpa melewati mesin x-ray dan loket penimbangan bagasi umum!

**Digunakan Untuk:**
- `struct sockaddr_ll` — Struktur alamat soket level Link-Layer (Physical Layer device address).
- `PACKET_MR_PROMISC` — Flag opsi socket untuk mengaktifkan kartu ke mode promiscuous secara manual.
- Mengirim dan menerima paket mentah termasuk header Ethernet/802.11 buatan sendiri.

**🔬 Deep Dive: Struktur & Makro Kunci di `<linux/if_packet.h>`:**
- `struct sockaddr_ll`:
  - `sll_family`: Selalu diisi `AF_PACKET`.
  - `sll_protocol`: Protokol yang ingin ditangkap dalam network byte order (contoh: `htons(ETH_P_ALL)` untuk semua protokol).
  - `sll_ifindex`: Angka indeks interface tujuan (didapat dari Header #1 `<net/if.h>`).
  - `sll_hatype`: Tipe hardware header (contoh: `ARPHRD_ETHER` atau `ARPHRD_IEEE80211`).
  - `sll_halen`: Panjang alamat hardware (6 byte untuk Wi-Fi / Ethernet).
  - `sll_addr[8]`: Alamat fisik hardware pengirim atau tujuan.

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <linux/if_packet.h> // Header #2: Definisi struct sockaddr_ll dan protokol AF_PACKET
#include <net/if.h>          // Dibutuhkan untuk ifr.ifr_ifindex
#include <arpa/inet.h>       // Dibutuhkan untuk fungsi konversi endianness htons()
#include <sys/socket.h>      // Dibutuhkan untuk syscall bind() dan socket()
#include <string.h>          // Dibutuhkan untuk memset()
#include <stdio.h>           // Dibutuhkan untuk printf() dan perror()
#include <unistd.h>          // Dibutuhkan untuk close()

// 1. Siapkan struktur alamat low-level link layer
struct sockaddr_ll sll;

// 2. Kosongkan seluruh field struktur (sangat krusial agar field internal tidak terisi garbage)
memset(&sll, 0, sizeof(sll));

// 3. Tentukan address family: AF_PACKET adalah keluarga soket khusus Linux untuk paket layer link
sll.sll_family   = AF_PACKET;

// 4. Pasang nomor indeks kartu jaringan (didapat dari ioctl SIOCGIFINDEX)
sll.sll_ifindex  = ifr.ifr_ifindex;

// 5. Tentukan protokol: ETH_P_ALL berarti dengarkan atau kirimkan semua jenis protokol
sll.sll_protocol = htons(ETH_P_ALL);

// 6. Ikat (bind) soket ke interface spesifik tersebut agar paket tidak terlempar ke kartu LAN/loopback
if (bind(sock, (struct sockaddr *)&sll, sizeof(sll)) < 0) {
    perror("[-] Gagal mengikat raw socket ke interface");
    close(sock);
    return -1;
}
printf("[+] Raw socket berhasil terikat sempurna di layer link!\n");
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Menggunakan `AF_INET` bukan `AF_PACKET`:** Jika kamu membuat socket dengan `AF_INET`, kernel menganggap kamu ingin mengirim paket IP (Layer 3), bukan frame 802.11 mentah (Layer 2). Kernel akan menolak data 802.11 buatanmu!
- **Tidak Mem-bind Socket:** Jika raw socket `AF_PACKET` tidak di-bind ke interface tertentu, saat kamu memanggil `recvfrom()`, programmu akan menangkap paket dari SEMUA interface komputer (termasuk `eth0`, `lo`, `docker0`), yang akan membanjiri CPU-mu!

**Kapan Dipakai:**
- Setiap kali mengikat raw socket sniffer atau transmitter ke interface monitor WiFi (`mon0`).
- Setiap kali mengirim frame 802.11 rakitan manual via `sendto()`.

---

### 5.4. Header #3: `<linux/if_ether.h>` — Konstanta Protokol Ethernet 🌐

**Fungsi:** Menyediakan konstanta universal untuk Ethernet dan protokol Layer 2, mendefinisikan panjang alamat fisik (MAC Address) serta kode-kode EtherType untuk protokol-protokol di atasnya.

**Analogi:** Ini seperti **buku pedoman standar ukuran surat dan prangko internasional** — menetapkan bahwa nomor identitas fisik kurir selalu tepat 6 angka (*6 bytes MAC*), serta kode stempel khusus untuk membedakan surat kilat, paket dokumen, atau kiriman diplomatik.

**Digunakan Untuk:**
- `ETH_P_ALL` — Nilai hexadecimal `0x0003`. Saat diserahkan ke `socket()` dengan fungsi `htons()`, ini memberi sinyal mutlak kepada kernel Linux: *"Berikan saya semua frame tanpa kecuali — Management, Control, Data, ARP, IPv4, IPv6!"*
- `ETH_ALEN` — Konstanta integer bernilai `6` (panjang byte alamat fisik IEEE 802 Media Access Control).
- `ETH_P_IP` (`0x0800`), `ETH_P_ARP` (`0x0806`), `ETH_P_PAE` (`0x888E` untuk 802.1X / WPA EAPOL Handshake).

**🔬 Deep Dive: Struktur & Makro Kunci di `<linux/if_ether.h>`:**
- `ETH_HLEN`: Panjang standar header Ethernet 802.3 (14 byte: 6 byte Dest + 6 byte Src + 2 byte EtherType).
- `ETH_DATA_LEN`: Batas payload Ethernet tradisional (1500 bytes MTU).
- `ETH_P_PAE`: Opcode `0x888E` yang menandakan paket autentikasi 4-Way Handshake EAPOL. Di Fase 4, kita memfilter paket menggunakan konstanta ini untuk menangkap kunci WPA!

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <linux/if_ether.h>  // Header #3: Definisi ETH_P_ALL (0x0003) dan ETH_ALEN (6)
#include <sys/socket.h>      // Dibutuhkan untuk fungsi socket()
#include <arpa/inet.h>       // Dibutuhkan untuk htons()
#include <string.h>          // Dibutuhkan untuk memset()
#include <stdio.h>           // Dibutuhkan untuk perror()

// 1. Buat socket raw di layer Data Link (AF_PACKET)
//    - Parameter 1: AF_PACKET (Akses tingkat fisik layer link)
//    - Parameter 2: SOCK_RAW (Akses paket tanpa abstraksi kernel)
//    - Parameter 3: htons(ETH_P_ALL) (Dengarkan seluruh lalu lintas paket radio)
int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
if (sock < 0) {
    perror("[-] Gagal membuat raw socket AF_PACKET");
    return -1;
}

// 2. Alokasikan buffer alamat MAC menggunakan ukuran standar ETH_ALEN (6 bytes)
//    Menghindari hardcoding angka '6' di seluruh source code
uint8_t target_mac[ETH_ALEN] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
uint8_t broadcast_mac[ETH_ALEN];
memset(broadcast_mac, 0xFF, ETH_ALEN); // FF:FF:FF:FF:FF:FF untuk alamat broadcast
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Lupa `htons()` pada `ETH_P_ALL`:** Pada prosesor x86/ARM (little-endian), nilai `ETH_P_ALL` adalah `0x0003`. Jika kamu lupa membungkusnya dengan `htons()`, nilai yang masuk ke kernel bisa terbalik menjadi `0x0300`, dan socket tidak akan menangkap paket apa pun!
- **Asumsi Bahwa Semua Frame Wi-Fi adalah Ethernet:** Ingat bahwa frame 802.11 asli di udara memiliki MAC header 24 bytes, bukan 14 bytes seperti Ethernet 802.3! Kartu monitor Wi-Fi menangkap frame 802.11 mentah, jadi jangan salah mengurai strukturnya.

**Kapan Dipakai:**
- Inisialisasi socket raw sniffer dan transmitter di Fase 1, 2, dan 5.
- Mendefinisikan ukuran array MAC address di seluruh struct.

---

### 5.5. Header #4: `<sys/ioctl.h>` — Remot Kontrol Hardware Jaringan 🎛️

**Fungsi:** Menyediakan fungsi antarmuka sistem `ioctl()` (*Input/Output Control*) untuk membaca dan mengubah konfigurasi perangkat keras kartu jaringan langsung dari userspace.

**Analogi:** Ini seperti **tombol kontrol dasbor mobil** — kamu bisa memeriksa kecepatan, menyalakan lampu, mengukur tekanan ban, atau melihat nomor sasis mobil.

**Digunakan Untuk:**
- `SIOCGIFINDEX` — Mengambil angka unik *Interface Index* (misal interface `mon0` = index 4).
- `SIOCGIFHWADDR` — Membaca MAC address asli kartu jaringan langsung dari EEPROM hardware.
- `SIOCGIFFLAGS` & `SIOCSIFFLAGS` — Menghidupkan (`UP`), mematikan (`DOWN`), atau mengatur mode `PROMISC`.

**🔬 Deep Dive: Struktur & Makro Kunci di `<sys/ioctl.h>`:**
- Prototype: `int ioctl(int fd, unsigned long request, ...);`
- Nilai kembalian: `0` jika sukses, `-1` jika gagal (dengan detail alasan tersimpan di variabel `errno`).
- `SIOCGIFHWADDR`: Mengisi `ifr.ifr_hwaddr.sa_data` dengan 6-byte alamat fisik kartu jaringan.

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <sys/ioctl.h>  // Header #4: Definisi ioctl() dan kode kontrol I/O sistem
#include <net/if.h>     // Dibutuhkan untuk struct ifreq
#include <string.h>     // Dibutuhkan untuk strncpy()
#include <stdio.h>      // Dibutuhkan untuk printf() dan perror()

struct ifreq ifr;
memset(&ifr, 0, sizeof(ifr));
strncpy(ifr.ifr_name, "wlan0mon", IFNAMSIZ - 1);

// Panggil kernel untuk membaca MAC Address asli interface
if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
    perror("[-] Gagal mengambil MAC address interface");
} else {
    // sa_data menyimpan byte-byte alamat MAC fisik
    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    printf("[+] MAC asli interface %s: %02X:%02X:%02X:%02X:%02X:%02X\n",
           ifr.ifr_name, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Izin Hak Akses (`root`):** Banyak perintah `ioctl` manipulasi jaringan memerlukan izin administratif (`CAP_NET_ADMIN` atau dijalankan dengan `sudo`). Jika dijalankan user biasa, `ioctl` akan gagal dengan error `EPERM` (Operation not permitted).

**Kapan Dipakai:**
- Fase inisialisasi raw socket sebelum `bind()`.
- Mendeteksi interface apakah dalam keadaan aktif atau mati.

---

### 5.6. Header #5: `<arpa/inet.h>` — Sang Penerjemah Endianness 🔄

**Fungsi:** Menyediakan fungsi konversi antara **Host Byte Order** (urutan penyimpanan byte di prosesor lokal — Little-Endian pada Intel/AMD x86 dan sebagian besar ARM) dengan **Network Byte Order** (Big-Endian standar protokol internet internasional).

**Analogi:** Ini seperti **penerjemah cara membaca angka**. Orang Arab menulis angka dari kanan ke kiri, sedangkan orang Indonesia menulis dari kiri ke kanan. Tanpa penerjemah, angka `50` bisa terbaca `05`!

**Digunakan Untuk:**
- `htons()` (*Host to Network Short*): Ubah nilai 16-bit dari CPU ke format jaringan.
- `ntohs()` (*Network to Host Short*): Ubah nilai 16-bit dari paket jaringan ke format CPU.
- `htonl()` & `ntohl()`: Versi 32-bit untuk alamat IP IPv4 atau sequence number.
- `inet_ntop()` & `inet_pton()`: Konversi string IP (misal `"192.168.1.1"`) ke biner 32-bit.

**🔬 Deep Dive: Operasi Biner Endianness:**
- Pada Little-Endian (CPU x86): Angka `0x8000` disimpan di RAM sebagai dua byte: `[0x00, 0x80]`.
- Di kabel / frekuensi udara: Protokol jaringan mengharuskan byte paling signifikan dikirim duluan (`[0x80, 0x00]`).
- Fungsi `htons(0x8000)` menukar posisi byte tersebut sehingga kartu Wi-Fi memancarkan `80 00` ke udara.

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <arpa/inet.h>  // Header #5: Definisi htons, ntohs, htonl, ntohl
#include <stdint.h>     // Dibutuhkan untuk uint16_t
#include <stdio.h>      // Dibutuhkan untuk printf()

// 1. Menulis Frame Control Beacon (0x8000) sebelum dikirim ke udara
uint16_t raw_fc = 0x8000;
uint16_t wire_fc = htons(raw_fc); // Memastikan byte di udara tersusun [0x80, 0x00]

// 2. Membaca Frame Control yang ditangkap oleh sniffer dari udara
uint16_t received_from_air = wire_fc;
uint16_t host_fc = ntohs(received_from_air); // Ubah kembali ke format CPU lokal

if (host_fc == 0x8000) {
    printf("[+] Paket ini adalah Beacon Frame!\n");
} else if (host_fc == 0x4000) {
    printf("[+] Paket ini adalah Probe Request!\n");
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Double Conversion (Konversi Ganda):** Memanggil `htons()` dua kali berturut-turut akan membalikkan byte dua kali, mengembalikan nilainya ke format yang salah!
- **Lupa `ntohs` saat Mengecek Tipe Frame:** Memeriksa `if (hdr->frame_control == 0x8000)` secara langsung tanpa `ntohs()` akan selalu menghasilkan FALSE di prosesor Little-Endian, karena di memori nilainya tersimpan terbalik (`0x0080`).

**Kapan Dipakai:**
- **WAJIB** setiap kali menulis atau membaca field multi-byte pada frame 802.11 (Frame Control, Duration, Sequence Control) dan header IP/TCP/UDP.

---

### 5.7. Header #6: `<sys/socket.h>` — Pondasi Komunikasi Jaringan 🔌

**Fungsi:** Menyediakan fungsi-fungsi dasar pembuatan saluran komunikasi (*socket*) di sistem operasi Unix/Linux.

**Analogi:** Ini seperti **mencolokkan kabel telepon** ke stopkontak dinding sebelum bisa mulai menelepon, serta mengangkat gagang telepon untuk mendengar suara penelepon.

**Digunakan Untuk:**
- `socket()` — Membuat endpoint komunikasi baru.
- `bind()` — Menautkan socket ke interface atau port fisik tertentu.
- `sendto()` — Menembakkan frame raw ke interface jaringan.
- `recvfrom()` — Menangkap frame yang terbang di udara melalui interface monitor.
- `setsockopt()` — Mengatur timeout transmisi atau mode kartu jaringan.

**🔬 Deep Dive: Parameter `recvfrom()` pada Raw Socket:**
- `ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);`
- Nilai kembalian: Jumlah byte yang berhasil ditangkap. Jika `< 0`, periksa `errno`.
- Jika socket diset timeout (via `SO_RCVTIMEO`), `recvfrom` akan kembali secara berkala tanpa memblokir thread selamanya.

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <sys/socket.h>  // Header #6: Definisi socket, bind, sendto, recvfrom, setsockopt
#include <stdio.h>       // Dibutuhkan untuk perror()
#include <stdint.h>      // Dibutuhkan untuk uint8_t
#include <sys/time.h>    // Dibutuhkan untuk struct timeval

// 1. Pasang timeout penerimaan 1 detik agar recvfrom tidak menggantung selamanya
struct timeval tv;
tv.tv_sec = 1;  // 1 detik
tv.tv_usec = 0;
setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

// 2. Menangkap paket di udara
uint8_t buffer[2048];
ssize_t bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);

if (bytes_received > 0) {
    printf("[+] Berhasil menangkap frame sebesar %zd bytes dari udara!\n", bytes_received);
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Buffer Terlalu Kecil:** Frame 802.11 di udara bisa mencapai ukuran hingga 2346 bytes jika ada enkripsi atau jumbo frame. Jika buffer `recvfrom` hanya 512 bytes, sisa paket akan terpotong (*truncated*) dan kamu kehilangan payload data!

**Kapan Dipakai:**
- Membuka socket raw sniffer dan transmitter di Fase 1 & 2.
- Mengirim frame deauthentication, beacon, dan probe response.

---

### 5.8. Header #7: `<unistd.h>` — Jendela System Calls POSIX ⏱️

**Fungsi:** Memberikan akses langsung ke fungsi standar sistem operasi Unix/POSIX, termasuk delay waktu presisi mikrodetik, kontrol file descriptor, dan operasi I/O tingkat rendah.

**Analogi:** Ini seperti **arloji dan sakelar utama ruang kontrol** — memberitahu program kapan harus istirahat sejenak dan kapan harus mematikan sakelar saat jam kerja berakhir.

**Digunakan Untuk:**
- `usleep(usec)` — Menidurkan proses selama sekian mikrodetik (1 detik = 1.000.000 mikrodetik).
- `sleep(sec)` — Menidurkan proses selama sekian detik.
- `close(fd)` — Menutup file descriptor socket agar tidak bocor (*socket resource leak*).

**🔬 Deep Dive: Mengapa `usleep` Sangat Penting untuk Wi-Fi?**
- Standar IEEE 802.11 menyatakan bahwa AP harus menyiarkan Beacon setiap 100 Time Units (1 TU = 1024 mikrodetik ≈ 102,4 ms).
- Fungsi `usleep(100000)` (tidur 100 milidetik) adalah jantung dari loop pemancar Beacon Rogue AP agar ritmenya menyerupai router Wi-Fi asli.

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <unistd.h>  // Header #7: Definisi usleep, sleep, close
#include <stdio.h>   // Dibutuhkan untuk printf()

// Siklus transmisi Beacon Rogue AP (10 frame per detik)
int counter = 0;
while (counter < 10) {
    // Siarkan frame beacon...
    printf("[*] Memancarkan Beacon #%d...\n", ++counter);
    
    // Tidur tepat 100.000 mikrodetik (100 ms) sebelum siaran berikutnya
    usleep(100000); 
}

// Selalu tutup socket setelah selesai untuk membersihkan file descriptor kernel
close(sock);
printf("[✓] Socket berhasil ditutup dengan bersih.\n");
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Tidur Tanpa Batas:** Menggunakan `sleep(1)` (1 detik) untuk Beacon akan membuat Rogue AP tidak stabil, karena client Wi-Fi menganggap sinyal AP putus jika tidak mendengar beacon selama >300ms!

**Kapan Dipakai:**
- Loop pemancar Beacon di Fase 2.
- Delay rotasi frekuensi pada Channel Hopper di Fase 10.
- Menutup socket pada proses shutdown program.

---

### 5.9. Header #8: `<string.h>` — Tukang Jahit Memori & String 🧵

**Fungsi:** Menyediakan fungsi-fungsi manipulasi blok memori biner dan string teks ASCII tingkat rendah.

**Analogi:** Ini seperti **gunting, meteran, dan lem kertas** di meja perakit surat. Kamu menyalin alamat fisik pengirim, menempelkan stiker nama SSID, dan membersihkan meja kerja sebelum mulai merakit amplop berikutnya.

**Digunakan Untuk:**
- `memcpy()` — Menyalin blok byte biner secara presisi (contoh: menyalin 6 byte MAC address ke header frame).
- `memset()` — Mereset seluruh isi buffer memori ke nilai nol atau nilai tertentu (seperti `0xFF` untuk broadcast).
- `memcmp()` — Membandingkan apakah dua blok memori sama persis (misal: mencocokkan BSSID paket dengan router target).
- `strncpy()` & `strlen()` — Membaca dan menyalin nama SSID dengan batas ukuran yang aman.

**🔬 Deep Dive: Mengapa `memcpy` Lebih Aman daripada Pointer Casting?**
- Pada frame 802.11, field-field sering kali berada di offset ganjil (tidak sejajar kelipatan 4 atau 8 byte di memori).
- Di arsitektur CPU tertentu (seperti ARM atau MIPS yang dipakai router OpenWrt), mengakses integer 32-bit di alamat memori ganjil dapat memicu crash hardware (*Bus Error / Alignment Fault*). Menggunakan `memcpy()` menjamin penyalinan aman byte-demi-byte tanpa mempedulikan alignment memori!

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <string.h>  // Header #8: Definisi memcpy, memset, memcmp, strncpy, strlen
#include <stdint.h>  // Dibutuhkan untuk uint8_t
#include <stdio.h>   // Dibutuhkan untuk printf()

uint8_t frame_buffer[256];

// 1. Kosongkan seluruh buffer frame (isi dengan angka 0x00)
memset(frame_buffer, 0, sizeof(frame_buffer));

// 2. Pasang alamat tujuan broadcast (FF:FF:FF:FF:FF:FF) di offset 4
memset(&frame_buffer[4], 0xFF, 6);

// 3. Salin alamat MAC AP kita (6 byte) ke offset 10 (Transmitter) dan offset 16 (BSSID)
uint8_t my_mac[6] = {0x00, 0xC0, 0xCA, 0x11, 0x22, 0x33};
memcpy(&frame_buffer[10], my_mac, 6);
memcpy(&frame_buffer[16], my_mac, 6);

// 4. Periksa apakah dua alamat MAC sama persis
if (memcmp(my_mac, &frame_buffer[16], 6) == 0) {
    printf("[+] BSSID cocok dengan MAC AP kita!\n");
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Menggunakan `strcpy` untuk Data Biner:** Jangan gunakan `strcpy()` pada paket 802.11! Fungsi `strcpy()` akan berhenti saat pertama kali menemukan byte `0x00` (null terminator), padahal paket biner jaringan dipenuhi byte `0x00`. Selalu gunakan `memcpy()` dengan panjang byte yang eksplisit!

**Kapan Dipakai:**
- Merakit seluruh struktur frame 802.11 dari byte pertama hingga terakhir.
- Memproses string nama SSID dan payload password.

---

### 5.10. Header #9: `<stdint.h>` — Presisi Ukuran Tipe Data C99 📏

**Fungsi:** Menyediakan tipe-tipe data integer dengan jaminan ukuran bit yang 100% pasti di SEMUA arsitektur komputer (32-bit, 64-bit, x86, ARM, MIPS).

**Analogi:** Ini seperti **cetakan kue berukuran milimeter standar internasional**. Di negara mana pun dan di oven mana pun, ukuran 1 byte tetap tepat 8 bit, tidak boleh melar atau menyusut!

**Digunakan Untuk:**
- `uint8_t` — Integer tanpa tanda 8-bit (tepat 1 byte) — sempurna untuk 1 oktet MAC address atau tag IE.
- `uint16_t` — Integer tanpa tanda 16-bit (tepat 2 bytes) — sempurna untuk Frame Control dan Sequence Control.
- `uint32_t` — Integer tanpa tanda 32-bit (tepat 4 bytes) — untuk FCS CRC32 atau IPv4 address.
- `uint64_t` — Integer tanpa tanda 64-bit (tepat 8 bytes) — untuk Timestamp microsecond WiFi.

**🔬 Deep Dive: Mengapa Menghindari Tipe Data Primitif C Seperti `int` dan `long`?**
- Di prosesor 32-bit, `long` berukuran 4 bytes. Namun di prosesor 64-bit Linux, `long` berukuran 8 bytes!
- Jika kamu mendefinisikan struct frame jaringan menggunakan `long`, ukuran frame kamu akan berubah-ubah tergantung komputer tempat program di-compile, merusak layout byte paket 802.11. Dengan `uint32_t`, ukurannya DIJAMIN selalu 4 bytes di seluruh dunia!

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <stdint.h>  // Header #9: Definisi uint8_t, uint16_t, uint32_t, uint64_t

// Definisi Header MAC 802.11 standar IEEE yang presisi tepat 24 bytes!
#pragma pack(push, 1) // Nonaktifkan padding compiler agar ukuran struktur tidak melar
typedef struct {
    uint16_t frame_control; // Tepat 2 bytes (16 bits)
    uint16_t duration;      // Tepat 2 bytes (16 bits)
    uint8_t  addr1[6];      // Tepat 6 bytes (Destination MAC)
    uint8_t  addr2[6];      // Tepat 6 bytes (Source / Transmitter MAC)
    uint8_t  addr3[6];      // Tepat 6 bytes (BSSID Filter)
    uint16_t seq_ctrl;      // Tepat 2 bytes (Fragment 4-bit + Sequence 12-bit)
} wifi_mgmt_hdr_t;
#pragma pack(pop)

// Total ukuran: 2 + 2 + 6 + 6 + 6 + 2 = Tepat 24 bytes!
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Lupa `#pragma pack(1)` atau `__attribute__((packed))`:** Compiler C modern cenderung meratakan data ke kelipatan 4 atau 8 byte untuk optimasi kecepatan memori CPU. Tanpa packed attribute, struct di atas bisa membengkak menjadi 28 byte karena compiler menambahkan padding kosong, yang akan membuat kartu Wi-Fi menolak paket tersebut!

**Kapan Dipakai:**
- Mendefinisikan seluruh `struct` protokol jaringan (802.11, Radiotap, IP, TCP, UDP, DHCP, DNS).

---

### 5.11. Header #10: `<pthread.h>` — Maestro Multi-threading Paralel 🤹

**Fungsi:** Menyediakan pustaka POSIX Threads untuk menjalankan beberapa fungsi secara independen dan bersamaan dalam satu ruang alamat memori proses.

**Analogi:** Ini seperti **mempekerjakan dua koki di satu dapur restoran**: Koki 1 bertugas terus memancarkan aroma kue (*Beacon thread*), sementara Koki 2 mencatat pelanggan yang datang memesan (*Sniffer thread*). Keduanya bekerja bersama tanpa saling menghentikan.

**Digunakan Untuk:**
- `pthread_create()` — Membuat dan meluncurkan thread baru di latar belakang.
- `pthread_join()` — Menunggu sebuah thread selesai bekerja sebelum program keluar.
- `pthread_mutex_t` — Kunci pintu pengaman data bersama (*mutual exclusion*) agar tidak terjadi *data race* saat dua thread mengakses socket atau variabel yang sama.

**🔬 Deep Dive: Mengamankan Global Socket dengan Mutex (Pelajaran Audit v15.3):**
- Pada proyek WiFi Pineapple tingkat lanjut, jika thread Beacon dan thread Deauth menggunakan socket transmisi yang sama secara serentak tanpa koordinasi, kernel Linux dapat mengalami race condition pada buffer socket.
- Kita menggunakan `pthread_mutex_lock(&g_tx_mutex)` dan `pthread_mutex_unlock(&g_tx_mutex)` untuk memastikan hanya satu thread yang memanggil `sendto()` pada satu waktu!

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <pthread.h> // Header #10: Definisi pthread_create, pthread_join, pthread_mutex_t
#include <stdio.h>   // Dibutuhkan untuk printf() dan perror()
#include <unistd.h>  // Dibutuhkan untuk sleep()

// Mutex global untuk melindungi sumber daya bersama
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

// Fungsi yang akan dijalankan oleh thread latar belakang
void *worker_thread(void *arg) {
    printf("[*] Thread latar belakang mulai bekerja...\n");
    for (int i = 0; i < 3; i++) {
        pthread_mutex_lock(&g_lock);   // Kunci akses data
        printf("[Worker] Melakukan pekerjaan #%d...\n", i + 1);
        pthread_mutex_unlock(&g_lock); // Buka kunci akses
        sleep(1);
    }
    return NULL;
}

int main(void) {
    pthread_t tid;
    // Buat thread baru
    if (pthread_create(&tid, NULL, worker_thread, NULL) != 0) {
        perror("[-] Gagal membuat pthread");
        return 1;
    }
    
    // Tunggu thread selesai sebelum fungsi main() ditutup
    pthread_join(tid, NULL);
    printf("[✓] Seluruh thread telah selesai dengan tertib.\n");
    return 0;
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Passing Pointer Variabel Lokal ke Thread:** Jika kamu mengirimkan pointer variabel lokal stack (`&var`) ke fungsi thread, lalu fungsi pemanggilnya langsung selesai (*return*), memori stack tersebut akan hilang dan thread akan membaca data sampah (*dangling pointer* / undefined behavior). Selalu gunakan variabel global terisolasi atau alokasikan via `malloc()`.

**Kapan Dipakai:**
- Memisahkan tugas transmisi rutin (Beacon & Channel Hopping) dengan tugas penerimaan sniffer yang intensif.

---

### 5.12. Header #11: `<signal.h>` — Rem Darurat Graceful Shutdown 🚨

**Fungsi:** Menangkap interupsi sinyal dari sistem operasi atau pengguna (seperti saat pengguna menekan tombol keyboard `Ctrl+C`).

**Analogi:** Ini seperti **tombol STOP darurat di eskalator**. Saat tombol ditekan, eskalator tidak meledak atau mati mendadak, tapi melambat dan berhenti dengan aman tanpa menjatuhkan penumpang.

**Digunakan Untuk:**
- `signal(SIGINT, handler)` — Menangkap penekanan tombol `Ctrl+C` dari terminal.
- `SIGTERM` — Menangkap perintah penghentian proses dari sistem (`kill`).
- `volatile sig_atomic_t` — Tipe data aman untuk flag penghentian antar sinyal dan loop utama.

**🔬 Deep Dive: Apa itu `volatile sig_atomic_t`?**
- `sig_atomic_t`: Tipe data integer yang dijamin oleh CPU dapat dibaca dan ditulis dalam SATU siklus instruksi tunggal (atomik), sehingga tidak akan terpotong di tengah jalan jika sinyal tiba-tiba masuk.
- `volatile`: Memberitahu compiler C untuk TIDAK mengoptimasi variabel ini ke register CPU, melainkan selalu membaca langsung dari RAM fisik setiap kali loop berputar.

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <signal.h>  // Header #11: Definisi signal, SIGINT, SIGTERM, sig_atomic_t
#include <stdio.h>   // Dibutuhkan untuk printf()
#include <unistd.h>  // Dibutuhkan untuk sleep()

// Flag global sinyal: 1 = berjalan, 0 = berhenti
static volatile sig_atomic_t g_keep_running = 1;

// Handler sinyal: fungsi ini dipanggil otomatis oleh OS saat Ctrl+C ditekan
static void handle_interrupt(int sig) {
    (void)sig; // Hindari compiler warning unused parameter
    g_keep_running = 0; // Beritahu semua loop untuk berhenti dengan sopan
}

int main(void) {
    // Daftarkan fungsi penanganan sinyal SIGINT (Ctrl+C)
    signal(SIGINT, handle_interrupt);
    signal(SIGTERM, handle_interrupt);

    printf("[+] Program aktif. Tekan Ctrl+C untuk keluar dengan aman...\n");
    while (g_keep_running) {
        // Lakukan pekerjaan sniffer / transmisi di sini...
        sleep(1);
    }

    printf("\n[*] Menerima sinyal keluar. Membersihkan socket dan memori...\n");
    // Lakukan pembersihan socket di sini
    printf("[✓] Selesai dengan aman!\n");
    return 0;
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Memanggil Fungsi Non-Reentrant di Signal Handler:** Jangan pernah memanggil fungsi kompleks seperti `printf()`, `malloc()`, atau `free()` di dalam fungsi signal handler! Fungsi-fungsi tersebut menggunakan mutex internal dan dapat memicu *Deadlock* (program hang selamanya). Cukup ubah nilai flag `g_keep_running = 0` dan biarkan thread utama yang mencetak teks pembersihan.

**Kapan Dipakai:**
- Menjamin socket tertutup dan kartu WiFi dikembalikan ke kondisi normal saat program dihentikan.

---

### 5.13. Header #12: `<stdio.h>` — Layar Dashboard & Jurnal Catatan 🖥️

**Fungsi:** Menyediakan fungsi input/output teks standar untuk terminal layar, pembacaan keyboard, dan penulisan berkas log.

**Analogi:** Ini seperti **pengeras suara pemanggil dan buku agenda** detektif untuk melaporkan apa yang sedang terjadi kepada pengawas.

**Digunakan Untuk:**
- `printf()` — Mencetak radar status dan menu CLI interaktif ke layar terminal.
- `snprintf()` — Menyusun string teks dengan jaminan batas ukuran agar aman dari *buffer overflow*.
- `perror()` — Menampilkan pesan error sistem operasi secara otomatis berdasarkan kode `errno`.
- `fopen()`, `fprintf()`, `fflush()`, `fclose()` — Menulis berkas log hasil audit atau kredensial yang tertangkap.

**🔬 Deep Dive: Mengapa Wajib `snprintf` Bukan `sprintf`?**
- Fungsi lama `sprintf(buf, "%s", input)` tidak memeriksa apakah teks melebihi ukuran `buf`. Hacker sering memanfaatkan celah ini untuk merusak susunan stack dan mengambil alih program (*stack smashing*).
- Fungsi `snprintf(buf, sizeof(buf), "%s", input)` menjamin penulisan TIDAK AKAN PERNAH melebihi ukuran buffer yang dialokasikan, dan selalu menyisipkan null-terminator `\0` di akhir string.

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <stdio.h>   // Header #12: Definisi printf, snprintf, perror, fopen, fprintf

char log_buffer[128];
char target_ssid[] = "CafeWiFi_Gratis";
int channel = 6;

// Format string dengan batas keamanan eksplisit sizeof(log_buffer)
snprintf(log_buffer, sizeof(log_buffer), "[+] Rogue AP aktif: SSID '%s' di Channel %d",
         target_ssid, channel);

// Cetak ke layar terminal
printf("%s\n", log_buffer);
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Lupa `fflush()` pada File Log:** Sistem operasi menahan penulisan file di memory cache buffer. Jika programmu tiba-tiba dimatikan paksa (misal dicabut dayanya), data kredensial di cache bisa hilang sebelum tertulis ke disk. Selalu panggil `fflush(file_ptr)` setelah mencatat kredensial penting!

**Kapan Dipakai:**
- Menampilkan visualisasi antarmuka CLI dan hasil sniffer secara realtime.
- Menyimpan laporan audit keamanan ke disk.

---

### 5.14. Header #13: `<stdlib.h>` — Pisau Serbaguna Standard Library 🛠️

**Fungsi:** Menyediakan fungsi-fungsi esensial sistem operasi, manajemen alokasi memori dinamis di heap, generator angka acak, konversi string ke angka, dan eksekusi perintah shell.

**Analogi:** Ini seperti **pisau lipat saku multifungsi Swiss Army** — ada obeng, pembuka botol, gergaji kecil, dan penggaris cadangan dalam satu genggaman.

**Digunakan Untuk:**
- `system()` — Menjalankan utilitas Linux eksternal (seperti `ip link set dev mon0 up` atau `iw dev mon0 set channel`).
- `malloc()` & `free()` — Mengalokasikan dan membebaskan memori dinamis di heap untuk daftar client atau database handshake.
- `atoi()` & `strtol()` — Mengubah string input argumen terminal (misal `"6"`) menjadi integer channel (`6`).
- `exit()` — Mengakhiri program secara langsung dengan kode status (0 = sukses, 1 = gagal).

**🔬 Deep Dive: Mengamankan Pemanggilan `system()`:**
- Perintah shell via `system()` sangat praktis untuk mengubah channel kartu Wi-Fi (`iw dev mon0 set channel 11`).
- Namun, jika string perintah disusun dari input pengguna luar tanpa validasi, penyerang bisa menyisipkan karakter `; rm -rf /`. Selalu validasi bahwa argumen (seperti nomor channel) berada dalam rentang angka legal (1 s/d 14)!

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <stdlib.h>  // Header #13: Definisi system, malloc, free, atoi, exit
#include <stdio.h>   // Dibutuhkan untuk printf()

// 1. Konversi argumen CLI nomor channel (string ke integer)
char *channel_arg = "11";
int ch = atoi(channel_arg);

// 2. Validasi rentang channel legal Wi-Fi 2.4 GHz
if (ch < 1 || ch > 14) {
    fprintf(stderr, "[-] Nomor channel tidak valid! Pilih antara 1 - 14.\n");
    exit(1); // Keluar dengan kode error
}

// 3. Eksekusi utilitas sistem Linux untuk mengatur frekuensi radio kartu Wi-Fi
char cmd[64];
snprintf(cmd, sizeof(cmd), "iw dev mon0 set channel %d", ch);
int status = system(cmd);
if (status == 0) {
    printf("[+] Kartu Wi-Fi berhasil dipindahkan ke Channel %d\n", ch);
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Memory Leak (Lupa `free`):** Setiap kali memanggil `malloc()`, wajib ada pasangan `free()` saat data tidak lagi digunakan. Jika program sniffer berjalan 24 jam dan terus mengalokasikan memori tanpa membebaskannya, RAM komputer akan habis dan program akan di-kill oleh Linux Out-Of-Memory (OOM) Killer.

**Kapan Dipakai:**
- Menyiapkan mode monitor kartu WiFi melalui script `ip`/`iw`.
- Alokasi dinamis database client dan modul di Fase 4 dan Fase 11.

---

### 5.15. Header #14: `<time.h>` — Pencatat Waktu & Stempel Sejarah ⏰

**Fungsi:** Menyediakan fungsi penanggalan kalender, pengukuran interval waktu, dan pembacaan stempel waktu (*timestamp*) sistem.

**Analogi:** Ini seperti **stempel tanggal dan jam otomatis** di loket pengadilan yang menandai detik persis kapan sebuah bukti perkara masuk.

**Digunakan Untuk:**
- `time()` — Mendapatkan waktu saat ini dalam format Unix epoch (jumlah detik sejak 1 Januari 1970).
- `localtime()` & `strftime()` — Memformat detik epoch menjadi string teks jam dan tanggal yang mudah dibaca manusia.
- Menghitung durasi *timeout* deauthentication atau usia *client last seen*.

**🔬 Deep Dive: Timestamp Mikrodetik pada Beacon Wi-Fi:**
- Pada frame Beacon 802.11, terdapat field `timestamp` berukuran 8 byte (64-bit integer) yang merepresentasikan waktu jam internal AP dalam satuan mikrodetik (*microsecond counter*).
- Kita menghitungnya menggunakan: `(uint64_t)time(NULL) * 1000000ULL`. Nilai ini digunakan oleh client Wi-Fi untuk menyinkronkan jam internalnya dengan Access Point (*Timing Synchronization Function / TSF*).

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <time.h>    // Header #14: Definisi time, localtime, strftime, time_t
#include <stdio.h>   // Dibutuhkan untuk printf()

// 1. Ambil waktu epoch sistem saat ini
time_t now = time(NULL);

// 2. Format menjadi string teks jam manusia (HH:MM:SS)
char time_str[32];
strftime(time_str, sizeof(time_str), "%H:%M:%S", localtime(&now));

// 3. Tampilkan catatan insiden keamanan bersama stempel waktu
printf("[%s] 🚨 TERDETEKSI 4-Way Handshake WPA dari Client!\n", time_str);
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **`localtime` Not Thread-Safe:** Fungsi standar `localtime(&now)` mengembalikan pointer ke struktur internal statis. Jika dua thread memanggilnya secara bersamaan, nilainya bisa tertimpa. Pada kode multithreaded berkecepatan tinggi, gunakan versi POSIX reentrant: `localtime_r(&now, &result_tm)`.

**Kapan Dipakai:**
- Menghasilkan nilai microsecond timestamp di header Beacon.
- Modul Logger, pencatatan waktu paket ditangkap, dan penentuan waktu timeout client.

---

### 5.16. Header #15: `<errno.h>` — Sang Dokter Diagnosa Masalah 🩺

**Fungsi:** Menyediakan variabel global `errno` (*error number*) dan kode-kode konstanta kesalahan standar Linux jika sebuah *system call* (seperti `socket`, `bind`, `sendto`, `recvfrom`) mengalami kegagalan.

**Analogi:** Ini seperti **kode hasil rontgen laboratorium** sang dokter yang menjelaskan secara medis mengapa sebuah operasi gagal (apakah karena kabel putus, izin ditolak, memori habis, atau saluran sibuk).

**Digunakan Untuk:**
- `errno == EINTR` — Mengetahui apakah system call terputus sementara karena interupsi sinyal (seperti saat pengguna menekan `Ctrl+C`).
- `errno == EAGAIN` atau `EWOULDBLOCK` — Socket non-blocking belum memiliki data baru untuk dibaca (bukan error fatal).
- `strerror(errno)` — Menerjemahkan kode angka error menjadi kalimat bahasa Inggris yang jelas untuk ditampilkan ke pengguna.

**🔬 Deep Dive: Menangani `EINTR` pada Socket Transmisi & Sniffer:**
- Ketika proses menangkap sinyal `SIGINT` (Ctrl+C), panggilan `recvfrom()` atau `sendto()` yang sedang menunggu akan langsung dibatalkan oleh kernel dan mengembalikan nilai `-1`, dengan `errno = EINTR` (Interrupted system call).
- Program yang tangguh HARUS memeriksa kondisi ini: jika `errno == EINTR`, jangan tampilkan pesan error panik, melainkan keluar dari loop secara tertib!

**Contoh Kode (dengan Komentar Lengkap):**
```c
#include <errno.h>   // Header #15: Definisi variabel errno, EINTR, EAGAIN
#include <string.h>  // Dibutuhkan untuk strerror()
#include <stdio.h>   // Dibutuhkan untuk printf()
#include <sys/socket.h> // Dibutuhkan untuk sendto()

int result = sendto(sock, frame_buf, frame_len, 0, NULL, 0);
if (result < 0) {
    if (errno == EINTR) {
        // Operasi terputus karena sinyal interupsi Ctrl+C, bukan error kartu jaringan
        printf("[*] Transmisi terputus oleh sinyal pengguna.\n");
    } else if (errno == EPERM) {
        printf("[-] Akses ditolak! Jalankan program menggunakan sudo.\n");
    } else {
        // Tampilkan diagnosa kesalahan lengkap dari kernel
        printf("[-] Gagal mengirim frame: %s (Kode errno: %d)\n", strerror(errno), errno);
    }
}
```

**⚠️ Kesalahan Umum & Gotcha Programmer C:**
- **Mengecek `errno` Tanpa Cek Return Code:** Nilai `errno` tidak di-reset ke 0 oleh system call yang sukses. Karena itu, kamu HANYA boleh memeriksa `errno` JIKA fungsi mengembalikan nilai error (biasanya `-1` atau `NULL`). Memeriksa `errno` setelah fungsi yang sukses bisa menampilkan pesan error palsu dari operasi sebelumnya!

**Kapan Dipakai:**
- Menangani kondisi batas (*edge cases*), timeout, dan error handling socket tingkat lanjut di seluruh fase.

---

## 6. Bab 5: Source Code Lengkap — Rogue AP dengan Semua Header 💻

Berikut adalah implementasi utuh **`rogue_ap_full.c`** yang memadukan ke-15 header Linux di atas menjadi satu program C mandiri yang siap di-compile dan dijalankan di Linux. 

### 6.1. Diagram Alir Sistem `rogue_ap_full.c` 🌊

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                       ARSITEKTUR MULTI-THREADING ROGUE AP                   │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│                     ┌───────────────────────────────┐                       │
│                     │       main() Inisialisasi     │                       │
│                     │  - Parse Argumen CLI          │                       │
│                     │  - Daftarkan Sinyal Ctrl+C    │                       │
│                     │  - Buka Raw Socket AF_PACKET  │                       │
│                     └───────────────┬───────────────┘                       │
│                                     │                                       │
│                    ┌────────────────┴────────────────┐                      │
│                    ▼                                 ▼                      │
│      ┌───────────────────────────┐     ┌───────────────────────────┐        │
│      │  Thread 1: Beacon Thread  │     │  Thread 2: Main Sniffer   │        │
│      │  (Pemancar Background)    │     │  (Penerima Loop)          │        │
│      ├───────────────────────────┤     ├───────────────────────────┤        │
│      │ • Rakit Beacon Frame:     │     │ • Tangkap frame mentah    │        │
│      │   - MAC Header (24 byte)  │     │   via recvfrom()          │        │
│      │   - Fixed Param (12 byte) │     │ • Filter Opcode:          │        │
│      │   - Tag 0: SSID           │     │   ntohs(fc) == 0x4000     │        │
│      │   - Tag 1: Rates          │     │ • Ekstrak Target SSID     │        │
│      │   - Tag 3: Channel        │     │ • Cetak radar ke terminal │        │
│      │ • sendto() ke udara       │     │ • Ulangi sampai Ctrl+C    │        │
│      │ • usleep(100.000) (100ms) │     └─────────────┬─────────────┘        │
│      └─────────────┬─────────────┘                   │                      │
│                    │                                 │                      │
│                    └────────────────┬────────────────┘                      │
│                                     ▼                                       │
│                     ┌───────────────────────────────┐                       │
│                     │   Graceful Cleanup & Exit     │                       │
│                     │  - pthread_join(beacon_thread)│                       │
│                     │  - close(raw_sock)            │                       │
│                     │  - Pulihkan terminal          │                       │
│                     └───────────────────────────────┘                       │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

### 6.2. Source Code Lengkap Beranotasi: `rogue_ap_full.c`

Setiap baris kode di bawah ini telah dilengkapi komentar mendalam dalam Bahasa Indonesia agar mudah dipelajari oleh pemula maupun pengembang tingkat lanjut:

```c
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
```

---

## 7. Bab 6: Ringkasan 15 Header & Kapan Memakainya 📋

Berikut adalah tabel contekan (*cheat sheet*) 15 header Linux yang dapat kamu simpan sebagai referensi cepat:

| No | Header C / Linux | Peran Utama | Kapan Wajib Dipakai? |
|:---:|:---|:---|:---|
| 1 | `<net/if.h>` | Definisi nama & index kartu jaringan (`struct ifreq`, `IFNAMSIZ`) | Saat mengonfigurasi atau mengikat socket ke interface seperti `mon0` |
| 2 | `<linux/if_packet.h>` | Alamat fisik Layer 2 Data Link (`struct sockaddr_ll`, `AF_PACKET`) | **Wajib** untuk mengirim dan menerima frame raw 802.11 tanpa IP stack |
| 3 | `<linux/if_ether.h>` | Konstanta protokol ethernet (`ETH_P_ALL`, `ETH_ALEN`) | Saat membuka socket sniffer yang menangkap semua paket udara |
| 4 | `<sys/ioctl.h>` | Kontrol perangkat keras via kernel (`ioctl`, `SIOCGIFINDEX`) | Saat membaca nomor identitas kartu jaringan atau status interface |
| 5 | `<arpa/inet.h>` | Konversi Endianness CPU ↔ Jaringan (`htons`, `ntohs`) | **Wajib** setiap kali menulis Frame Control (`0x8000`) atau Sequence |
| 6 | `<sys/socket.h>` | Panggilan socket standar (`socket`, `bind`, `sendto`, `recvfrom`) | Pondasi komunikasi pembuatan soket data |
| 7 | `<unistd.h>` | System calls POSIX (`usleep`, `close`, `sleep`) | Mengatur delay interval beacon (100 ms) dan menutup file descriptor |
| 8 | `<string.h>` | Manipulasi buffer memori biner (`memcpy`, `memset`, `strncpy`) | Merakit frame byte-demi-byte, menyalin MAC, dan membersihkan memori |
| 9 | `<stdint.h>` | Tipe data integer pasti (`uint8_t`, `uint16_t`, `uint32_t`) | Menjamin struct frame 802.11 memiliki ukuran byte yang 100% presisi |
| 10 | `<pthread.h>` | Multi-threading POSIX (`pthread_create`, `pthread_join`) | Membagi tugas Beacon broadcast dan sniffer ke thread berbeda |
| 11 | `<signal.h>` | Penanganan sinyal OS (`signal`, `SIGINT`, `sig_atomic_t`) | Graceful exit saat pengguna menekan `Ctrl+C` agar tidak leak |
| 12 | `<stdio.h>` | I/O layar terminal & log (`printf`, `snprintf`, `perror`) | Menampilkan status dashboard, deteksi client, dan pelaporan error |
| 13 | `<stdlib.h>` | Eksekusi shell & utilitas umum (`system`, `atoi`, `exit`) | Mengatur mode monitor kartu WiFi melalui script `ip`/`iw` |
| 14 | `<time.h>` | Penanggalan & stempel waktu (`time`, `localtime`, `strftime`) | Memberi catatan jam persis saat client terdeteksi oleh radar |
| 15 | `<errno.h>` | Pelacakan kode error kernel (`errno`, `EINTR`, `strerror`) | Menangani socket terputus atau timeout secara elegan |

---

## 8. Bab 7: Jembatan Menuju WiFi Pineapple & Header Custom 🍍

Sekarang kamu sudah memahami bahwa:
1. **802.11 adalah sebuah bahasa**: Setiap aksi memiliki kode opcode biner tertentu (seperti `0x8000` untuk Beacon, `0x4000` untuk Probe Request, dan `0xC000` untuk Deauthentication).
2. **15 Header Linux adalah kamus dan perkakasmu**: Melalui raw socket layer 2, kamu memegang kendali penuh atas paket apa pun yang terbang di udara.

Lalu, bagaimana kita merancang **Header Custom Buatan Sendiri** untuk proyek skala enterprise sebesar WiFi Pineapple?

### 8.1. Mengapa Kita Membuat Header Custom (`.h`) Sendiri? 🏗️

Dalam software enterprise, kita tidak meletakkan 3.000 baris kode dalam satu file `main.c`. Kita membaginya menjadi modul-modul independen:
- Setiap modul memiliki satu file antarmuka **`.h`** (kontrak publik)
- Dan satu atau lebih file implementasi **`.c`** (dapur privat)

### 8.2. Pola Penulisan Header C Bersih & Standar Industri 💎

Setiap file header custom yang kita bangun di Dokumen 3 mengikuti 3 kaidah emas perancangan C:

1. **Include Guard (`#ifndef / #define / #endif`):**  
   Mencegah error *redefinition of struct* jika sebuah file header di-include beberapa kali oleh modul berbeda.
   ```c
   #ifndef MODULE_NAME_H
   #define MODULE_NAME_H
   
   // Konten header di sini...
   
   #endif // MODULE_NAME_H
   ```

2. **Memaksa Ukuran Memori Tanpa Padding (`__attribute__((packed))`):**  
   Wajib digunakan pada semua struktur frame jaringan agar compiler tidak menyisipkan byte padding siluman.

3. **Deklarasi vs Definisi:**  
   File `.h` **HANYA** mendeklarasikan tipe data, konstanta, dan prototype fungsi (`int deauth_start(void);`). File `.h` **TIDAK BOLEH** mengalokasikan variabel global tanpa kata kunci `extern`, agar tidak terjadi error kompilasi *multiple definition*.

### 8.3. Peta 15 Header Custom Proyek di Dokumen 3 🗺️

Di **[Dokumen 3: WiFi Pineapple — Dari Nol sampai Legendaris](03_wifi_pineapple.md)**, kamu akan membedah dan menggunakan 15 header custom ini:

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                 PETA 15 HEADER CUSTOM PROYEK WIFI PINEAPPLE                 │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   [Fase 1: Fondasi]                                                         │
│   ├── config.h             -> Konstanta global, struct frame, master config │
│   ├── utils/logger.h       -> Logging multi-level thread-safe (DEBUG..ALERT)│
│   └── utils/utils.h        -> Abstraksi socket, parser MAC, helper waktu    │
│                                                                             │
│   [Fase 2 - 5: Radio & Serangan Nirkabel]                                   │
│   ├── ap_manager/ap_manager.h -> API siklus hidup Rogue AP & beacon loop    │
│   ├── attack/karma.h       -> Respons probe otomatis berbasis whitelist/blk │
│   ├── attack/deauth.h      -> Injeksi frame pemutus koneksi burst           │
│   ├── attack/flood.h       -> Pembanjir beacon multi-SSID acak              │
│   ├── capture/capture.h    -> Perekam 4-Way Handshake WPA & Client DB       │
│   └── sniffer/sniffer.h    -> Dispatcher sentral penyaring frame radio      │
│                                                                             │
│   [Fase 6 - 9: Layanan Jaringan & MITM]                                     │
│   ├── network/dhcp.h       -> Server DHCP mini peminjam IP korban           │
│   ├── network/dns.h        -> Server DNS Spoofer pembelok rute domain       │
│   ├── mitm/proxy.h         -> Proxy transparan HTTP & credential sniffer    │
│   └── mitm/evil_portal.h   -> Web server captive portal phishing            │
│                                                                             │
│   [Fase 10 - 12: Spektrum, Modul, & Antarmuka]                              │
│   ├── attack/channel_hopper.h -> Rotator frekuensi kanal radio otomatis    │
│   ├── attack/wps.h         -> Mesin penguji keamanan PIN WPS router         │
│   └── modules/modules.h    -> Sistem arsitektur plugin modular dinamis      │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 9. Bab 8: Kurikulum Belajar 4 Tahap: Menguasai Bytecode 802.11 & Linux Raw Socket Programming 🎓

Setelah mempelajari konsep dasar bytecode dan membedah 15 header bawaan Linux, bagaimana langkah sistematis untuk menguasainya dari nol hingga mahir memanipulasi frame biner secara mandiri? 

Kurikulum ini dirancang khusus untuk mengasah kompetensi **Low-Level Systems Programming**, **Manipulasi Memori Biner**, dan **Linux Network Kernel Subsystem**:

```
┌─────────────────────────────────────────────────────────────────────────────┐
│       ROADMAP KURIKULUM 4 TAHAP: BYTECODE 802.11 & LINUX RAW SOCKET         │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  [Tahap 1: Representasi Bit/Byte & Memori C]                                │
│   └── Memahami Hex Dump, Bit Shifting, Endianness htons(), #pragma pack     │
│       │                                                                     │
│       ▼                                                                     │
│  [Tahap 2: 15 Header Linux & Kernel System Calls]                           │
│   └── AF_PACKET, ioctl SIOCGIFINDEX, bind(), pthread_mutex_t, sigaction     │
│       │                                                                     │
│       ▼                                                                     │
│  [Tahap 3: Konstruksi Manual Frame 802.11 & TLV]                            │
│   └── Merakit 24B MAC Header, Fixed Parameters, & Tagged Elements (SSID/DS) │
│       │                                                                     │
│       ▼                                                                     │
│  [Tahap 4: Program Dual-Thread Standalone (rogue_ap_full.c)]                │
│   └── Thread Beacon TX + Thread Sniffer RX, Mutex Transmisi, Signal Safe   │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

### 🧠 Tahap 1: Representasi Bit/Byte, Hex Dump, Endianness & Alignment Memori C
- **Fokus Utama:** Belajar membaca dan memanipulasi data mentah biner di dalam memori RAM komputer.
- **Topik Pembelajaran:**
  1. **Konsep Hex Dump & Biner:** Membaca aliran byte jaringan (`0x80`, `0x00`, `0x00`, `0x00`) dan membedahnya menjadi field-field protokol menggunakan operasi bit (*bitmasking* `&` dan *bit shifting* `>>`).
  2. **Endianness (Urutan Byte):** Memahami perbedaan antara *Little-Endian* (urutan penyimpanan byte prosesor Intel/AMD x86 di mana *Least Significant Byte* disimpan di alamat memori terendah) dengan *Big-Endian* (*Network Byte Order* di mana *Most Significant Byte* dikirim terlebih dahulu di kabel/udara). Menguasai fungsi konverter standar: `htons()`, `ntohs()`, `htonl()`, dan `ntohl()`.
  3. **Struktur Data Terpadatkan (`#pragma pack`):** Memahami mengapa secara default compiler C menambahkan *padding bytes* (misal 2 atau 4 byte kosong) untuk mensejajarkan memori (alignment), dan mengapa pada pemrograman jaringan kita **wajib** menggunakan `#pragma pack(push, 1)` agar ukuran struct di C tepat 100% cocok dengan panjang frame fisik di kartu Wi-Fi.
- **Tolak Ukur Keberhasilan (Milestone):** Mampu mendeklarasikan struct paket kustom di C, mengisinya dengan data hex, dan mencetak representasi heksadesimalnya dengan benar tanpa ada byte geser atau byte korup.

---

### 🐧 Tahap 2: Penguasaan 15 Header Inti Linux & Kernel System Calls
- **Fokus Utama:** Menguasai API bawaan sistem operasi Linux untuk mengendalikan kartu jaringan langsung tanpa melewati stack TCP/IP kernel.
- **Topik Pembelajaran:**
  1. **Layer 2 Raw Socket:** Membuka soket mentah dengan `socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))` dari `<linux/if_packet.h>` dan `<linux/if_ether.h>`.
  2. **Identifikasi Antarmuka Jaringan:** Menggunakan `ioctl(fd, SIOCGIFINDEX, &ifr)` dari `<sys/ioctl.h>` dan `<net/if.h>` untuk menerjemahkan nama antarmuka teks (misalnya `"wlan0mon"`) menjadi indeks antarmuka internal kernel Linux (`ifindex`).
  3. **Pengikatan & Transmisi:** Mengisi `struct sockaddr_ll` dan melakukan `bind()` ke kartu nirkabel, serta memanggil `sendto()` dan `recvfrom()` dari `<sys/socket.h>`.
  4. **Stabilitas & Keandalan POSIX:** Mengimplementasikan mutex thread-safe `<pthread.h>` untuk mengunci akses pemancar, memasang handler sinyal `<signal.h>` (`SIGINT` / `SIGTERM`) dengan flag `volatile sig_atomic_t` untuk keluar secara tertib (*graceful exit*), dan mendeteksi interupsi sinyal dengan `<errno.h>` (`EINTR`, `EAGAIN`).
- **Tolak Ukur Keberhasilan (Milestone):** Mampu menulis program C pendek 50 baris yang berhasil membuka interface dalam mode monitor dan menangkap paket nirkabel mentah tanpa crash.

---

### 🧩 Tahap 3: Konstruksi Manual Frame 802.11 & TLV (Tag-Length-Value)
- **Fokus Utama:** Merakit paket nirkabel IEEE 802.11 byte demi byte langsung di dalam array buffer memori.
- **Topik Pembelajaran:**
  1. **Anatomi 24-Byte MAC Header:** Mengisi Frame Control (`0x8000` untuk Beacon), Duration/ID (`0x0000`), Address 1 (Destination/Broadcast `FF:FF:FF:FF:FF:FF`), Address 2 (Source/BSSID AP), Address 3 (BSSID AP), dan Sequence Control.
  2. **12-Byte Fixed Parameters:** Memahami nilai Timestamp 64-bit hardware, Beacon Interval (100 TU / milidetik), dan Capability Information (`0x0411` untuk ESS + Short Slot Time).
  3. **Mekanisme Tag-Length-Value (TLV):** Memahami bahwa ekstensi frame 802.11 disusun dengan format dinamis:
     - Tag ID 1 byte (misal: Tag 0 = SSID, Tag 1 = Supported Rates, Tag 3 = DS Parameter Channel).
     - Length 1 byte (panjang data muatan).
     - Value byte array (data sebenarnya, seperti `"Free_Public_WiFi"`).
  4. **Perhitungan Offset Memori:** Menggunakan `memcpy()` dari `<string.h>` untuk menyambung header, fixed params, dan deretan tag TLV ke dalam satu buffer transmisi berurutan tanpa buffer overflow.
- **Tolak Ukur Keberhasilan (Milestone):** Mampu merakit buffer frame Beacon biner lengkap secara manual di memori dan memverifikasi kelayakan strukturnya dengan Wireshark.

---

### 🚀 Tahap 4: Merakit Program Dual-Thread Raw Socket C Mandiri (`rogue_ap_full.c`)
- **Fokus Utama:** Mengintegrasikan seluruh teori bytecode dan 15 header Linux ke dalam satu aplikasi C yang utuh, tangguh, dan dapat diuji di dunia nyata.
- **Topik Pembelajaran:**
  1. **Arsitektur Multi-Threading:** Memisahkan peran aplikasi menjadi dua thread independen:
     - **Thread 1 (Transmitter):** Menembakkan frame Beacon ke udara setiap 100 ms secara periodik.
     - **Thread 2 (Sniffer & Responder):** Mendengarkan Probe Request dari smartphone korban dan seketika menembakkan Probe Response balik yang cocok.
  2. **Sinkronisasi Mutex:** Mencegah tabrakan pemanggilan `sendto()` pada deskriptor socket yang sama dari dua thread sekaligus menggunakan `pthread_mutex_lock()` dan `pthread_mutex_unlock()`.
  3. **Pembersihan Bersih (Clean Teardown):** Menutup socket (`close()`), menunggu thread selesai (`pthread_join()`), dan mengembalikan status terminal secara elegan saat pengguna menekan `Ctrl+C`.
- **Tolak Ukur Keberhasilan (Milestone):** Berhasil mengompilasi `rogue_ap_full.c` dengan `gcc -O2 rogue_ap_full.c -lpthread -o rogue_ap_full`, menjalankannya di Linux dengan kartu monitor mode, dan melihat SSID broadcast muncul di layar pencarian Wi-Fi smartphone!

---

## 10. Bab 9: Lampiran: Cheat Sheet & Fun Facts 📝

### 9.1. Istilah Penting Bytecode 802.11
- **Raw Socket:** Socket tingkat rendah yang memotong protokol transport/network bawaan kernel, memberi kebebasan menulis header layer link sendiri.
- **Endianness:** Urutan penyimpanan byte di memori. Jaringan menggunakan *Big-Endian*, sedangkan prosesor Intel/AMD/ARM umumnya *Little-Endian*.
- **Information Element (IE):** Format *Tag-Length-Value (TLV)* standar pada frame Management 802.11 untuk membawa parameter dinamis seperti nama SSID, kecepatan, dan channel.
- **Graceful Shutdown:** Praktik pemrograman di mana aplikasi merapikan memori, melepaskan socket, dan mematikan thread secara tertib sebelum keluar dari sistem.

### 9.2. Fun Facts 🎉
1. **Kabel vs Udara:** Pada jaringan kabel (Ethernet 802.3), kartu jaringan biasanya hanya menerima paket yang ditujukan untuk MAC-nya sendiri. Di udara (802.11), gelombang radio menyebar ke segala arah, sehingga mode monitor dapat mendengar "bisikan" semua perangkat di sekitarnya!
2. **Kecepatan 100 ms:** Standar interval Beacon 802.11 adalah 100 TU (*Time Units* = 102.4 mikrodetik). Itu artinya sebuah Access Point berteriak mengumumkan namanya sebanyak **10 kali setiap detik**!
3. **Mengapa Raw Socket Butuh Root?** Karena raw socket memberi akses tak terbatas untuk memalsukan (*spoofing*) paket jaringan apa pun, sistem operasi Linux membatasi pembuatannya hanya untuk pengguna dengan hak akses administratif (`root` atau capability `CAP_NET_RAW`).

---

## 🎉 Selesai!

Kamu telah berhasil menaklukkan "Bahasa & Bytecode 802.11", menguasai 15 Header Linux, dan memahami rancangan kode C tingkat tinggi!  
Langkah berikutnya: jelajahi laboratorium sistem pentesting lengkap di **[03_wifi_pineapple.md](03_wifi_pineapple.md)**! 🚀📡

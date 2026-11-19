# 📚 Kurikulum Belajar & Peta Arsitektur WiFi Pineapple C System Programming

**Panduan Langkah Demi Langkah: Dari Nol Compiler Hingga Sistem Audit Nirkabel Tingkat Enterprise**

Selamat datang! Dokumen ini dirancang khusus agar siapa pun — mulai dari yang baru pertama kali menyentuh bahasa C dan Makefile, hingga programmer yang ingin mendalami *low-level Linux network programming* — dapat memahami **alur pembuatan sistem ini secara logis, teratur, dan memiliki alasan kuat di setiap tahapannya.**

---

## 🗺️ Peta Filosofi Konstruksi: Piramida Sistem

Membangun perangkat lunak sistem tingkat rendah (*system programming*) di Linux diibaratkan membangun sebuah gedung pencakar langit:

```
                      ┌──────────────────────────┐
                      │   Tahap 8: Dashboard CLI │  ◄── Atap & Ruang Kemudi (main.c)
                      ├──────────────────────────┤
                      │  Tahap 7: Automasi & Lap.│  ◄── Lift & Laporan (modules, report)
                      ├──────────────────────────┤
                      │  Tahap 6: Layer 7 Proxy  │  ◄── Jaringan Interior (proxy, portal)
                      ├──────────────────────────┤
                      │  Tahap 5: Layer 3 Jaring.│  ◄── Pipa Air & Listrik (dhcp, dns)
                      ├──────────────────────────┤
                      │  Tahap 4: Mesin Nirkabel │  ◄── Dinding & Pintu (karma, deauth)
                      ├──────────────────────────┤
                      │  Tahap 3: Radio Sniffer  │  ◄── Sensor & Radar (sniffer, capture)
                      ├──────────────────────────┤
                      │  Tahap 2: Layer 2 Wi-Fi  │  ◄── Tiang Pancang Radio (rogue_ap)
                      ├──────────────────────────┤
                      │  Tahap 1: Kamus & Fondasi│  ◄── Pondasi Beton (config, utils, logger)
                      ├──────────────────────────┤
                      │  Tahap 0: GCC & Makefile │  ◄── Pabrik Semen & Peralatan Tukang
                      └──────────────────────────┘
```

Jika kamu langsung membuat antarmuka menu (Tahap 8) atau membuat serangan Wi-Fi (Tahap 4) tanpa fondasi socket dan tipe data (Tahap 1), programmu akan rapuh, membingungkan, dan dipenuhi kesalahan memori (*segmentation fault*). 

Mari kita bedah setiap tahap secara terperinci: **Apa yang dibuat, bagaimana caranya, dan MENGAPA tahapan itu wajib diletakkan di sana.**

---

## 🛠️ Tahap 0: Persiapan Compiler & Memahami Build System

### 1. Memasang GCC & Perkakas Kompilasi di Linux
Sebelum menulis satu baris kode C pun, komputer kita memerlukan dua alat utama:
- **`gcc` (GNU Compiler Collection):** Penerjemah kode teks C menjadi instruksi biner mesin yang dipahami oleh prosesor CPU.
- **`make`:** Automator yang menjalankan perintah kompilasi secara cerdas hanya pada berkas yang mengalami perubahan.

**Cara pasang di Ubuntu / Debian / Kali Linux / Raspberry Pi OS / WSL:**
```bash
sudo apt update
sudo apt install build-essential
```
*Perintah di atas otomatis memasang `gcc`, `g++`, `make`, dan pustaka standar Linux C (`glibc`).*

### 2. Mengapa Kita Wajib Menggunakan `Makefile`?
Bayangkan jika proyek ini terdiri dari **55 berkas**. Setiap kali kamu ingin menguji program, kamu harus mengetik perintah panjang ini di terminal:
```bash
gcc -Wall -Wextra -O2 -std=c11 -pthread -o wifi_pineapple main.c core/rogue_ap.c core/sniffer.c core/channel_hopper.c attack/karma.c attack/deauth.c attack/beacon_flood.c attack/wps_attack.c server/dhcp.c server/dns.c server/evil_portal.c server/ssl_strip.c mitm/capture.c mitm/session_hijack.c mitm/browser_exploit.c mitm/advanced_mitm.c mitm/proxy.c capture/handshake.c capture/client_db.c modules/module.c modules/multi_target.c modules/auto_exploit.c filter/filter.c report/report.c utils/utils.c utils/logger.c -lpthread -ldl
```
Satu huruf salah ketik, proses kompilasi gagal! Dan lebih buruk lagi, setiap kali kamu mengubah 1 baris komentar di `utils.c`, compiler harus mengulang kompilasi seluruh 50 berkas lainnya dari awal.

Dengan **`Makefile`**, kamu cukup mengetik satu kata saja:
```bash
make
```

### 3. Bedah Anatomi Berkas `Makefile`
Mari kita pahami isi `Makefile` proyek kita:
```makefile
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -pthread
LDFLAGS = -lpthread -ldl
TARGET = wifi_pineapple
```
- **`CC = gcc`**: Menentukan compiler C yang digunakan.
- **`CFLAGS` (Compiler Flags):**
  - `-Wall -Wextra`: Nyalakan semua peringatan compiler. Jika ada variabel tak terpakai atau casting tipe data yang meragukan, compiler akan memberitahu kita!
  - `-O2`: Optimasi kecepatan eksekusi kode tingkat 2 agar pemrosesan paket jaringan cepat tanpa lag.
  - `-std=c11`: Menggunakan standar bahasa C modern (ISO C11).
  - `-pthread`: Mengaktifkan pustaka threading POSIX secara aman.
- **`LDFLAGS = -lpthread -ldl`**: Menautkan pustaka eksternal sistem operasi:
  - `-lpthread`: Untuk multi-threading (Beacon thread, Sniffer thread, DHCP thread).
  - `-ldl`: Untuk Dynamic Linker (`dlopen`, `dlclose`) pembuat modul plugin `.so`.
- **Target `%.o: %.c`**: Aturan sakti yang meng-compile setiap berkas `.c` menjadi file objek `.o` secara terpisah. Jika hanya `proxy.c` yang diubah, `make` HANYA akan meng-compile `proxy.c`!

---

## 🧱 Tahap 1: Fondasi & Kamus Protokol Sentral

**Berkas yang Dibuat:**
- `config.h`
- `utils/logger.h` & `utils/logger.c`
- `utils/utils.h` & `utils/utils.c`

### Mengapa Tahap Ini Dibuat Pertama Kali?
- **Pusat Kebenaran Tunggal (*Single Source of Truth*):** Kamu tidak bisa merakit paket 802.11 sebelum mengetahui susunan bit dan byte-nya. `config.h` mendefinisikan bentuk memori `wifi_header_t` (24 bytes) dan opcode seperti `0x8000` (Beacon). Seluruh modul di masa depan akan membaca kamus ini.
- **Keamanan Multi-Thread:** Modul Wi-Fi kita memiliki banyak thread yang bekerja serentak. Jika Koki A dan Koki B sama-sama berteriak ke layar terminal tanpa antrean, tulisan di terminal akan hancur saling tumpang tindih. Karena itu, `utils/logger` dibuat di awal dengan proteksi `pthread_mutex_t`.
- **Pintu Masuk Perangkat Keras:** `utils/utils` menyediakan fungsi pembuka raw socket `open_raw_socket()` dan manipulasi alamat fisik MAC. Tanpa fungsi ini, modul radio tidak bisa berkomunikasi dengan kartu Wi-Fi.

---

## 📡 Tahap 2: Lapisan Nirkabel Layer 2 — Memancarkan Suar

**Berkas yang Dibuat:**
- `core/rogue_ap.h` & `core/rogue_ap.c`

### Mengapa Tahap Ini Dibuat Kedua?
- **Agar Terlihat di Radar:** Komputer atau smartphone di sekitarmu tidak akan pernah tahu keberadaan perangkatmu jika kamu tidak bersuara.
- **Beacon Frame:** Router Wi-Fi asli mengumumkan keberadaannya setiap 100 milidetik. Di tahap ini, kita belajar merakit paket biner frame Beacon byte-demi-byte di memori RAM dan menembakkannya langsung ke kartu Wi-Fi melalui raw socket `AF_PACKET`.
- **Respon Cepat Unicast:** Di modul ini juga disiapkan fungsi `send_probe_response`, `send_auth_response`, dan `send_assoc_response` agar ketika ada perangkat yang merespons suar kita, AP kita siap menyambutnya.

---

## 👂 Tahap 3: Pendengar Udara (Sniffer) & Basis Data Klien

**Berkas yang Dibuat:**
- `core/sniffer.h` & `core/sniffer.c`
- `capture/client_db.h` & `capture/client_db.c`
- `capture/handshake.h` & `capture/handshake.c`

### Mengapa Tahap Ini Dibuat Ketiga?
- **Menyeimbangkan "Bicara" dan "Mendengar":** Di Tahap 2 kita sudah bisa "bicara" (transmisi beacon). Sekarang di Tahap 3 kita membuat "telinga" — yaitu Sniffer yang berjalan di interface monitor mode (`mon0`).
- **Pelacakan Perangkat Korban:** Saat radio mendengar frame di udara, kita mencatat MAC address pengirim ke dalam `capture/client_db`. Ini seperti buku tamu hotel otomatis.
- **Penyelamat Bukti Audit (PCAP):** Di tahap ini pula penyaring frame EAPOL (`0x888E`) dipasang untuk menangkap 4-Way Handshake WPA dan menuliskannya ke berkas standar PCAP (`handshake.cap`).

---

## 🎯 Tahap 4: Mesin Pengujian Keamanan Nirkabel (Attack Engine)

**Berkas yang Dibuat:**
- `attack/karma.h` & `attack/karma.c`
- `attack/deauth.h` & `attack/deauth.c`
- `attack/beacon_flood.h` & `attack/beacon_flood.c`
- `attack/wps_attack.h` & `attack/wps_attack.c`

### Mengapa Tahap Ini Dibuat Keempat?
- **Menghubungkan Telinga dan Mulut:** Di Tahap 2 kita punya pemancar, di Tahap 3 kita punya pendengar. Di Tahap 4, kita menyatukan keduanya!
  - **Karma Attack:** Telinga (Sniffer) mendengar: *"Ada SSID 'CafeGratis'?"* ➔ Otak (Karma) memutuskan ➔ Mulut (Rogue AP) langsung berteriak: *"Ya, saya CafeGratis!"*
  - **Deauthentication Engine:** Memaksa perangkat client yang tersambung ke router uji lab untuk putus koneksi sesaat agar melakukan handshake ulang.
  - **Beacon Flood:** Memancarkan puluhan suar sekaligus untuk menguji ketahanan antarmuka pemindai perangkat client terhadap kelebihan beban spektrum radio.

---

## 🌐 Tahap 5: Layanan Jaringan Layer 3 (IP & DNS)

**Berkas yang Dibuat:**
- `server/dhcp.h` & `server/dhcp.c`
- `server/dns.h` & `server/dns.c`

### Mengapa Tahap Ini Dibuat Kelima?
- **Klien Butuh Alamat Rumah (IP):** Setelah smartphone korban tertipu dan berhasil tersambung ke Rogue AP di Layer 2 (Wi-Fi), mereka belum bisa bertukar data internet! Mengapa? Karena HP mereka berada dalam status *"Obtaining IP Address..."*.
- **DHCP Server Mandiri:** Modul `server/dhcp` hadir meminjamkan IP (192.168.1.100 s/d 200) dan memberitahu HP korban: *"Gateway kamu adalah 192.168.1.1, dan DNS server kamu adalah 192.168.1.1!"*.
- **DNS Spoofer:** Begitu HP korban membuka website apa pun, query nama domain akan masuk ke server DNS kita di port UDP 53. DNS Spoofer kita akan menjawab dengan IP lokal Rogue AP.

---

## 🕵️ Tahap 6: Analisis Lalu Lintas Aplikasi Layer 7 (MITM Chain & Proxy)

**Berkas yang Dibuat:**
- `mitm/proxy.h` & `mitm/proxy.c`
- `server/evil_portal.h` & `server/evil_portal.c`
- `mitm/capture.h` & `mitm/capture.c`
- `mitm/session_hijack.h` & `mitm/session_hijack.c`
- `mitm/browser_exploit.h` & `mitm/browser_exploit.c`
- `server/ssl_strip.h` & `server/ssl_strip.c`
- `filter/filter.h` & `filter/filter.c`

### Mengapa Tahap Ini Dibuat Keenam?
- **Pembedahan Data Tingkat Aplikasi:** Sekarang lalu lintas web korban telah mengalir ke perangkat kita. Modul Proxy HTTP bertindak sebagai penengah transparan.
- **Rantai Intersepsi (MITM Chain):**
  - Menginspeksi formulir login POST (`mitm/capture`).
  - Menggandakan cookie sesi otentikasi (`mitm/session_hijack`).
  - Menghapus enkripsi SSL/TLS pada tautan dokumen web (`server/ssl_strip`).
  - Menampilkan portal verifikasi Wi-Fi palsu yang responsif untuk simulasi phishing edukasi (`server/evil_portal`).

---

## ⚙️ Tahap 7: Automasi, Pengelolaan Spektrum Radio & Modul Plugin

**Berkas yang Dibuat:**
- `core/channel_hopper.h` & `core/channel_hopper.c`
- `modules/module.h` & `modules/module.c`
- `modules/multi_target.h` & `modules/multi_target.c`
- `modules/auto_exploit.h` & `modules/auto_exploit.c`
- `report/report.h` & `report/report.c`

### Mengapa Tahap Ini Dibuat Ketujuh?
- **Menghilangkan Keterbatasan Manual:** Tanpa rotasi frekuensi radio (`channel_hopper`), sistem hanya memantau 1 channel saja. Hopper memungkinkan radar melihat seluruh channel 1 s/d 11.
- **Arsitektur Plugin Ekstensibel:** Menggunakan `dlopen()` pada `modules/module` memungkinkan penambahan fitur baru di masa depan tanpa mengubah kode sumber program inti.
- **Laporan Investigasi Resmi:** Modul `report` mengumpulkan bukti-bukti pengujian lab (log kredensial, cookie, PCAP handshake) menjadi satu berkas ringkasan audit (`report_audit.txt`).

---

## 🎛️ Tahap 8: Integrasi Dasbor Terminal CLI & Graceful Shutdown

**Berkas yang Dibuat:**
- `main.c`

### Mengapa Tahap Ini Dibuat Paling Akhir?
- **Sang Dirigen Orkestra:** `main.c` tidak boleh ditulis di awal karena ia memanggil fungsi-fungsi dari seluruh modul Tahap 1 hingga Tahap 7.
- **Penanganan Sinyal & Pembersihan Aman:** Saat pengguna menekan tombol keyboard `Ctrl+C`, `main.c` bertugas memastikan:
  - Seluruh thread dihentikan dengan tertib (`pthread_join`).
  - Seluruh socket ditutup tanpa meninggalkan socket yatim piatu di kernel.
  - Kartu Wi-Fi dikembalikan ke konfigurasi semula tanpa merusak sistem jaringan komputer host.

---

## 🌳 Pohon Lengkap 55 Berkas Proyek & Pemetaannya

Berikut adalah inventaris total 55 berkas yang telah dibuat di folder [wifi_pineapple/](wifi_pineapple):

```text
wifi_pineapple/
│
├── config.h                        # [Tahap 1] Master Kamus Protokol & Struct Packed
├── main.c                          # [Tahap 8] Dasbor Menu CLI Interaktif 32 Fitur
├── Makefile                        # [Tahap 0] Skrip Build Otomatis GNU Make
├── README.md                       # Dokumentasi Cepat Proyek
├── KURIKULUM.md                    # Panduan Kurikulum Belajar & Mengapa-nya (Dokumen ini)
│
├── core/                           # Modul Inti Radio Wi-Fi
│   ├── rogue_ap.h / rogue_ap.c     # [Tahap 2] Pemancar Beacon, Probe, Auth, Assoc Response
│   ├── sniffer.h / sniffer.c       # [Tahap 3] Penyalur Frame Radio Non-blocking di mon0
│   └── channel_hopper.h / .c       # [Tahap 7] Pemutar Frekuensi Radio 2.4 GHz & 5 GHz
│
├── attack/                         # Mesin Pengujian Nirkabel (Attack Modules)
│   ├── karma.h / karma.c           # [Tahap 4] Penyerapan Probe Request Target
│   ├── deauth.h / deauth.c         # [Tahap 4] Injektor Frame Pemutus Koneksi 0xC000
│   ├── beacon_flood.h / .c         # [Tahap 4] Pembanjir Multi-SSID Acak di Udara
│   └── wps_attack.h / .c           # [Tahap 4] Audit WPS PIN Pixie Dust via Reaver
│
├── server/                         # Layanan Jaringan Lokal (Layer 3 & Aplikasi)
│   ├── dhcp.h / dhcp.c             # [Tahap 5] Server DHCP Mini Peminjam IP Korban
│   ├── dns.h / dns.c               # [Tahap 5] Server DNS Spoofer Pembelok Domain
│   ├── evil_portal.h / .c          # [Tahap 6] Web Server Captive Portal Login Phishing
│   └── ssl_strip.h / ssl_strip.c   # [Tahap 6] Mesin Downgrade Enkripsi Web HTTPS->HTTP
│
├── mitm/                           # Analisis Lalu Lintas Transparan (Layer 7)
│   ├── capture.h / capture.c       # [Tahap 6] Penyadapan Form Login POST & Host Header
│   ├── session_hijack.h / .c       # [Tahap 6] Perekam Token Cookie Sesi Set-Cookie
│   ├── browser_exploit.h / .c      # [Tahap 6] Injektor Skrip JavaScript ke Tag </body>
│   ├── advanced_mitm.h / .c        # [Tahap 6] Pengendali Orkestrasi Rantai MITM
│   └── proxy.h / proxy.c           # [Tahap 6] Server Proxy Transparan Port 8080
│
├── capture/                        # Perekaman Barang Bukti
│   ├── handshake.h / handshake.c   # [Tahap 3] Perekam 4-Way Handshake EAPOL ke PCAP
│   └── client_db.h / client_db.c   # [Tahap 3] Basis Data Pelacakan Klien di RAM
│
├── modules/                        # Plugin & Automasi Dinamis
│   ├── module.h / module.c         # [Tahap 7] Dynamic Linker Plugin Shared Library (.so)
│   ├── multi_target.h / .c         # [Tahap 7] Antrean Serangan Multi-Target Deauth
│   └── auto_exploit.h / .c         # [Tahap 7] Mode Penyerangan Otonom Otomatis
│
├── filter/                         # Penyaring Konten
│   └── filter.h / filter.c         # [Tahap 6] Firewall Aturan Domain & URL
│
├── report/                         # Pelaporan Investigasi
│   └── report.h / report.c         # [Tahap 7] Penyusun Berkas Ringkasan Laporan Audit
│
├── utils/                          # Perkakas Tingkat Rendah & Pencatatan
│   ├── utils.h / utils.c           # [Tahap 1] Raw Socket AF_PACKET, MAC Parser, Hexdump
│   └── logger.h / logger.c         # [Tahap 1] Logging Thread-safe 5 Level dengan Mutex
│
└── examples/                       # Laboratorium Belajar Mandiri
    └── rogue_ap_full.c             # [Tahap 2] Program 1 File Mandiri dari Dokumen 2
```

---

## 🎯 Panduan Memulai Belajar Hari Ini

1. Buka terminal di folder proyek:
   ```bash
   cd wifi_pineapple
   ```
2. Jalankan perintah kompilasi:
   ```bash
   make
   ```
3. Baca urutan kode mengikuti nomor tahap di atas:
   - Mulai dari `config.h` dan `utils/utils.c`.
   - Lanjut ke `core/rogue_ap.c` dan `core/sniffer.c`.
   - Pahami bagaimana `server/dhcp.c` dan `server/dns.c` bekerja sama.
   - Terakhir, lihat bagaimana `main.c` menyatukan semuanya ke dalam menu CLI yang elegan!

Selamat menjelajah dunia pemrograman sistem jaringan Linux tingkat rendah! 🚀📡✨

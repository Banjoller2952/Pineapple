// ============================================================================
// HEADER: capture/handshake.h — Perekam 4-Way Handshake WPA/WPA2 (EAPOL)
// ============================================================================
#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "../config.h"

int handshake_init(const char *output_pcap);
void handshake_cleanup(void);
int handshake_process_packet(const uint8_t *frame, int len, const uint8_t *bssid);
int handshake_save_pcap(const char *path);

#endif // HANDSHAKE_H

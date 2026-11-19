// ============================================================================
// HEADER: attack/deauth.h — Mesin Injeksi Deauthentication 802.11
// ============================================================================
#ifndef DEAUTH_H
#define DEAUTH_H

#include "../config.h"

// API Kontrol Thread Deauthentication
int deauth_start(const char *iface, const uint8_t *bssid, const uint8_t *client, int reason, int burst);
void deauth_stop(void);
int deauth_stop_all_wait(int timeout_ms);
void deauth_reset(void);
void close_deauth_socket(void);

// Transmisi Paket Deauth Satuan
int send_deauth_frame(int sock, const struct sockaddr_ll *sll,
                      const uint8_t *bssid, const uint8_t *client, int reason);
int send_deauth_broadcast(const char *iface, const uint8_t *bssid, int reason, int count);

#endif // DEAUTH_H

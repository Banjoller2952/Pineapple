/*
 * ============================================================================
 * HEADER     : core/rogue_ap.h
 * MODUL      : Mesin Pemancar & Pengelola Rogue Access Point
 * DESKRIPSI  : Antarmuka pembuatan dan injeksi frame Beacon, Probe Response,
 *              Authentication Response, dan Association Response.
 * ============================================================================
 */

#ifndef ROGUE_AP_H
#define ROGUE_AP_H

#include "../config.h" // Butuh ap_config_t dan wifi_header_t

// --- Siklus Hidup Rogue AP ---
int setup_ap_mode(const char *iface);
int start_rogue_ap(ap_config_t *cfg);
void close_tx_socket(void);
void *beacon_thread(void *arg);

// --- Generator Transmisi Frame 802.11 ---
int send_beacon(ap_config_t *cfg);
int send_beacon_with_ssid(ap_config_t *cfg, const char *ssid);
int send_probe_response(ap_config_t *cfg, uint8_t *client_mac, const char *ssid);
int send_auth_response(ap_config_t *cfg, uint8_t *client_mac);
int send_assoc_response(ap_config_t *cfg, uint8_t *client_mac);

#endif // ROGUE_AP_H

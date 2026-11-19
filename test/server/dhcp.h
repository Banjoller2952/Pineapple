// ============================================================================
// HEADER: server/dhcp.h — Server DHCP Ringan Peminjam Alamat IP
// ============================================================================
#ifndef DHCP_H
#define DHCP_H

#include "../config.h"

// Konstanta Tipe Pesan DHCP
#define DHCP_DISCOVER       1
#define DHCP_OFFER          2
#define DHCP_REQUEST        3
#define DHCP_ACK            5
#define DHCP_SERVER_PORT    67
#define DHCP_CLIENT_PORT    68

// API Siklus Hidup DHCP Server
int dhcp_start(ap_config_t *cfg);
int dhcp_stop(void);

#endif // DHCP_H

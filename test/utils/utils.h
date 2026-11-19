// ============================================================================
// HEADER: utils/utils.h — Perkakas Jaringan, Soket, dan Manipulasi MAC
// ============================================================================
#ifndef UTILS_H
#define UTILS_H

#include "../config.h"
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/if.h>

// Fungsi Operasional Raw Socket & Interface
int open_raw_socket(const char *iface, struct sockaddr_ll *sll);
int set_interface_channel(const char *iface, int channel);
int set_interface_promisc(const char *iface, bool enable);

// Fungsi Parsing & Pemformatan Data
int parse_mac(const char *str, uint8_t *mac);
void format_mac(const uint8_t *mac, char *out);
void format_timestamp(time_t t, char *out, size_t max_len);
void get_random_mac(uint8_t *mac);
void hexdump(const void *data, size_t len);

#endif // UTILS_H

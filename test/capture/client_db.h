// ============================================================================
// HEADER: capture/client_db.h — Basis Data Perangkat Klien Terdeteksi
// ============================================================================
#ifndef CLIENT_DB_H
#define CLIENT_DB_H

#include "../config.h"

int client_db_init(void);
void client_db_cleanup(void);
int client_db_add_or_update(const uint8_t *mac, const char *ip, bool connected);
client_t *client_db_get(const uint8_t *mac);
int client_db_list(client_t *out_list, int max_clients);
int client_db_count(void);

#endif // CLIENT_DB_H

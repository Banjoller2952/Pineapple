# ============================================================================
# MAKEFILE: Proyek WiFi Pineapple Pure C Enterprise (v15.4)
# ============================================================================

CC = gcc
# Flag kompilasi ketat untuk menjamin kepatuhan standar C11 dan keamanan memori
CFLAGS = -Wall -Wextra -O2 -std=c11 -pthread -I.
# Pustaka pendukung: lpthread (Multi-threading) dan ldl (Dynamic Plugin Loader)
LDFLAGS = -lpthread -ldl
TARGET = wifi_pineapple

# Seluruh berkas source code C dari Fase 1 hingga Fase 12
SRCS = main.c     core/rogue_ap.c core/sniffer.c core/channel_hopper.c     attack/karma.c attack/deauth.c attack/beacon_flood.c attack/wps_attack.c     server/dhcp.c server/dns.c server/evil_portal.c server/ssl_strip.c     mitm/capture.c mitm/session_hijack.c mitm/browser_exploit.c     mitm/advanced_mitm.c mitm/proxy.c     capture/handshake.c capture/client_db.c     modules/module.c modules/multi_target.c modules/auto_exploit.c     filter/filter.c report/report.c     utils/utils.c utils/logger.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build Berhasil: $(TARGET) (v15.4 Ready for Action)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.log *.cap *.txt

run: $(TARGET)
	sudo ./$(TARGET)

debug: CFLAGS += -g -DDEBUG
debug: clean $(TARGET)

.PHONY: all clean run debug

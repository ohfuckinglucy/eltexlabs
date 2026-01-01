SRCDIR := src
BINDIR := bin
INCDIR := include

SSERVER_SRC := $(SRCDIR)/s_server_posix.c
SSERVERP_SRC := $(SRCDIR)/s_server_system.c
SERVER_SRC := $(SRCDIR)/server.c $(SRCDIR)/functions.c
SCLIENT_SRC := $(SRCDIR)/s_client_posix.c
SCLIENTP_SRC := $(SRCDIR)/s_client_system.c
CLIENT_SRC := $(SRCDIR)/client.c $(SRCDIR)/functions.c

BINS := $(BINDIR)/s_server_posix.elf $(BINDIR)/s_client_posix.elf $(BINDIR)/s_server_system.elf $(BINDIR)/s_client_system.elf $(BINDIR)/server.elf $(BINDIR)/client.elf

CC := gcc
CFLAGS := -I$(INCDIR) -Wall -Wextra

.PHONY: all clean run help

all: $(BINDIR) $(BINS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/s_server_posix.elf: $(SSERVER_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(SSERVER_SRC) -o $@

$(BINDIR)/s_client_posix.elf: $(SCLIENT_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(SCLIENT_SRC) -o $@

$(BINDIR)/s_server_system.elf: $(SSERVERP_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(SSERVERP_SRC) -o $@

$(BINDIR)/s_client_system.elf: $(SCLIENTP_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(SCLIENTP_SRC) -o $@

$(BINDIR)/client.elf: $(CLIENT_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $@ -lncurses

$(BINDIR)/server.elf: $(SERVER_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $@ -lncurses

clean:
	rm -rf $(BINDIR)

run: all
	@echo " ./bin/s_server_posix.elf"
	@echo " ./bin/s_client_posix.elf"

help:
	@echo "Доступные команды:"
	@echo "  make        - компиляция всех программ в папку bin/"
	@echo "  make clean  - удаление папки bin/"
	@echo "  make run    - показать инструкцию по запуску"
	@echo "  make help   - показать эту справку"
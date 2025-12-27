SRCDIR := src
BINDIR := bin
INCDIR := src

FORK_SRC := $(SRCDIR)/fork.c $(SRCDIR)/functions.c
CLIENT_SRC := $(SRCDIR)/client.c
SERVER_SRC := $(SRCDIR)/server.c
BASH_SRC := ${SRCDIR}/bash.c $(SRCDIR)/functions.c

BINS := $(BINDIR)/fork.elf $(BINDIR)/client.elf $(BINDIR)/server.elf $(BINDIR)/bash.elf 

CC := gcc
CFLAGS := -I$(INCDIR) -Wall -Wextra

.PHONY: all clean run help

all: $(BINDIR) $(BINS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/fork.elf: $(FORK_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(FORK_SRC) -o $@

$(BINDIR)/client.elf: $(CLIENT_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $@

$(BINDIR)/server.elf: $(SERVER_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(SERVER_SRC)  -o $@

$(BINDIR)/bash.elf: $(BASH_SRC) $(INCDIR)/bash.h | $(BINDIR)
	$(CC) $(CFLAGS) $(BASH_SRC) -o $@

clean:
	rm -rf $(BINDIR)

run: all
	@echo "  ./bin/fork.elf"
	@echo " sudo ./bin/client.elf"
	@echo " sudo ./bin/server.elf"

help:
	@echo "Доступные команды:"
	@echo "  make        - компиляция всех программ в папку bin/"
	@echo "  make clean  - удаление папки bin/"
	@echo "  make run    - показать инструкцию по запуску"
	@echo "  make help   - показать эту справку"
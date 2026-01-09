SRCDIR := src
BINDIR := bin
INCDIR := include

KILLER_SRC := $(SRCDIR)/killer.c
FIRST_SRC := $(SRCDIR)/first.c $(SRCDIR)/functions.c
SECOND_SRC := $(SRCDIR)/second.c $(SRCDIR)/functions.c
THIRD_SRC := $(SRCDIR)/third.c $(SRCDIR)/functions.c

BINS := $(BINDIR)/first.elf $(BINDIR)/killer.elf $(BINDIR)/second.elf $(BINDIR)/third.elf 

CC := gcc
CFLAGS := -I$(INCDIR) -Wall -Wextra

.PHONY: all clean run help

all: $(BINDIR) $(BINS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/first.elf: $(FIRST_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(FIRST_SRC) -o $@

$(BINDIR)/second.elf: $(SECOND_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(SECOND_SRC) -o $@

$(BINDIR)/third.elf: $(THIRD_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(THIRD_SRC) -o $@

$(BINDIR)/killer.elf: $(KILLER_SRC) $(INCDIR)/header.h | $(BINDIR)
	$(CC) $(CFLAGS) $(KILLER_SRC) -o $@

clean:
	rm -rf $(BINDIR)

run: all
	@echo " ./bin/first.elf"

help:
	@echo "Доступные команды:"
	@echo "  make        - компиляция всех программ в папку bin/"
	@echo "  make clean  - удаление папки bin/"
	@echo "  make run    - показать инструкцию по запуску"
	@echo "  make help   - показать эту справку"
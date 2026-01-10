CC = gcc
LIBS = -lncurses
TARGET = main.elf
SOURCES = main.c functions.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

help:
	@echo "Доступные команды:"
	@echo "  make        - компиляция программы"
	@echo "  make clean  - удаление исполняемого файла"
	@echo "  make run    - запуск программы"
	@echo "  make help   - показать эту справку"
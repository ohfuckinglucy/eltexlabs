all: main.elf

main.elf: main.c functions.c header.h
	gcc main.c functions.c -o main.elf -pthread

clean:
	rm -f main.elf

run: main.elf
	./main.elf

help:
	@echo "Доступные команды:"
	@echo "  make        - компиляция программы"
	@echo "  make clean  - удаление исполняемого файла"
	@echo "  make run    - запуск программы"
	@echo "  make help   - показать эту справку"
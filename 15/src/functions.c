#include "../include/header.h"

void sigusr_handler(int signum){
    printf("Получен сигнал %d.\n", signum);
    exit(0);
}
#include "../include/header.h"

int main(void) {
    sigset_t mask;
    int sig;

    sigemptyset(&mask);

    sigaddset(&mask, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1){
        perror("sigprocmask");
        exit(1);
    }

    printf("Ожидание SIGUSR1(pid: %d)\n", getpid());

    while (1){
        if (sigwait(&mask, &sig) != 0){
            perror("sigwait");
            exit(1);
        }

        if (sig == SIGUSR1){
            printf("Получен сигнал SIGUSR1!\n");
        }
    }

    return 0;
}
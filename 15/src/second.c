#include "../include/header.h"

int main(){
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1){
        perror("sigprocmask");
        exit(1);
    }

    printf("Бесконечгный цикл(pid: %d).\n", getpid());

    while(1){
        sleep(1);
    }

    return 0;
}
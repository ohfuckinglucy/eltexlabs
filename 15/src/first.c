#include "../include/header.h"

int main(){
    struct sigaction sa;

    sa.sa_handler = sigusr_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1, &sa, NULL) == -1){
        perror("sigaction");
        exit(1);
    }

    printf("Ожидание SIGUSR1(pid: %d)\n", getpid());

    while(1){
        sleep(1);
    }

    return 0;
}
#include "../include/header.h"

int main(){
    pid_t pid;

    printf("Введите pid процесса, на который хотите отправить сигнал: ");
    scanf("%d", &pid);

    printf("\n");

    int sig;

    printf("Введите номер посылаемого сигнала: ");
    scanf("%d", &sig);

    printf("\n");

    if (kill(pid, sig) == 0){
        printf("Успешно!!!\n");
    } else {
        perror("kill: Ошибка при отправке сигнала");
        exit(1);
    }
    return 0;
}
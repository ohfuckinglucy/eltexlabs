#include "header.h"

void *consumer(void *arg){
    struct ConsumerArgs *args = arg;

    int id = args->id;
    int *shop = args->shop;

    int balance = rand() % 2000 + 9000;

    while (balance >= 0){
        int buy = 0;
        for (int i = 0; i < 5; i++) {
            if ((pthread_mutex_trylock(&mutex[i]) == 0) && shop[i] >= 5) {
                shop[i] -= 5;
                buy = 1;
                // sleep(1);
                pthread_mutex_unlock(&mutex[i]);
                break;
            }
        }
        if (buy){
            balance -= 5;
            printf("Потребитель чтото купил\nbalance[%d]: %d\n", id, balance);
        } else{
            printf("Потребитель завершил работу\nbalance[%d]: %d\n", id, balance);
            break;
        }
    }
}

void *producer(void *arg){
    struct ConsumerArgs *args = arg;

    int id = args->id;
    int *shop = args->shop;

    while(!stop_producer){
        for (int i = 0; i < 5; i++) {
            if ((pthread_mutex_trylock(&mutex[i]) == 0) && shop[i] >= 0) {
                shop[i] += 5;
                // sleep(1);
                pthread_mutex_unlock(&mutex[i]);
                break;
            }
        }
        printf("Погрузчик что-то принес\n");
        for (int  i = 0; i < 5; i++){
            printf("Shop[%d]: %d\n", i, shop[i]);
        }
    }

    printf("Погрузчик завершил работу\n");
}
#include "header.h"

pthread_mutex_t mutex[5];
int stop_producer = 0;

int main(){
    srand(0);
    int shop[5];
    int consumer_num[3];
    pthread_t num_of_consumer[4];

    struct ConsumerArgs args[4];

    for (int i = 0; i < 5; i++){
        pthread_mutex_init(&mutex[i], NULL);
    }

    for (int  i = 0; i < 5; i++){
        shop[i] = rand() % 200 + 400;
        
    }
    for (int i = 0; i < 3; i++){
        args[i].id = i;
        args[i].shop = shop;
        pthread_create(&num_of_consumer[i], NULL, consumer, &args[i]);
    }

    args[3].id = 3;
    args[3].shop = shop;

    pthread_create(&num_of_consumer[3], NULL, producer, &args[3]);

    for (int i = 0; i < 3; i++){
        pthread_join(num_of_consumer[i], NULL);
    }
    stop_producer = 1;
    pthread_join(num_of_consumer[3], NULL);
}
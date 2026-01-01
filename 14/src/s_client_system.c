#include "../include/header.h"

int main(){
    const char *file_pathname = FILE_PATH;
    int ftok_id = FTOK_ID;
    size_t size = BUFFER_SIZE;
    void *shm_ptr;

    struct sembuf minus = {0, -1, 0};

    key_t key = ftok(file_pathname, ftok_id);
    key_t key_sem = ftok(file_pathname, ftok_id+1);

    if (key_sem == -1){
        perror("ftok");
        exit(1);
    }

    if (key == -1){
        perror("ftok");
        exit(1);
    }

    int sem_id = semget(key_sem, 1, 0);

    if (sem_id == -1){
        perror("semget");
        exit(1);
    }

    int shm_id = shmget(key, size, 0);

    if (shm_id == -1){
        perror("shmget");
        exit(1);
    }

    shm_ptr = shmat(shm_id, NULL, 0);

    printf("Получено клиентом: %s\n", (char*)shm_ptr);

    snprintf(shm_ptr, BUFFER_SIZE, "%s", "Hello!");

    semop(sem_id, &minus, 1);

    if (shmdt(shm_ptr) == -1){
        perror("shmdt");
        exit(1);
    }

    return 0;
}
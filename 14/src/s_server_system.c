// открывает очередь, отправляет "Hi!" и ждет ответа от клиента в виде "Hello!" SYSTEM V
#include "../include/header.h"

int main(){
    const char *file_pathname = FILE_PATH;
    int ftok_id = FTOK_ID;
    size_t size = BUFFER_SIZE;
    void *shm_ptr;

    struct sembuf plus = {0, 1, 0};
    struct sembuf zero = {0, 0, 0};

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

    int sem_id = semget(key_sem, 1, IPC_CREAT | 0666);

    if (sem_id == -1){
        perror("semget");
        exit(1);
    }

    union semun arg;
    arg.val = 0;
    if (semctl(sem_id, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL");
        exit(1);
    }

    int shm_id = shmget(key, size, IPC_CREAT | 0666);

    if (shm_id == -1){
        perror("shmget");
        exit(1);
    }

    shm_ptr = shmat(shm_id, NULL, 0);

    snprintf(shm_ptr, BUFFER_SIZE, "%s", "Hi!");

    semop(sem_id, &plus, 1);

    semop(sem_id, &zero, 1);

    printf("Получено сервером: %s\n", (char*)shm_ptr);

    if (shmdt(shm_ptr) == -1){
        perror("shmdt");
        exit(1);
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    if (semctl(sem_id, 1, IPC_RMID) == -1){
        perror("semctl");
        exit(1);
    }

    return 0;
}
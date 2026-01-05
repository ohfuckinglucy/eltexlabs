#include "../include/header.h"

int main(){
    if (signal(SIGINT, sigint_handler) == SIG_ERR){
        perror("signal");
        exit(1);
    }
    
    const char *name_shm = SHM_NAME;
    const char *name_sem = SEM_NAME;

    void *ptr_shm;
    sem_t *ptr_sem;
    size_t size = sizeof(struct chat_room);
    int fd_shm = shm_open(name_shm, O_CREAT | O_RDWR, 0666);
    if (fd_shm == -1){
        perror("shm_open");
        exit(1);
    }
    ptr_sem = sem_open(name_sem, O_CREAT | O_RDWR, 0666, 1);
    if(ptr_sem == SEM_FAILED){
        perror("sem_open");
        exit(1);
    }

    if (ftruncate(fd_shm, size) == -1){
        perror("fruncate");
        close(fd_shm);
        exit(1);
    }

    ptr_shm = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    if (ptr_shm == MAP_FAILED){
        perror("mmap");
        close(fd_shm);
        exit(1);
    }

    struct chat_room *room = (struct chat_room *)ptr_shm;
    memset(ptr_shm, 0, sizeof(struct chat_room));

    while (flag) {
        sem_wait(ptr_sem);
        printf("Total messages: %d\n", room->count);
        for (int i = 0; i < room->count; i++) {
            printf("[%d][%c] %s: %s\n", i,
                room->messages[i].type, 
                room->messages[i].name, 
                room->messages[i].text);
        }
        sem_post(ptr_sem);
        sleep(1);
    }

    printf("Ctrl + c: Завершение сервера\n");

    shm_unlink(name_shm);
    sem_unlink(name_sem);

    close(fd_shm);
    munmap(ptr_shm, size);
    sem_close(ptr_sem);

    return 0;
}
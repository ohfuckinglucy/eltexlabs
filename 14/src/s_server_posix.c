// открывает разделяемую память, отправляет "Hi!" и ждет ответа от клиента в виде "Hello!" POSIX
#include "../include/header.h"

int main(){
    const char *name_shm = SHM_NAME;
    const char *name_sem = SEM_NAME;
    void *ptr_shm;
    sem_t *ptr_sem;
    size_t size = BUFFER_SIZE;
    int fd_shm = shm_open(name_shm, O_CREAT | O_RDWR, 0666);
    if (fd_shm == -1){
        perror("shm_open");
        exit(1);
    }
    ptr_sem = sem_open(name_sem, O_CREAT | O_RDWR, 0666, 0);
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

    snprintf(ptr_shm, BUFFER_SIZE, "%s", "Hi!");

    sem_wait(ptr_sem);

    printf("Принято сервером: %s\n", (char *)ptr_shm);

    munmap(ptr_shm, size);
    close(fd_shm);
    sem_close(ptr_sem);
    shm_unlink(name_shm); 
    sem_unlink(name_sem);

    return 0;
}
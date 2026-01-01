#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <ncurses.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <locale.h> 

#define BUFFER_SIZE 64
#define SHM_NAME "/shm_posix"
#define SEM_NAME "/sem_posix"
#define FILE_PATH "Makefile"
#define FTOK_ID 475

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

extern WINDOW *chat_win;
extern WINDOW *input_win;
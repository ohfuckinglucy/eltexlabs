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
#include <ctype.h>

#define BUFFER_SIZE 64
#define NAME_SIZE 32
#define MAX_MSGS 128
#define SHM_NAME "/shm_posix"
#define SEM_NAME "/sem_posix"
#define FILE_PATH "Makefile"
#define FTOK_ID 475

extern volatile sig_atomic_t flag;

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct chat_room{
    int count;
    struct {
        char type; // J - join M - msgm Q- quit, s - systenm
        char name[NAME_SIZE];
        char text[BUFFER_SIZE];
    } messages[MAX_MSGS];
};

struct thread_args{
    sem_t *ptr_sem;
    WINDOW *chat;
    struct chat_room *room;
    int state;
};

void input_txt(char *buffer, WINDOW *input);
void get_username(char *name);
void sigint_handler(int sig);
void *pthread_chat(void *arg);
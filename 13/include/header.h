#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <ncurses.h>

#define BUFFER_SIZE 64
#define NAME_SIZE 32
#define MAX_CLIENTS 4
#define Q_NAME 32
#define MSG_SIZE sizeof(struct msg)
#define KEY 666

struct msgbuf{
    long mtype;
    char mtext[BUFFER_SIZE];
};

struct msg {
    char type; // J - join, M - msg, Q quit, S - service
    char q_name[Q_NAME];
    char name[NAME_SIZE];
    char text[BUFFER_SIZE];
};

struct client_data {
    mqd_t mq_receive;
    mqd_t mq_server;
    char username[NAME_SIZE];
};

extern volatile sig_atomic_t flag;
extern WINDOW *chat_win;
extern WINDOW *input_win;

void sigint_handler(int sig);
void *read_msg(void *arg);
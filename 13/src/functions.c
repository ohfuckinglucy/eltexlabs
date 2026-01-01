#include "../include/header.h"

volatile sig_atomic_t flag = 1;
WINDOW *chat_win;
WINDOW *input_win;

void sigint_handler(int sig){
    flag = 0;
    printf("Ctrl + c: Завершение программы\n");
}

void *read_msg(void *arg) {
    struct client_data *data = (struct client_data *)arg;
    struct msg msg_inc;

    while (flag) {
        if (mq_receive(data->mq_receive, (char *)&msg_inc, MSG_SIZE, NULL) == -1){
            usleep(100000);
            continue;
        }
        wprintw(chat_win, "%s: %s\n", msg_inc.name, msg_inc.text);
        wrefresh(chat_win);

    }
    return NULL;
}
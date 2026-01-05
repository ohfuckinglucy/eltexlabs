#include "../include/header.h"

volatile sig_atomic_t flag = 1;

void sigint_handler(int sig){
    (void)sig;
    flag = 0;
}

void input_txt(char *buffer, WINDOW *input){
    int ch;
    int pos = 0;

    while (((ch = wgetch(input)) != '\n') || pos == 0){
        if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (pos > 0) {
                pos--;
                buffer[pos] = '\0';

                werase(input);
                wprintw(input, "> %s", buffer);
                wrefresh(input);
            }
        } else if (pos < BUFFER_SIZE - 1 && isprint(ch)) {
            buffer[pos++] = (char)ch;
            buffer[pos] = '\0';

            werase(input);
            wprintw(input, "> %s", buffer);
            wrefresh(input);
        }
    }
    
    buffer[pos] = '\0';

    wrefresh(input);
}

void get_username(char *name){
    int row, col;
    getmaxyx(stdscr, row, col);

    int h = 5;
    int w = 30;
    int y = row / 2 - h / 2;
    int x = col / 2 - w / 2;

    WINDOW *popup = newwin(h, w, y, x);
    keypad(popup, TRUE);
    wborder(popup, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwprintw(popup, 1, 2, "Ur name:");
    mvwprintw(popup, 2, 2, "> ");
    wrefresh(popup);

    echo();
    wattroff(popup, A_PROTECT);

    wscanw(popup, "%31s", name);

    noecho();

    delwin(popup);
    redrawwin(stdscr);
    refresh();
}
void *pthread_chat(void *arg){
    struct thread_args *args = (struct thread_args *)arg;
    while (1) {
        sem_wait(args->ptr_sem);
        werase(args->chat);

        for (int i = 0; i < args->room->count; i++) {
            if (args->room->messages[i].type == 'J') {
                mvwprintw(args->chat, i + 1, 1, "User %s connect to chat. (type \"exit\" for disconnect)", 
                          args->room->messages[i].name);
            } else if (args->room->messages[i].type == 'Q') {
                mvwprintw(args->chat, i + 1, 1, "User %s disconnect to chat.", 
                          args->room->messages[i].name);
            } else if (args->room->messages[i].type == 'M') {
                mvwprintw(args->chat, i + 1, 1, "%s: %s", 
                          args->room->messages[i].name, args->room->messages[i].text);
            }
        }

        wborder(args->chat, 0, 0, 0, 0, 0, 0, 0, 0);
        wrefresh(args->chat);
        sem_post(args->ptr_sem);
        usleep(200000);

        if (args->state == 2){
            break;
        }
    }
    return NULL;
}

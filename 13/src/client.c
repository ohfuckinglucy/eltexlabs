#include "../include/header.h"

int main() {
    short first_call = 1;
    char q_name[Q_NAME];
    snprintf(q_name, sizeof(q_name), "/client_%d", getpid());
    struct msg msg = {0};
    strncpy(msg.q_name, q_name, Q_NAME);

    struct mq_attr attr = {0};
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE;

    mqd_t mq = mq_open(q_name, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &attr);
    mqd_t mq_serv = mq_open("/server", O_RDWR, 0666, NULL);

    if (mq == -1 || mq_serv == -1) {
        perror("mq_open");
        exit(1);
    }

    printf("Введите имя пользователя: ");
    fgets(msg.name, NAME_SIZE, stdin);
    msg.name[strcspn(msg.name, "\n")] = '\0';

    initscr();
    noecho();
    cbreak();
    curs_set(1);

    int height, width;
    getmaxyx(stdscr, height, width);

    chat_win = newwin(height - 1, width, 0, 0);
    input_win = newwin(1, width, height - 1, 0);

    scrollok(chat_win, TRUE);
    wrefresh(chat_win);
    wrefresh(input_win);

    struct client_data data = {0};
    data.mq_receive = mq;
    data.mq_server = mq_serv;
    strncpy(data.username, msg.name, sizeof(data.username) - 1);

    signal(SIGINT, sigint_handler);

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, read_msg, &data);

    char input_line[MSG_SIZE];
    while (flag) {
        wprintw(input_win, "> ");
        wrefresh(input_win);

        echo();
        if (wgetnstr(input_win, input_line, sizeof(input_line) - 1) == ERR) {
            break;
        }
        noecho();

        werase(input_win);
        wrefresh(input_win);

        strncpy(msg.text, input_line, MSG_SIZE - 1);
        msg.text[MSG_SIZE - 1] = '\0';
        msg.type = first_call ? 'J' : 'M';
        first_call = 0;
        strncpy(msg.name, data.username, NAME_SIZE);

        mq_send(mq_serv, (const char *)&msg, MSG_SIZE, 1);
    }

    msg.type = 'Q';
    mq_send(mq_serv, (const char *)&msg, MSG_SIZE, 1);

    pthread_join(recv_thread, NULL);

    endwin();

    mq_unlink(q_name);
    mq_close(mq);
    mq_close(mq_serv);

    return 0;
}
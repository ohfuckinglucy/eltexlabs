#include "../include/header.h"

int main(){
    struct mq_attr attr = {0};
    struct msg msg = {0};
    int client_count = 0;

    if (signal(SIGINT, sigint_handler) == SIG_ERR){
        perror("signal");
        exit(1);
    }

    struct msg history[BUFFER_SIZE] = {0};

    attr.mq_flags = 0;
    attr.mq_maxmsg = NAME_SIZE;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;

    char client_queues[MAX_CLIENTS][NAME_SIZE];
    mqd_t client_mq[MAX_CLIENTS];
    mqd_t mq = mq_open("/server", O_CREAT | O_RDWR | O_NONBLOCK, 0666, &attr);
    if (mq == -1){
        perror("mq_open");
        exit(1);
    }

    int count_of_msg = 0;

    while(flag){
        if (mq_receive(mq, &msg, MSG_SIZE, NULL) == -1){
            sleep(0.1);
            continue;
        } else{
            if (msg.type == 'M') {
                history[count_of_msg] = msg;
                count_of_msg++;
                if (count_of_msg >= BUFFER_SIZE) {
                    count_of_msg = BUFFER_SIZE - 1;
                }
            }

            if (msg.type == 'J'){
                printf("User %s has joined\n", msg.name);

                snprintf(client_queues[client_count], Q_NAME, "%s", msg.q_name);
                client_mq[client_count] = mq_open(msg.q_name, O_WRONLY);
                if (client_mq[client_count] == -1){
                    perror("mq_open");
                    continue;
                }

                int new_index = client_count;
                client_count++;

                for (int i = 0; i < count_of_msg; i++) {
                    mq_send(client_mq[new_index], (const char *)&history[i], sizeof(struct msg), 1);
                }

                struct msg join_msg = {0};
                join_msg.type = 'S';
                strncpy(join_msg.name, "Server", NAME_SIZE - 1);
                snprintf(join_msg.text, BUFFER_SIZE - 1, "User %s has joined", msg.name);
                for (int i = 0; i < client_count; i++) {
                    mq_send(client_mq[i], (const char *)&join_msg, sizeof(struct msg), 1);
                }
                if (client_count >= MAX_CLIENTS) {
                    return 0;
                }
            }
            if (msg.type == 'Q'){
                int found_index = -1;
                for (int i = 0; i < client_count; i++) {
                    if (strcmp(client_queues[i], msg.q_name) == 0) {
                        found_index = i;
                        break;
                    }
                }
                struct msg leave_msg = {0};
                leave_msg.type = 'S';
                strncpy(leave_msg.name, "Server", NAME_SIZE);
                snprintf(leave_msg.text, BUFFER_SIZE, "%s has left the chat", msg.name);

                for (int i = 0; i < client_count; i++) {
                    if (i != found_index) {
                        mq_send(client_mq[i], (const char *)&leave_msg, sizeof(struct msg), 1);
                    }
                }
                if (found_index != -1) {
                    mq_close(client_mq[found_index]);

                    for (int i = found_index; i < client_count - 1; i++) {
                        client_mq[i] = client_mq[i + 1];
                        strcpy(client_queues[i], client_queues[i + 1]);
                    }
                    client_count--;
                }
            }

            printf("%c, %s %s: %s\n", msg.type, msg.q_name, msg.name, msg.text);

            if (msg.type == 'M' || msg.type == 'S') {
                for (int i = 0; i < client_count; i++) {
                    mq_send(client_mq[i], (const char *)&msg, sizeof(struct msg), 1);
                }
            }
        }
    }

    mq_unlink("/server");
    mq_close(mq);

    return 0;
}
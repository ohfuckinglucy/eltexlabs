// открывает очередь, отправляет "Hi!" и ждет ответа от клиента в виде "Hello!" POSIX
#include "../include/header.h"
#include <mqueue.h>

int main(){
    mqd_t mq;
    struct mq_attr attr = {0};

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = BUFFER_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open("/queue", O_CREAT | O_RDWR, 0666, &attr);

    char msg[BUFFER_SIZE];
    char msg_inc[BUFFER_SIZE];

    strncpy(msg, "Hi!", BUFFER_SIZE);
    msg[BUFFER_SIZE - 1] = '\0';


    mq_send(mq, msg, BUFFER_SIZE, 0);
    sleep(2);
    mq_receive(mq, msg_inc, BUFFER_SIZE, NULL);

    printf("Полученное сообщение: %s\n", msg_inc);

    mq_close(mq);
    mq_unlink("/queue");

    return 0;
}
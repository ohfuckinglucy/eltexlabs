// открывает очередь, отправляет "Hi!" и ждет ответа от клиента в виде "Hello!" SYSTEM V
#include "../include/header.h"
#include <sys/msg.h>

int main(){
    struct msgbuf msg = {0};
    struct msgbuf msg_inc = {0};
    msg_inc.mtype = 2;
    msg.mtype = 1;
    strncpy(msg.mtext, "Hi!", BUFFER_SIZE);

    int msqid = msgget(KEY, 0666 | IPC_CREAT);
    if (msqid == -1){
        perror("mssget");
        exit(1);
    }

    msgsnd(msqid, &msg, sizeof(msg.mtext), 0);

    if (msgrcv(msqid, &msg_inc, sizeof(msg_inc.mtext), msg_inc.mtype, 0) == -1){
        perror("msgrcv");
        exit(1);
    }
    printf("Полученное сообщение тип %ld: %s\n", msg_inc.mtype, msg_inc.mtext);

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID");
        exit(1);
    }

    return 0;
}
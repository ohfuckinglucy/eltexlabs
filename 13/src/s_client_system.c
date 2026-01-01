#include "../include/header.h"
#include <sys/msg.h>

int main(){
    struct msgbuf msg = {0};
    struct msgbuf msg_inc = {0};
    msg_inc.mtype = 1;
    msg.mtype = 2;
    strcpy(msg.mtext, "Hello!");

    int msqid = msgget(KEY, 0);
    if (msqid == -1){
        perror("mssget");
        exit(1);
    }

    if (msgrcv(msqid, &msg_inc, sizeof(msg_inc.mtext), msg_inc.mtype, 0) == -1){
        perror("msgrcv");
        exit(1);
    }

    printf("Полученное сообщение тип %ld: %s\n", msg_inc.mtype, msg_inc.mtext);

    msgsnd(msqid, &msg, sizeof(msg.mtext), 0);

    return 0;
}
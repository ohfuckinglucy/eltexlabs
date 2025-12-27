#include "header.h"
#include <string.h>

int main(){
    int fd[2]; // fd[0] - чтение, fd[1] - запись
    pid_t pid;
    char buffer[BUFFER_SIZE];
    

    if (pipe(fd) == -1){ // Создаем канал
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid < 0){
        perror("fork");
        exit(1);
    }

    if (pid != 0){
        int status;
        
        close(fd[0]);
        write(fd[1], "Hi!", 3);
        printf("Родительский процесс отправил сообщение!\n");
        close(fd[1]);

        wait(&status);
    } else {
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("Дочерний процесс получил: %s\n", buffer);
        close(fd[0]);
        exit(0);
    }

    return 0;
}
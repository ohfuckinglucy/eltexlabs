#include "header.h"
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int fd;
    char *msg = "Hi!";

    mkfifo(PATH, O_WRONLY);
    
    fd = open(PATH, O_WRONLY);
    if (fd == -1){
        perror("fd open");
        return 1;
    }

    write(fd, msg, 3);

    close(fd);

    return 0;
}
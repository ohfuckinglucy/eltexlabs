#include "header.h"
#include <fcntl.h>

int main(){
    int fd;
    char buf[BUFFER_SIZE];

    fd = open(PATH, O_RDONLY);
    if (fd == -1){
        perror("fd open");
        return 1;
    }

    read(fd, buf, 3);

    printf("%s\n", buf);

    return 0;
}
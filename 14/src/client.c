#include "../include/header.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    pthread_t input_thread;

    const char *name_shm = SHM_NAME;
    const char *name_sem = SEM_NAME;
    void *ptr_shm;
    sem_t *ptr_sem;
    size_t size = sizeof(struct chat_room);

    int fd_shm = shm_open(name_shm, O_RDWR, 0666);
    if (fd_shm == -1){
        perror("shm_open");
        exit(1);
    }

    ptr_sem = sem_open(name_sem, 0);
    if(ptr_sem == SEM_FAILED){
        perror("sem_open");
        exit(1);
    }
    
    ptr_shm = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    if (ptr_shm == MAP_FAILED){
        perror("mmap");
        close(fd_shm);
        exit(1);
    }
    
    struct chat_room *room = (struct chat_room *)ptr_shm;
    
    initscr();
    refresh();
    clear();
    cbreak();
    noecho();
    
    int row, col;
    getmaxyx(stdscr, row, col);
    
    char username[NAME_SIZE] = {0};
    get_username(username);
    
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    
    WINDOW *chat = newwin(row - 3, col - 2, 1, 1);
    WINDOW *input = newwin(1, col - 2, row - 2, 1);
    
    wrefresh(chat);
    char buffer[BUFFER_SIZE] = {0};

    struct thread_args args = {0};

    args.ptr_sem = ptr_sem;
    args.chat = chat;
    args.room = room;
    args.state = 0;
    
    pthread_create(&input_thread, NULL, pthread_chat, &args);
    
    while(1){
        input_txt(buffer, input);
        sem_wait(ptr_sem);
        if (room->count < MAX_MSGS) {
            if (args.state == 0){
                room->messages[room->count].type = 'J';
                args.state = 1;
            } else if (strncmp(buffer, "exit", 5) == 0){
                room->messages[room->count].type = 'Q';
                buffer[0] = '\0';
                strncpy(room->messages[room->count].name, username, NAME_SIZE);
                strncpy(room->messages[room->count].text, buffer, BUFFER_SIZE);
                room->count++;
                args.state = 2;
                sem_post(ptr_sem);
                break;
            } else {
                room->messages[room->count].type = 'M';
            }
            strncpy(room->messages[room->count].name, username, NAME_SIZE);
            strncpy(room->messages[room->count].text, buffer, BUFFER_SIZE);
            room->count++;
        }
        sem_post(ptr_sem);
        
        refresh();
    }

    pthread_join(input_thread, NULL);
    
    refresh();
    // getch();
    endwin();

    close(fd_shm);
    sem_close(ptr_sem);

    return 0;
}
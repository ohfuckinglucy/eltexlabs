#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_FILE_COUNT 1024
#define FILE_NAME 512
#define MAX_PATH 1024

struct cur_path{
    char files[MAX_FILE_COUNT][FILE_NAME];
    int file_counts;
    int selected;
    int offset;
    char path[MAX_PATH];
};

void scan_dir(struct cur_path *left_panel);
int input_win(WINDOW *win, char *buffer, int buf_size, const char *msg);
void create_file(struct cur_path *win, int is_dir);
void show_dir(WINDOW *win, struct cur_path *left_panel);
int isdir(const char *filename);
void change_dir(struct cur_path *left_panel);
int rmrf(const char *path);
void del_file(struct cur_path *panel);
void copy_or_move(const char *src, const char *dest, int is_move);
void handle_copy_move(int is_move, struct cur_path *src_panel, struct cur_path *dest_panel);
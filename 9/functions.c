#include "header.h"

void scan_dir(struct cur_path *left_panel, const char *path){
    memset(left_panel, 0, sizeof(struct cur_path));

    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        exit(1);
    }
    struct dirent *entry;

    int i = 0;

    while ((entry = readdir(dir)) != NULL && i < MAX_FILE_COUNT){
        if ((strcmp(entry->d_name, ".")) == 0){
            continue;
        }
        strncpy(left_panel->files[i], entry->d_name, FILE_NAME - 1);
        left_panel->files[i][FILE_NAME - 1] = '\0';
        ++i;
    }
    left_panel->file_counts = i;
    left_panel->selected = 0;
    left_panel->offset = 0;
    
    closedir(dir);
}

int input_win(WINDOW *win, char *buffer, int buf_size, const char *msg){
    int row, col;
    getmaxyx(stdscr, row, col);

    int h = 4, w = 40;
    int y = row /2 - h -2;
    int x = col /2 - w -2;

    WINDOW *input_win = newwin(h, w, y, x);
    box(input_win, 0, 0);
    mvwprintw(input_win, 1, 2, "%s ", msg);
    wrefresh(input_win);

    echo();
    int res = wgetnstr(input_win, buffer, buf_size - 1);
    noecho();

    delwin(input_win);
    refresh();

    return (res == 0 && buffer[0] != '\0');
}

void create_file(struct cur_path *win, int is_dir){
    char name[FILE_NAME];
    if (!input_win(stdscr, name, sizeof(name), is_dir ? "Folder name: " : "File name: ")){
        return;
    }

    if (is_dir){
        if (mkdir(name, 0755) != 0){
            return;
        } 
    } else{
        int fd = open(name, O_CREAT | O_WRONLY, 0644);
        if (fd == -1){
            perror("fd");
            return;
        }
        close(fd);
    }

    scan_dir(win, ".");
}

int isdir(const char *filename){
    struct stat st;

    if (lstat(filename, &st) != 0){
        return 0;
    }
    return S_ISDIR(st.st_mode);
}

void show_dir(WINDOW *win, struct cur_path *left_panel){
    int win_h = getmaxy(win) - 2;
    if (win_h <= 0) return;

    for (int i = 0; i < win_h && (left_panel->offset + i) < left_panel->file_counts; i++){
        int idx = left_panel->offset + i;

        if (isdir(left_panel->files[idx])){
            mvwprintw(win, 1 + i, 2, "[DIR]  %s", left_panel->files[idx]);
        } else{
            mvwprintw(win, 1 + i, 2, "[FILE] %s", left_panel->files[idx]);
        }

        if (idx == left_panel->selected){
            wattron(win, A_REVERSE);
            mvwprintw(win, 1 + i, 2, "%s %s",
                isdir(left_panel->files[idx]) ? "[DIR] " : "[FILE]",
                left_panel->files[idx]);
            wattroff(win, A_REVERSE);
        }
    }
}


void change_dir(struct cur_path *left_panel){
    const char *sel_name = left_panel->files[left_panel->selected];

    if (sel_name[0] == '\0') return;

    if (isdir(sel_name)){
        if (chdir(sel_name) != 0){
            return;
        }
    } else {
        def_prog_mode();
        endwin();

        pid_t pid = fork();

        if (pid == 0){
            execl("/bin/nano", "nano", sel_name, (char *)NULL);
            perror("ececl nano");
            exit(1);
        } else if (pid > 0){
            int status;
            wait(&status);
        } else {
            perror("fork");
        }
        reset_prog_mode();
        refresh();
    }
}

int rmrf(const char *path){
    struct stat st;
    if (lstat(path, &st) != 0){
        return 1;
    }

    if (!S_ISDIR(st.st_mode)){
        return unlink(path);
    }

    DIR *dir = opendir(path);
    if (!dir){
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    int res = 0;

    while ((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (rmrf(full_path) != 0) {
            res = 1;
        }
    }

    closedir(dir);

    if (rmdir(path) != 0) {
        return -1;
    }

    return res;
}

void del_file(struct cur_path *panel){
    if (panel->file_counts == 0 || panel->selected >= panel->file_counts) {
        return;
    }

    const char *name = panel->files[panel->selected];
    if (name[0] == '\0' || strcmp(name, "..") == 0) {
        return;
    }

    char confirm[10];
    if (!input_win(stdscr, confirm, sizeof(confirm), "Delete? (y/n):")) {
        return;
    }
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        return;
    }

    int success;
    if (isdir(name)) {
        success = (rmrf(name) == 0);
    } else {
        success = (unlink(name) == 0);
    }

    if (!success) {
        input_win(stdscr, confirm, sizeof(confirm), "Error deleting!");
    }

    scan_dir(panel, ".");
}

void copy_or_move(const char *src, const char *dest, int is_move){
    def_prog_mode();
    endwin();

    pid_t pid = fork();
    if (pid == 0){
        if (is_move){
            execl("/bin/mv", "mv", src, dest, (char*)NULL);
        } else {
            execl("/bin/cp", "cp", src, dest, (char*)NULL);
        }
        perror("execl");
        exit(1);
    } else if (pid > 0){
        int status;
        wait(&status);
    } else {
        perror("fork");
        exit(1);
    }

    reset_prog_mode();
    refresh();
}

void handle_copy_move(int is_move, struct cur_path *src_panel, struct cur_path *dest_panel){
    if (src_panel->file_counts == 0 || src_panel->selected >= src_panel->file_counts)
        return;

    const char *src_name = src_panel->files[src_panel->selected];
    if (src_name[0] == '\0' || strcmp(src_name, "..") == 0)
        return;

    const char *dest_name = src_name;

    char prompt[2048];
    snprintf(prompt, sizeof(prompt), "%s '%s' -> '%s'? (y/n)",
             is_move ? "Move" : "Copy", src_name, dest_name);

    char confirm[10];
    if (!input_win(stdscr, confirm, sizeof(confirm), prompt))
        return;
    if (confirm[0] != 'y' && confirm[0] != 'Y')
        return;

    copy_or_move(src_name, dest_name, is_move);

    scan_dir(src_panel, ".");
    scan_dir(dest_panel, ".");
}
#include "header.h"

int isdir(const char *filename){
    struct stat st;

    if (lstat(filename, &st) != 0){
        return 0;
    }
    return S_ISDIR(st.st_mode);
}

void scan_dir(struct cur_path *panel){
    DIR *dir = opendir(panel->path);
    if (!dir) {
        perror("opendir");
        fprintf(stderr, "Failed to open: '%s'\n", panel->path);
        exit(1);
    }

    struct dirent *entry;
    int i = 0;

    while ((entry = readdir(dir)) != NULL && i < MAX_FILE_COUNT){
        if (strcmp(entry->d_name, ".") == 0) continue;
        strncpy(panel->files[i], entry->d_name, FILE_NAME - 1);
        panel->files[i][FILE_NAME - 1] = '\0';
        i++;
    }

    panel->file_counts = i;
    panel->selected = 0;
    panel->offset = 0;
    closedir(dir);
}

void show_dir(WINDOW *win, struct cur_path *panel){
    int win_h = getmaxy(win) - 2;
    if (win_h <= 0) return;

    for (int i = 0; i < win_h && (panel->offset + i) < panel->file_counts; i++){
        int idx = panel->offset + i;
        const char *name = panel->files[idx];
        char full[MAX_PATH];
        snprintf(full, sizeof(full), "%s/%s", panel->path, name);
        const char *prefix = isdir(full) ? "[DIR] " : "[FILE] ";

        if (idx == panel->selected){
            wattron(win, A_REVERSE);
        }

        mvwprintw(win, 1 + i, 2, "%s%s", prefix, name);

        if (idx == panel->selected){
            wattroff(win, A_REVERSE);
        }
    }
}

void change_dir(struct cur_path *panel) {
    if (panel->file_counts == 0 || panel->selected >= panel->file_counts)
        return;

    const char *name = panel->files[panel->selected];
    if (name[0] == '\0')
        return;

    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s/%s", panel->path, name);

    if (!isdir(full_path)) {
        def_prog_mode();
        endwin();

        pid_t pid = fork();
        if (pid == 0) {
            execl("/bin/nano", "nano", full_path, (char *)NULL);
            _exit(1);
        } else if (pid > 0) {
            int status;
            wait(&status);
        }

        reset_prog_mode();
        refresh();
        return;
    }

    if (strcmp(name, "..") == 0) {
        if (strcmp(panel->path, "/") == 0) {
            return;
        }
        char *last_slash = strrchr(panel->path, '/');
        if (last_slash && last_slash > panel->path) {
            *last_slash = '\0';
        } else {
            strcpy(panel->path, "/");
        }
        scan_dir(panel);
        return;
    }

    if (strlen(panel->path) + 1 + strlen(name) >= MAX_PATH) {
        return;
    }
    strcat(panel->path, "/");
    strcat(panel->path, name);
    scan_dir(panel);
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

void create_file(struct cur_path *panel, int is_dir) {
    char name[FILE_NAME];
    if (!input_win(stdscr, name, sizeof(name), is_dir ? "Folder:" : "File:"))
        return;

    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s/%s", panel->path, name);

    if (is_dir){
        mkdir(full_path, 0755);
    } else{
        int fd = open(full_path, O_CREAT | O_WRONLY, 0644);
        if (fd != -1) close(fd);
    }

    scan_dir(panel);
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
    if (panel->file_counts == 0 || panel->selected >= panel->file_counts){
        return;
    }

    const char *name = panel->files[panel->selected];
    if (name[0] == '\0' || strcmp(name, "..") == 0){
        return;
    }

    char confirm[10];
    if (!input_win(stdscr, confirm, sizeof(confirm), "Delete? (y/n):")){
        return;
    }
    if (confirm[0] != 'y' && confirm[0] != 'Y'){
        return;
    }

    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s/%s", panel->path, name);

    int success;
    if (isdir(full_path)){
        success = (rmrf(full_path) == 0);
    } else{
        success = (unlink(full_path) == 0);
    }

    if (!success){
        input_win(stdscr, confirm, sizeof(confirm), "Error deleting!");
    }

    scan_dir(panel);
}

void copy_or_move(const char *src_full, const char *dest_full, int is_move) {
    def_prog_mode(); endwin();

    pid_t pid = fork();
    if (pid == 0){
        if (is_move){
            execl("/bin/mv", "mv", src_full, dest_full, (char*)NULL);
        } else{
            execl("/bin/cp", "cp", "-r", src_full, dest_full, (char*)NULL);
        }
        _exit(1);
    } else if (pid > 0){
        int status; wait(&status);
    }

    reset_prog_mode(); refresh();
}

void handle_copy_move(int is_move, struct cur_path *src_panel, struct cur_path *dest_panel){
    if (src_panel->file_counts == 0 || src_panel->selected >= src_panel->file_counts)
        return;

    const char *src_name = src_panel->files[src_panel->selected];
    if (src_name[0] == '\0' || strcmp(src_name, "..") == 0)
        return;

    const char *dest_name = src_name;

    char src_full[MAX_PATH], dest_full[MAX_PATH];
    snprintf(src_full, sizeof(src_full), "%s/%s", src_panel->path, src_name);
    snprintf(dest_full, sizeof(dest_full), "%s/%s", dest_panel->path, src_name);

    char prompt[2048];
    snprintf(prompt, sizeof(prompt), "%s '%s' -> '%s'? (y/n)",
             is_move ? "Move" : "Copy", src_name, dest_name);

    char confirm[10];
    if (!input_win(stdscr, confirm, sizeof(confirm), prompt))
        return;
    if (confirm[0] != 'y' && confirm[0] != 'Y')
        return;

    copy_or_move(src_full, dest_full, is_move);

    scan_dir(src_panel);
    scan_dir(dest_panel);
}
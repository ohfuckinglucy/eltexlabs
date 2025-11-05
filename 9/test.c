#include "header.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

void scan_dir(struct Panel *panel){
    DIR *dir;
    struct dirent *cur;
    struct stat file_info;
    char full_path[MAX_PATH];

    dir = opendir(panel->current_path);
    if (dir == NULL){
        perror("opendir");
        return;
    }

    panel->file_count = 0;

    while((cur = readdir(dir)) != NULL){
        if ((strcmp(cur->d_name, ".")) == 0){
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", panel->current_path, cur->d_name);

        if (stat(full_path, &file_info) != 0) {
            continue;
        }
        if (panel->file_count >= MAX_FILES) {
            break;
        }

        struct FileInfo *file = &panel->files[panel->file_count];

        strncpy(file->name, cur->d_name, sizeof(file->name) - 1);
        file->name[sizeof(file->name) - 1] = '\0';

        file->size = file_info.st_size;
        file->mtime = file_info.st_mtime;
        file->is_dir = S_ISDIR(file_info.st_mode);

        panel->file_count++;
    }

    closedir(dir);
}

void draw_box(WINDOW *win, struct Panel panel, int height, int width) {
    if (win == NULL) return;

    werase(win);
    wattron(win, COLOR_PAIR(3));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(3));

    int max_y = getmaxy(win);
    int max_x = getmaxx(win);
    int y = 1;

    int left_margin = 1;
    int name_width = 20;
    int type_col = left_margin + name_width;

    for (int i = 0; i < panel.file_count; i++) {
        if (y >= max_y - 1) {
            mvwprintw(win, y, left_margin, "...");
            break;
        }

        const char *name = panel.files[i].name;
        const char *type = panel.files[i].is_dir ? "[DIR] " : "[FILE]";

        if (i == panel.selected) {
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, y, left_margin, "> %-*s", name_width - 2, name);
            wattroff(win, COLOR_PAIR(2));
        } else {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, y, left_margin + 2, "%-*s", name_width - 2, name);
            wattroff(win, COLOR_PAIR(1));
        }

        if (type_col < max_x - 15 && (panel.files[i].is_dir) != 1) {
            mvwprintw(win, y, type_col, "%s | size: %ld", type, panel.files[i].size);
        } else if (type_col < max_x - 15 && (panel.files[i].is_dir)){
            mvwprintw(win, y, type_col, "%s |", type);
        }

        y++;
    }
    wattron(win, COLOR_PAIR(3));
    char status[256];
    snprintf(status, sizeof(status), "Selected: %s", panel.files[panel.selected].name);
    int status_len = strlen(status);
    if (status_len < (size_t)max_x - 2) {
        int status_x = (max_x - status_len) / 2;
        if (status_x < 1) status_x = 1;
        mvwprintw(win, max_y - 1, status_x, "%s", status);
    }

    wrefresh(win);
}

int del_dir(const char *path){
    DIR *dir;

    struct stat file_info;
    struct dirent *cur;

    dir = opendir(path);
    if (dir == NULL){
        perror("opendir");
        return 1;
    }

    while ((cur = readdir(dir)) != NULL){
        if (strcmp(cur->d_name, ".") == 0 || strcmp(cur->d_name, "..") == 0) {
            continue;
        }
        char del_path[MAX_PATH];
        snprintf(del_path, sizeof(del_path), "%s/%s", path, cur->d_name);

        if (stat(del_path, &file_info) != 0){
            continue;
        }

        if (S_ISDIR(file_info.st_mode)){
            del_dir(del_path);
        } else {
            if (unlink(del_path) != 0){
                perror("unlink");
                return 1;
            }
        }
    }

    closedir(dir);

    if(rmdir(path) != 0){
        perror("rmdir");
    }

    return 0;
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

    int height = 20, width = 45;

    WINDOW *panel1 = newwin(height, width, 0, 0);

    struct Panel panel;
    panel.selected = 0;

    strcpy(panel.current_path, ".");

    scan_dir(&panel);

    int ch;

    while((ch = getch()) != KEY_F(10)) {
        switch(ch){
            case KEY_UP: {
                if (panel.selected > 0){
                    panel.selected--;
                }
                break;
            }

            case KEY_DOWN: {
                if (panel.selected < panel.file_count - 1){
                    panel.selected++;
                }
                break;
            }

            case 10: {
                struct FileInfo *file = &panel.files[panel.selected];

                if (file->is_dir) {
                    char new_path[MAX_PATH];
                    snprintf(new_path, sizeof(new_path), "%s/%s", panel.current_path, file->name);
                    strcpy(panel.current_path, new_path);
                    scan_dir(&panel);
                    panel.selected = 0;
                } else {
                    char full_path[MAX_PATH];
                    snprintf(full_path, sizeof(full_path), "%s/%s", panel.current_path, file->name);

                    endwin();

                    pid_t pid = fork();
                    if (pid == 0) {
                        execlp("nano", "nano", full_path, NULL);
                        exit(1);
                    } else if (pid > 0) {
                        int status;
                        wait(&status);
                    }

                    initscr();
                    cbreak();
                    noecho();
                    keypad(stdscr, TRUE);
                    curs_set(0);

                    scan_dir(&panel);
                }
                break;
            }
            case KEY_F(5): {
                struct FileInfo *file = &panel.files[panel.selected];

                char del_path[MAX_PATH];
                snprintf(del_path, sizeof(del_path), "%s/%s", panel.current_path, file->name);

                int popup_height = 5;
                int popup_width = 50;
                int start_y = (LINES - popup_height) / 2;
                int start_x = (COLS - popup_width) / 2;

                WINDOW *popup = newwin(popup_height, popup_width, start_y, start_x);
                box(popup, 0, 0);

                mvwprintw(popup, 1, 2, "Delete file?");
                mvwprintw(popup, 2, 2, "Name: %s", file->name);
                mvwprintw(popup, 3, 2, "Press 'y' for yes, 'n' for no");

                wrefresh(popup);

                int choice = wgetch(popup);

                if (choice == 'y' || choice == 'Y') {
                    if (panel.files[panel.selected].is_dir){
                        if (del_dir(del_path) == 0) {
                            mvwprintw(popup, 4, 2, "Deleted!");
                            char *last_slash = strrchr(panel.current_path, '/');
                            if (last_slash && last_slash != panel.current_path) {
                                *last_slash = '\0';
                            } else {
                                strcpy(panel.current_path, ".");
                            }
                            wrefresh(popup);
                            sleep(1);
                        } else {
                            mvwprintw(popup, 4, 2, "Error: %s", strerror(errno));
                            wrefresh(popup);
                            sleep(2);
                        }
                    } else {
                        unlink(del_path);
                        mvwprintw(popup, 4, 2, "Deleted!");
                            wrefresh(popup);
                            sleep(1);
                    }
                } else {
                    mvwprintw(popup, 4, 2, "Cancelled.");
                    wrefresh(popup);
                    sleep(1);
                }
                wrefresh(popup);
                delwin(popup);
                refresh();

                scan_dir(&panel);
                break;
            }
        }
        draw_box(panel1, panel, height, width);
    }
    delwin(panel1);
    endwin();
}
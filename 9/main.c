#include "header.h"

int main(){
    struct cur_path left_panel = {0};
    struct cur_path right_panel = {0};

    int state = 0;

    left_panel.selected = 0;
    
    initscr();
    refresh();
    clear();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    
    int row, col;
    getmaxyx(stdscr, row, col);

    int panel_width = col / 2 - 1;
    
    WINDOW *tab1 = newwin(row - 4, panel_width, 1, 1);
    WINDOW *tab2 = newwin(row - 4, panel_width, 1, col / 2 + 1);
    
    int tab_h = getmaxy(tab1) - 2;
    if (tab_h < 1) tab_h = 1;

    char init_path[MAX_PATH];
    if (!getcwd(init_path, sizeof(init_path))) {
        perror("getcwd");
        endwin();
        return 1;
    }

    strncpy(left_panel.path, init_path, MAX_PATH - 1);
    left_panel.path[MAX_PATH - 1] = '\0';
    strncpy(right_panel.path, init_path, MAX_PATH - 1);
    right_panel.path[MAX_PATH - 1] = '\0';

    scan_dir(&left_panel);
    scan_dir(&right_panel);

    while(1){
        int key = getch();

        struct cur_path *cur_panel = (state == 0) ? &left_panel : &right_panel;
        WINDOW *current_win = (state == 0) ? tab1 : tab2;

        switch (key) {
            case 9:
                state = 1 - state;
                break;
            case KEY_UP:
                if (cur_panel->selected > 0) {
                    cur_panel->selected--;
                    if (cur_panel->selected < cur_panel->offset)
                        cur_panel->offset = cur_panel->selected;
                }
                break;
            case KEY_DOWN:
                if (cur_panel->selected < cur_panel->file_counts - 1) {
                    cur_panel->selected++;
                    int h = getmaxy(current_win) - 2;
                    if (h > 0 && cur_panel->selected >= cur_panel->offset + h)
                        cur_panel->offset = cur_panel->selected - h + 1;
                }
                break;
            case 10:
                change_dir(cur_panel);
                scan_dir(cur_panel);
                break;
            case 'a':
            case 'A':
                char choice;
                if (input_win(stdscr, &choice, 2, "f=File, d=Folder")){
                    if (choice == 'd' || choice == 'D'){
                        create_file(cur_panel, 1);
                    } else if (choice == 'f' || choice == 'F'){
                        create_file(cur_panel, 0);
                    }
                }
                break;
            case 'd':
            case 'D':
                del_file(cur_panel);
                scan_dir(cur_panel);
                break;
            case 'c':
            case 'C': {
                struct cur_path *dest = (state == 0) ? &right_panel : &left_panel;
                struct cur_path *src = (state == 0) ? &left_panel : &right_panel;
                handle_copy_move(0, src, dest);
                break;
            }
            case 'm':
            case 'M': {
                struct cur_path *dest = (state == 0) ? &right_panel : &left_panel;
                struct cur_path *src = (state == 0) ? &left_panel : &right_panel;
                handle_copy_move(1, src, dest);
                break;
            }
            case 'q':
                endwin();
                return 0;
        }

        werase(tab1);
        show_dir(tab1, &left_panel);
        wborder(tab1, 0, 0, 0, 0, 0, 0, 0, 0);
        wrefresh(tab1);

        werase(tab2);
        show_dir(tab2, &right_panel);
        wborder(tab2, 0, 0, 0, 0, 0, 0, 0, 0);
        wrefresh(tab2);

        mvprintw(0, 2, "Path: %s", cur_panel->path);
        clrtoeol();

        mvprintw(row - 3, 3, "TAB-Switch  A-Create file  D-Delete file  C-Copy  M-Move  ENTER-Open  Q-Quit");
        refresh();
        
    }
    refresh();
    endwin();

    return 0;
}
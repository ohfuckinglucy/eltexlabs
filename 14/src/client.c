#include "../include/header.h"

int main(){
    initscr();

    int row, col;
    getmaxyx(stdscr, row, col);

    move(row - 1, 0);

    printw("Hello world");
    refresh();
    getch();

    endwin();
    return 0;
}
/*************************************************************************
  > FileName: ncurses-2.c
  > Author  : DingJing
  > Mail    : dingjing@live.cn
  > Created Time: Fri 13 Jan 2023 01:53:25 PM CST
 ************************************************************************/

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int i = 0;

    initscr();

    if (FALSE == has_colors()) {
        addstr("Do not support colors\n.");
        sleep(1);
        exit(EXIT_FAILURE);
    }

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_RED,   COLOR_WHITE);
    init_pair(3, COLOR_GREEN, COLOR_WHITE);
    init_pair(4, COLOR_YELLOW,COLOR_WHITE);
    init_pair(5, COLOR_BLUE,  COLOR_WHITE);
    init_pair(6, COLOR_MAGENTA,COLOR_WHITE);
    init_pair(7, COLOR_CYAN,  COLOR_WHITE);
    init_pair(8, COLOR_WHITE, COLOR_WHITE);

    for (i = 1; i < 9; i++) {
        attron(COLOR_PAIR(i));
        printw("Test color No.%d\n", i);
        attroff(COLOR_PAIR(i));
        refresh();
        sleep(1);
    }

    getch();
    endwin();

    return 0;
} 

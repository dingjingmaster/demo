/*************************************************************************
> FileName: ncurses-1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 13 Jan 2023 01:50:04 PM CST
 ************************************************************************/
#include <stdlib.h>
#include <curses.h>

int main (void)
{
    initscr();

    printw ("Hello World!!!\n");

    refresh ();

    getch();

    endwin();

    return 0;
}

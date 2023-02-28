/*************************************************************************
> FileName: ncurses-window1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 22 Feb 2023 09:17:44 AM CST
 ************************************************************************/
#include <stdlib.h>
#include <curses.h>

static int gW = 0;
static int gH = 0;

int main (int argc, char* argv[])
{
    initscr ();

    ///////////////////////////////////////





    ///////////////////////////////////////
    refresh ();

    getch ();

    endwin ();

    return 0;
}

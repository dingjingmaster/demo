/*************************************************************************
  > FileName: ncurses-3.c
  > Author  : DingJing
  > Mail    : dingjing@live.cn
  > Created Time: Fri 13 Jan 2023 01:55:22 PM CST
 ************************************************************************/

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    WINDOW *w0, * w1, *w2;

    initscr();

    // newwin 创建新窗口
    w0 = newwin(20, 80, 1, 1);
    box(w0, '|', '-');
    wrefresh(w0);
    wgetch(w0);

    // derwin 和 subwin 建立子窗口
    // 其中subwin的位置相对于屏幕来说
    // derwin相对于父窗口来说
    w1 = derwin(w0, 4, 4, 5, 5);
    box(w1, '*', '*');
    wrefresh(w1);
    wgetch(w1);

    w2 = derwin(w0, 3, 3, 15, 16);
    box(w2, '#', '#');
    wrefresh(w2);
    wgetch(w2);

    delwin(w2);
    delwin(w1);
    delwin(w0);

    endwin();

    return 0; 
}

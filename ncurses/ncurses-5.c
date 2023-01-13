/*************************************************************************
> FileName: ncurses-5.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 13 Jan 2023 01:50:04 PM CST
 ************************************************************************/
#include <stdlib.h>
#include <curses.h>

int main (void)
{
    // 开启 curses 模式
    initscr();

    //cbreak();
    raw();

    // 打印 Hello Wold
    printw ("Hello World!!!\n");

    // 将字符串打印到真实屏幕上
    refresh ();

    getch();

    // 关闭 curses 模式
    endwin();

    return 0;
}

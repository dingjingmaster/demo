/*************************************************************************
  > FileName: ncurses-4.c
  > Author  : DingJing
  > Mail    : dingjing@live.cn
  > Created Time: Fri 13 Jan 2023 01:59:30 PM CST
 ************************************************************************/

#include <stdlib.h>
#include <curses.h>

int main(void)
{
    SCREEN * scr;

    if (NULL == (scr = newterm(NULL, stdout, stdin))) {
        perror("Failed on newterm\n");
        exit(EXIT_FAILURE);
    }

    if (NULL == set_term(scr)) {
        perror("failed on set_term\n");
        endwin();
        delscreen(scr);
        exit(EXIT_FAILURE);
    }

    printw("Test newterm...\n");
    refresh();
    getch();

    endwin();
    delscreen(scr);

    return 0;
}

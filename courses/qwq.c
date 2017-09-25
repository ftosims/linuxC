#include <string.h>
#include <stdio.h>
#include "ui.c"


int
main()
{
    initscr();
    int xa,ya;
    getmaxyx(stdscr, ya, xa);
    WINDOW *local_win;

    getchar();
    endwin();
    return 0;
}

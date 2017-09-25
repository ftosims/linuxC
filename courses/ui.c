#include <ncurses.h>


//创建一个可指定大小位置的窗口

int
winlist(int x, int y, int startx, int starty)
{
    WINDOW *cur_win;
    cur_win = newwin(y, x, starty, startx);
    wborder(win, ' ', '|', ' ', ' ', ' ', ' ', '|', '|');
    wrefresh(local_win);
    return cur_win;
}

int
wintype(int x, int y, int startx, int starty)
{
    WINDOW *cur_win;
    cur_win = newwin(y, x, starty, startx);
    wborder(win, '|', '|', ' ', ' ', '|', '|', '|', '|');
    wrefresh(local_win);
    return cur_win;
}
int
wincurrent(int x, int y, int startx, int starty)
{
    WINDOW *cur_win;
    cur_win = newwin(y, x, starty, startx);
    wborder(win, '|', ' ', ' ', ' ', '|', '|', ' ', ' ');
    wrefresh(local_win);
    return cur_win;
}

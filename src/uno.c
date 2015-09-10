#include <curses.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "window.h"

static void init_screen() {
    //(void) signal(SIGINT, finish);      /* arrange interrupts to terminate */
    (void) initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) cbreak();       /* take input chars one at a time, don't wait for \n */
    (void) noecho();       /* do not echo input */

    if (has_colors()) {

        start_color();
        // initialise you color pairs (foreground, background)
        init_pair(1,COLOR_YELLOW,COLOR_RED);
        init_pair(2,COLOR_YELLOW,COLOR_BLUE);
        init_pair(3, COLOR_BLUE, COLOR_GREEN);
        init_pair(4, COLOR_BLUE, COLOR_YELLOW);
        init_pair(5,COLOR_WHITE,COLOR_BLACK);
        init_pair(6, COLOR_BLACK, COLOR_WHITE);



    }
    bkgd(COLOR_PAIR(6));
    attrset(COLOR_PAIR(6));
}

int main()
{
    //char buffer[1024];
    MEVENT event;
    int c;
    init_screen();
    curs_set(0); /* Invisible cursor */

    //keypad(stdscr, TRUE); /* Enable keypad mode */
    mousemask(ALL_MOUSE_EVENTS, NULL); /* Report all mouse events */

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int maxx, maxy;
    //WINDOW *menu = newwin(w.ws_row,w.ws_col,0,0);
    //wbkgd(menu,COLOR_PAIR(1));
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, maxy, maxx);
    setMenuScreen(stdscr, maxy, maxx);

    while (1) {
        c = wgetch(stdscr);

        if (KEY_MOUSE == c) {
            /* Mouse event. */
            if (OK == getmouse(&event)) {
                if(event.y == maxy/2-3 && (event.x >= maxx/2-2 && event.x <= maxx/2+2)){
                    mvwprintw(stdscr, 1, 1, "You clicked Menu");
                }
                else if(event.y == maxy/2-1 && (event.x >= maxx/2-4 && event.x <= maxx/2+4)){
                    gameScreen(w.ws_row,w.ws_col);
                    clear();
                    refresh();
                    setMenuScreen(stdscr, maxy, maxx);
                }
                else if(event.y == maxy/2+1 && (event.x >= maxx/2-3 && event.x <= maxx/2+3)){
                    creditScreen(w.ws_row,w.ws_col);
                    clear();
                    refresh();
                    setMenuScreen(stdscr, maxy, maxx);

                }
                else if(event.y == maxy/2+3 && (event.x >= maxx/2-2 && event.x <= maxx/2+2)){
                    endwin();
                    return 0;
                }
            }
        }
    }
}



#include <curses.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "window.h"
void init_screen() {
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

    mousemask(ALL_MOUSE_EVENTS, NULL); /* Report all mouse events */


    keypad(stdscr, TRUE);

    setMenuScreen(stdscr, LINES, COLS);

    while (1) {
        c = wgetch(stdscr);

        if (KEY_MOUSE == c) {
            /* Mouse event. */
            if (OK == getmouse(&event)) {
                if(event.y == LINES/2-3 && (event.x >= COLS/2-4 && event.x <= COLS/2+4)){
                    gameScreen(LINES,COLS,0);
                    clear();
                    refresh();
                    setMenuScreen(stdscr, LINES, COLS);
                }
                else if(event.y == LINES/2-1 && (event.x >= COLS/2-4 && event.x <= COLS/2+4)){
                    char fname[1000];
                    snprintf(fname, sizeof(fname), "saves/uno.save");
                    if( access( fname, F_OK ) != -1 ) {
                        // file exists
                        gameScreen(LINES,COLS, 1);
                        clear();
                        refresh();
                        setMenuScreen(stdscr, LINES, COLS);
                    } else {
                        // file doesn't exist
                        mvprintw(3,COLS/2-12,"There is no save game!");
                    }
                }
                else if(event.y == LINES/2+1 && (event.x >= COLS/2-3 && event.x <= COLS/2+3)){
                    creditScreen(LINES,COLS);
                    clear();
                    refresh();
                    setMenuScreen(stdscr, LINES, COLS);

                }
                else if(event.y == LINES/2+3 && (event.x >= COLS/2-2 && event.x <= COLS/2+2)){
                    endwin();
                    return 0;
                }
            }
        }
        else if(c==KEY_RESIZE){
            wclear(stdscr);
            setMenuScreen(stdscr,LINES,COLS);
            wrefresh(stdscr);
        }
    }
}



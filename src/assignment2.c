//#include <stdlib.h>
//#include <curses.h>
//#include <signal.h>
//#include <menu.h>
////#include <time.h>
//
//#define UNO_COLOR_BACKGROUND    0
//#define UNO_COLOR_RED           1
//#define UNO_COLOR_BLUE          2
//#define UNO_COLOR_TEXT          7
//
//static void init_screen();
//static void finish(int sig);
//
//
//
//#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
//#define CTRLD 	4
//
//char *choices[] = {
//        "Choice 1",
//        "Choice 2",
//        "Choice 3",
//        "Choice 4",
//        "Exit",
//};
//
//int main()
//{	ITEM **my_items;
//    int c;
//    MENU *my_menu;
//    int n_choices, i;
//    ITEM *cur_item;
//
//
//    (void) initscr();
//    (void) cbreak();
//    (void) noecho();
//    keypad(stdscr, TRUE);
//
//    n_choices = ARRAY_SIZE(choices);
//    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
//
//    for(i = 0; i < n_choices; ++i)
//        my_items[i] = new_item(choices[i], choices[i]);
//    my_items[n_choices] = (ITEM *)NULL;
//
//    my_menu = new_menu((ITEM **)my_items);
//    mvprintw(LINES - 2, 0, "F1 to Exit");
//    post_menu(my_menu);
//    refresh();
//
//    while((c = getch()) != KEY_F(1))
//    {   switch(c)
//        {	case KEY_DOWN:
//                menu_driver(my_menu, REQ_DOWN_ITEM);
//                break;
//            case KEY_UP:
//                menu_driver(my_menu, REQ_UP_ITEM);
//                break;
//        }
//    }
//
//    free_item(my_items[0]);
//    free_item(my_items[1]);
//    free_menu(my_menu);
//    endwin();
//}
//
//
////static void init_screen() {
////    (void) signal(SIGINT, finish);      /* arrange interrupts to terminate */
////    (void) initscr();      /* initialize the curses library */
////    keypad(stdscr, TRUE);  /* enable keyboard mapping */
////    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
////    //(void) cbreak();       /* take input chars one at a time, don't wait for \n */
////    (void) noecho();       /* do not echo input */
////    //timeout(500);          /* wait maximum 500ms for a character */
////    /* Use timeout(-1) for blocking mode */
////
////    if (has_colors()) {
////
////        start_color();
////        // initialise you color pairs (foreground, background)
////        init_pair(1, UNO_COLOR_TEXT, UNO_COLOR_BACKGROUND);
////        init_pair(2, UNO_COLOR_RED, UNO_COLOR_BACKGROUND);
////        init_pair(3, UNO_COLOR_BLUE, UNO_COLOR_BACKGROUND);
////    }
////    /* set default color pair */
////    attrset(COLOR_PAIR(1));
////}
////
////static void finish(int sig) {
////    endwin();
////
////    /* do your non-curses wrap up here, like freeing the memory allocated */
////
////
////    exit(sig);
////}

#include <ncurses.h>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

char *choices[] = { 	"Choice 1",
                       "Choice 2",
                       "Choice 3",
                       "Choice 4",
                       "Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW *menu_win, int highlight);
void report_choice(int mouse_x, int mouse_y, int *p_choice);

int main()
{	int c, choice = 0;
    WINDOW *menu_win;
    MEVENT event;

    /* Initialize curses */
    initscr();
    clear();
    noecho();
    cbreak();	//Line buffering disabled. pass on everything

    /* Try to put the window in the middle of screen */
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    attron(A_REVERSE);
    mvprintw(23, 1, "Click on Exit to quit (Works best in a virtual console)");
    refresh();
    attroff(A_REVERSE);

    /* Print the menu for the first time */
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    print_menu(menu_win, 1);
    /* Get all the mouse events */
    mousemask(ALL_MOUSE_EVENTS, NULL);

    while(1)
    {	c = wgetch(menu_win);
        switch(c)
        {	case KEY_MOUSE:
                if(getmouse(&event) == OK)
                {	/* When the user clicks left mouse button */
                    if(event.bstate & BUTTON1_PRESSED)
                    {	report_choice(event.x + 1, event.y + 1, &choice);
                        if(choice == -1) //Exit chosen
                            goto end;
                        mvprintw(22, 1, "Choice made is : %d String Chosen is \"%10s\"", choice, choices[choice - 1]);
                        refresh();
                    }
                }
                print_menu(menu_win, choice);
                break;
        }
    }
    end:
    endwin();
    return 0;
}


void print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i)
    {	if(highlight == i + 1)
        {	wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

/* Report the choice according to mouse position */
void report_choice(int mouse_x, int mouse_y, int *p_choice)
{	int i,j, choice;

    i = startx + 2;
    j = starty + 3;

    for(choice = 0; choice < n_choices; ++choice)
        if(mouse_y == j + choice && mouse_x >= i && mouse_x <= i + strlen(choices[choice]))
        {	if(choice == n_choices - 1)
                *p_choice = -1;
            else
                *p_choice = choice + 1;
            break;
        }
}
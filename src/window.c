//
// Created by bit on 9/5/15.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "function.h"
#include "window.h"


extern Player *players;
extern Card current_card;
extern Deck *remaining_pile;
extern Deck *discard_pile;
extern int currentPosition;
extern int numPlayers;

char *nameCard[15] = {"0","1","2","3","4","5","6","7","8","9","S", "R", "+","W"};
WINDOW *line1[9];
WINDOW *line2[9];
WINDOW *playedCard;
WINDOW *game;
int startX;
int startY;
int gameX, gameY;

void setMenuScreen(WINDOW *menu, int maxy, int maxx){
    mvwprintw(menu,maxy/2-3,maxx/2-2,"Menu");
    mvwprintw(menu,maxy/2-1,maxx/2-4,"New Game");
    mvwprintw(menu,maxy/2+1,maxx/2-3,"Credit");
    mvwprintw(menu,maxy/2+3,maxx/2-2,"Quit");
}

int new_game(WINDOW *question) {
    MEVENT event;
    int x, y;
    getmaxyx(question, y, x);
    mvwprintw(question, y/2 - 1,x/2 - 18 , "How many players do you want to play?");
    mvwprintw(question, y/2 + 1, x/2 - 8, "2");
    mvwprintw(question, y/2 + 1, x/2 - 6, "3");
    mvwprintw(question, y/2 + 1, x/2 - 4, "4");
    mvwprintw(question, y/2 + 1, x/2 - 2, "5");
    mvwprintw(question, y/2 + 1, x/2 , "6");
    mvwprintw(question, y/2 + 1, x/2 + 2, "7");
    mvwprintw(question, y/2 + 1, x/2 + 4, "8");
    mvwprintw(question, y/2 + 1, x/2 + 6, "9");
    mvwprintw(question, y/2 + 1, x/2 + 8, "10");
    mvwprintw(question, y-5,x-4, "Back");
    int c;
    int returnNum = -1;
    while (1) {
        c = wgetch(question);
        if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
                if ((event.y == y - 5) && (event.x >= x - 4 && event.x < x )) {
                    delwin(question);
                    returnNum = -1;
                    break;
                } else if ((event.y == y / 2 + 1) && (event.x == x / 2 - 8)) {
                    returnNum = 2;
                    break;
                } else if ((event.y == y / 2 + 1) && (event.x == x / 2 - 6)) {
                    returnNum = 3;
                    break;;
                } else if ((event.y == y / 2 + 1) && (event.x == x / 2 - 4)) {
                    returnNum = 4;
                    break;
                } else if ((event.y == y / 2 + 1) && (event.x == x / 2 - 2)) {
                    returnNum = 5;
                    break;
                } else if ((event.y == y / 2 + 1) && (event.x == x / 2)) {
                    returnNum = 6;
                    break;
                } else if ((event.y == y / 2 + 1) && (event.x == x / 2 + 2)) {
                    returnNum = 7;
                    break;
                }
                else if ((event.y == y / 2 + 1) && (event.x == x / 2 + 4)) {
                    returnNum = 8;
                    break;
                }
                else if ((event.y == y / 2 + 1) && (event.x == x / 2 + 6)) {
                    returnNum = 9;
                    break;
                }
                else if ((event.y == y / 2 + 1) && (event.x == x / 2 + 8 || event.x == x / 2 + 9)) {
                    returnNum = 10;
                    break;
                }

            }
        }
    }
    return returnNum;
}
void gameScreen(int y, int x){
    game = newwin(y,x,0,0);
    wbkgd(game,COLOR_PAIR(6));
    keypad(game, TRUE);
    gameX = x;
    gameY = y;

    int numPlay = new_game(game);
    if(numPlay != -1){
        //char buffer[1];
        //sprintf(buffer,"%d",numPlay);
        wclear(game);
        wrefresh(game);
        playedCard = subwin(game,3,5,y/2-5,x/2-2);

        mvwprintw(game, y-5,x-4, "Back");

        startGame(numPlay);
        printCard();

        WINDOW* drawCard  = subwin(game,3,5,y/2-5,startX);
        wbkgd(drawCard,COLOR_PAIR(5));
        mvwprintw(drawCard,1,1,"UNO");
        wrefresh(drawCard);

        playGame(game,x);
        wrefresh(game);


        /*int c;
        while(1){
            c=wgetch(game);
        }*/


    }
}
void creditScreen(int y, int x) {
    MEVENT event;
    WINDOW *credit = newwin(y,x,0,0);
    wbkgd(credit,COLOR_PAIR(6));
    mvwprintw(credit, 1, 1, "Tran Thach Thao - s3494528");
    mvwprintw(credit, 2, 1, "Bui Minh Dung - s3461484");
    mvwprintw(credit, 3, 1, "COSC2541-Programing Techiniques");
    mvwprintw(credit, 4, 1, "Semester B 2015");
    mvwprintw(credit, 5, 1, "Lecturer's Name: Denis Rinfret");
    mvwprintw(credit, y-5,x-10, "Back");
    keypad(credit, TRUE);
    int c;
    while (1) {
        c = wgetch(credit);
        if  ((OK == getmouse(&event))&&(event.y==y-5)&&(event.x>=x-10&&event.x<x-5)) {
            wrefresh(credit);
            wclear(credit);
            delwin(credit);
            break;
        }
    }
}

void startGame(int num)
{
    //time_t current_time = time(NULL);
    //srand(current_time);

    int canRun = create_cards();
    if (canRun == 0){
        canRun = shuffle(108);
        if(canRun == 0){
            Deck *current = remaining_pile;
            canRun = create_players(num);
        }
        else
        {

        }
    }
    else{}

}
void printCard(){
    Deck *current = players[0].cards;
    int positionX = gameX/2 - 23;
    int positionY = gameY - 13;
    startX = positionX;
    startY = positionY;

    int i=0;
    while(current!=NULL){
        //sprintf(buffer,"%d",current->card.name);
        if(i<9) {
            line1[i] = subwin(game, 3, 5, positionY, positionX);
            wclear(line1[i]);
            wrefresh(line1[i]);
            wbkgd(line1[i], COLOR_PAIR((current->card.color) + 1));
            if (current->card.name < PLUS) {
                mvwprintw(line1[i], 1, 2, "%s", nameCard[current->card.name]);
            }
            else if (current->card.name == PLUS) {
                if (current->card.color == BLACK) {
                    mvwprintw(line1[i], 1, 1, "%s", nameCard[current->card.name]);
                    mvwprintw(line1[i], 1, 3, "4");
                }
                else {
                    mvwprintw(line1[i], 1, 1, "%s", nameCard[current->card.name]);
                    mvwprintw(line1[i], 1, 3, "2");
                }
            }
            else {
                wattrset(line1[i], COLOR_PAIR(1));
                mvwprintw(line1[i], 0, 0, "  ");
                wattrset(line1[i], COLOR_PAIR(2));
                mvwprintw(line1[i], 0, 3, "  ");
                wattrset(line1[i], COLOR_PAIR(3));
                mvwprintw(line1[i], 2, 0, "  ");
                wattrset(line1[i], COLOR_PAIR(4));
                mvwprintw(line1[i], 2, 3, "  ");
                wattrset(line1[i], COLOR_PAIR(5));
                mvwprintw(line1[i], 1, 2, "%s", nameCard[current->card.name]);
            }
            wrefresh(line1[i]);

            positionX += 6;
            current = current->next;
            if(i==8){
                positionX = startX;
                positionY += 4;
            }
        }
        else
        {
            line2[i-9] = subwin(game, 3, 5, positionY, positionX);
            wclear(line2[i-9]);
            wrefresh(line2[i-9]);
            wbkgd(line2[i-9], COLOR_PAIR((current->card.color) + 1));
            if (current->card.name < PLUS) {
                mvwprintw(line2[i-9], 1, 2, "%s", nameCard[current->card.name]);
            }
            else if (current->card.name == PLUS) {
                if (current->card.color == BLACK) {
                    mvwprintw(line2[i-9], 1, 1, "%s", nameCard[current->card.name]);
                    mvwprintw(line2[i-9], 1, 3, "4");
                }
                else {
                    mvwprintw(line2[i-9], 1, 1, "%s", nameCard[current->card.name]);
                    mvwprintw(line2[i-9], 1, 3, "2");
                }
            }
            else {
                wattrset(line2[i-9], COLOR_PAIR(1));
                mvwprintw(line2[i-9], 0, 0, "  ");
                wattrset(line2[i-9], COLOR_PAIR(2));
                mvwprintw(line2[i-9], 0, 3, "  ");
                wattrset(line2[i-9], COLOR_PAIR(3));
                mvwprintw(line2[i-9], 2, 0, "  ");
                wattrset(line2[i-9], COLOR_PAIR(4));
                mvwprintw(line2[i-9], 2, 3, "  ");
                wattrset(line2[i-9], COLOR_PAIR(5));
                mvwprintw(line2[i-9], 1, 2, "%s", nameCard[current->card.name]);
            }
            wrefresh(line2[i-9]);

            positionX += 6;
            current = current->next;

        }
        i++;
    }
    while(i<9){
        wbkgd(line1[i],COLOR_PAIR(6));
        wclear(line1[i]);
        wrefresh(line1[i]);
        i++;
    }

    free(current);
}

void setPlayedCard(){
    wclear(playedCard);
    wrefresh(playedCard);


    wbkgd(playedCard,COLOR_PAIR((current_card.color) + 1));
    if(current_card.name != PLUS) {
        mvwprintw(playedCard, 1, 2, "%s", nameCard[current_card.name]);
        //mvwprintw(playedCard, 1, 2, "4");
    }
    else {
        if(current_card.color ==BLACK)
        {
            mvwprintw(playedCard, 1, 1, "%s", nameCard[current_card.name]);
            mvwprintw(playedCard, 1, 3, "4");
        }
        else
        {
            mvwprintw(playedCard, 1, 1, "%s", nameCard[current_card.name]);
            mvwprintw(playedCard, 1, 3, "2");
        }
    }
    wrefresh(playedCard);
}

void playGame(WINDOW *parent,int x) {
    Deck *current = remaining_pile;
    current_card = remaining_pile->card;
    remaining_pile = remaining_pile->next;
    free(current);


    discard_pile = malloc(sizeof(Deck));
    discard_pile->card = current_card;
    discard_pile->next = NULL;
    processCard();

    mvwprintw(game,2,gameX/2-8,"Player %d's turn!",currentPosition);
    setPlayedCard();
    wrefresh(parent);

    comPlay(parent,x);
}
void comPlay(WINDOW *parent, int x){

    while (players[currentPosition].length > 0) {
        int index = currentPosition;


        if (players[currentPosition].type == COMPUTER) {
            delay(2);


            int comCanPlay = play_card_com();
            mvwprintw(game,2,gameX/2-8,"Player %d's turn!",currentPosition);

            if (comCanPlay == 0){
                mvwprintw(game,3,gameX/2-8,"Player %d skipped!", index);
            }
            else {
                mvwprintw(game,3,gameX/2-8,"                 ", index);
                mvwprintw(game,4,gameX/2-8,"Player %d played:",index);
            }
            setPlayedCard();
            wrefresh(parent);
        }
        else{
            mvwprintw(parent,2,x/2-8,"   Your turn!   ",currentPosition);
            wrefresh(parent);
            userInput();
            mvwprintw(game,3,gameX/2-8,"                 ", index);
            mvwprintw(parent,1,gameX / 2 - 7,"             ");
            mvwprintw(parent,startY+8,startX,"    ");
            wrefresh(parent);
        }
    }
}
void userInput(){
    MEVENT event;
    int c;
    int count = 0;
    int canPlay = 0;
    while(1){
        c=wgetch(game);
        if (KEY_MOUSE == c) {
            /* Mouse event. */
            if (OK == getmouse(&event)) {
                if(players[0].length < 10) {
                    if ((event.y >= startY && event.y <= startY + 2) &&
                        (event.x >= startX && event.x <= (startX + 4) + 6 * (players[0].length - 1))) {
                        if ((event.x - startX + 1) % 6 != 0) {
                            int index = (event.x - startX) / 6;
                            canPlay = play_card_user(index);
                            if (canPlay == 1) {
                                mvwprintw(game,2,gameX/2-8,"Player %d's turn!",currentPosition);
                                mvwprintw(game,4,gameX/2-8,"You played:     ");
                                printCard();
                                setPlayedCard();

                                wrefresh(game);
                                break;
                            }
                            else {
                                mvwprintw(game, 1, gameX / 2 - 7, "Invalid Card!");
                            }
                        }
                    }
                }
                if(count<1) {
                    if (event.y >= (gameY / 2 - 5) && event.y <= (gameY / 2 - 2) &&
                        event.x >= startX && event.x <= startX + 5) {
                        drawCard(1);
                        printCard();
                        count++;
                        mvwprintw(game,startY+8,startX,"Skip");
                        wrefresh(game);
                    }
                }
                else if (event.y == startY+8  &&
                        event.x >= startX && event.x <= startX + 3) {
                    next_player();
                    break;
                }

            }
        }
    }
}
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
extern int is_stacking;

char *nameCard[15] = {"0","1","2","3","4","5","6","7","8","9","S", "R", "+","W"};
WINDOW *line1[13];
WINDOW *line2[13];
WINDOW *playedCard;
WINDOW *game;
int startX;
int startY;
int gameX, gameY;
int haveToDraw = 0;
int canPlay = 0;
int winning_score;



void setMenuScreen(WINDOW *menu, int maxy, int maxx){
    //mvwprintw(menu,maxy/2-3,maxx/2-2,"Menu");
    mvwprintw(menu,maxy/2-3,maxx/2-4,"New Game");
    mvwprintw(menu,maxy/2-1,maxx/2-4,"Continue");
    mvwprintw(menu,maxy/2+1,maxx/2-3,"Credit");
    mvwprintw(menu,maxy/2+3,maxx/2-2,"Quit");
}

int askNumberPLayer(WINDOW *question) {
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
int typeGame(){
    MEVENT event;
    wclear(game);
    wrefresh(game);
    mvwprintw(game, gameY/2 - 5,gameX/2 - 12 , "Choose your variation: ");
    mvwprintw(game, gameY/2 - 3, gameX/2 - 5, "Standard");
    mvwprintw(game, gameY/2 - 1, gameX/2 - 7, "Stacking UNO");
    mvwprintw(game, gameY/2 + 1, gameX/2 - 4, "Time Bomb");
    mvwprintw(game, gameY-5,gameX-4, "Back");
    int c;
    int returnNum = -1;
    while (1) {
        c = wgetch(game);
        if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
                if ((event.y == gameY-5) && (event.x >= gameX - 4 && event.x < gameX )) {
                    delwin(game);
                    returnNum = -1;
                    break;
                } else if ((event.y == gameY / 2 - 3) &&
                        (event.x >= gameX / 2 - 5) && (event.x < gameX/2 + 3)) {
                    returnNum = 0;
                    break;
                }
                else if ((event.y == gameY / 2 - 1) &&
                         (event.x >= gameX / 2 - 7) && (event.x < gameX/2 + 5)) {
                    returnNum = 1;
                    break;
                }
                else if ((event.y == gameY / 2 + 1) &&
                         (event.x >= gameX / 2 - 4) && (event.x < gameX/2 + 5)) {
                    returnNum = 1;
                    break;
                }
            }

        }
    }
    return returnNum;
}
int winScore(){
    MEVENT event;
    wclear(game);
    wrefresh(game);
    mvwprintw(game, gameY/2 - 3,gameX/2 - 12 , "Choose your winning score: ");
    mvwprintw(game, gameY/2 , gameX/2 - 12, "300     500     750    1000");
    mvwprintw(game, gameY-5,gameX-4, "Back");
    int c;
    int returnNum = 0;
    while (1) {
        c = wgetch(game);
        if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
                if ((event.y == gameY - 5) && (event.x >= gameX - 4 && event.x < gameX)) {
                    delwin(game);
                    returnNum = -1;
                    break;
                }
                else if ((event.y == gameY / 2 ) &&
                         (event.x >= gameX / 2 - 12) && (event.x <= gameX / 2 - 10)) {
                    winning_score = 300;
                    break;
                }
                else if ((event.y == gameY / 2 ) &&
                         (event.x >= gameX / 2 - 4) && (event.x <= gameX / 2 - 2)) {
                    winning_score = 500;
                    break;
                }
                else if ((event.y == gameY / 2 ) &&
                         (event.x >= gameX / 2 + 4) && (event.x <= gameX / 2 + 6)) {
                    winning_score = 750;
                    break;
                }
                else if ((event.y == gameY / 2 ) &&
                         (event.x >= gameX / 2 + 11) && (event.x <= gameX / 2 + 14)) {
                    winning_score = 1000;
                    break;
                }

            }
        }
    }
    return returnNum;
}
void gameScreen(int y, int x, int ifContinue){
    game = newwin(y,x,0,0);
    wbkgd(game,COLOR_PAIR(6));
    keypad(game, TRUE);
    gameX = x;
    gameY = y;
    if(ifContinue == 0) {
        int numPlay = askNumberPLayer(game);
        int typePlay = typeGame();
        int set_win_score = winScore();

        if (numPlay != -1 && typePlay != -1 && set_win_score != -1) {
            wclear(game);
            wrefresh(game);
            playedCard = subwin(game, 3, 5, y / 2 - 5, x / 2 - 2);

            mvwprintw(game, y - 5, x - 4, "Back");
            if (typePlay == 0) {
                mvwprintw(game, 0, 1, "Standard Mode");
            }
            else if(typePlay == 1){
                mvwprintw(game, 0, 1, "Stacking UNO Mode");
            }
            else{
                mvwprintw(game, 0, 1, "Time Bomb Mode");
            }
            if (ifContinue == 0) {
                startGame(numPlay, typePlay);
            }
        }
        printCard();

        WINDOW *drawCard = subwin(game, 3, 5, y / 2 - 5, startX);
        wbkgd(drawCard, COLOR_PAIR(5));
        mvwprintw(drawCard, 1, 1, "UNO");
        wrefresh(drawCard);

        playGame(game, x);
        wrefresh(game);

        wclear(drawCard);
        delwin(drawCard);
        wclear(game);
        delwin(game);
    }
    else{
        continue_saved_game();
        printCard();

        WINDOW *drawCard = subwin(game, 3, 5, y / 2 - 5, startX);
        wbkgd(drawCard, COLOR_PAIR(5));
        mvwprintw(drawCard, 1, 1, "UNO");
        wrefresh(drawCard);

        playGame(game, x);
        wrefresh(game);

        wclear(drawCard);
        delwin(drawCard);
        wclear(game);
        delwin(game);
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

void startGame(int num, int type)
{
    //time_t current_time = time(NULL);
    //srand(current_time);

    int canRun = create_cards(type);
    if (canRun == 0){
        canRun = shuffle(108);
        if(canRun == 0){
            canRun = create_players(num);
            if(canRun == 0){

            }
            else{
                mvwprintw(game, gameY/2,gameX/2 - 8, "Sorry you can't start game!");
                wrefresh(game);
            }
        }
        else
        {

        }
    }
    else{}

}
void printCard(){
    Deck *current = players[0].cards;
    int positionX = gameX/2 - 34;
    int positionY = gameY - 15;
    startX = positionX;
    startY = positionY;

    int i=0;
    while(current!=NULL){
        if(i<13) {
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
            if(i==12){
                positionX = startX;
                positionY += 4;
            }
        }
        else
        {
            line2[i-13] = subwin(game, 3, 5, positionY, positionX);
            wclear(line2[i-13]);
            wrefresh(line2[i-13]);
            wbkgd(line2[i-13], COLOR_PAIR((current->card.color) + 1));
            if (current->card.name < PLUS) {
                mvwprintw(line2[i-13], 1, 2, "%s", nameCard[current->card.name]);
            }
            else if (current->card.name == PLUS) {
                if (current->card.color == BLACK) {
                    mvwprintw(line2[i-13], 1, 1, "%s", nameCard[current->card.name]);
                    mvwprintw(line2[i-13], 1, 3, "4");
                }
                else {
                    mvwprintw(line2[i-13], 1, 1, "%s", nameCard[current->card.name]);
                    mvwprintw(line2[i-13], 1, 3, "2");
                }
            }
            else {
                wattrset(line2[i-13], COLOR_PAIR(1));
                mvwprintw(line2[i-13], 0, 0, "  ");
                wattrset(line2[i-13], COLOR_PAIR(2));
                mvwprintw(line2[i-13], 0, 3, "  ");
                wattrset(line2[i-13], COLOR_PAIR(3));
                mvwprintw(line2[i-13], 2, 0, "  ");
                wattrset(line2[i-13], COLOR_PAIR(4));
                mvwprintw(line2[i-13], 2, 3, "  ");
                wattrset(line2[i-13], COLOR_PAIR(5));
                mvwprintw(line2[i-13], 1, 2, "%s", nameCard[current->card.name]);
            }
            wrefresh(line2[i-13]);

            positionX += 6;
            current = current->next;

        }
        i++;
    }
    while(i<26){
        if(i<13) {
            if(line1[i] == NULL){
                break;
            }
            wbkgd(line1[i], COLOR_PAIR(6));
            wclear(line1[i]);
            wrefresh(line1[i]);
        }
        else if (line2[i-13] != NULL){
            wbkgd(line2[i - 13],COLOR_PAIR(6));
            wclear(line2[i - 13]);
            wrefresh(line2[i - 13]);
        }
        i++;
    }
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

    for(int i=1;i<numPlayers;i++){
        mvwprintw(game,5+(i-1),1,"Player %d: %d cards", i,players[i].length);
    }

    wrefresh(parent);

    comPlay(parent,x);
}
void comPlay (WINDOW *parent, int x){

    while (players[currentPosition].length > 0) {
        int index = currentPosition;

        mvwprintw(parent, 5, gameX / 2 - 7, "             ");
        if (players[currentPosition].type >= COMPUTER) {
            delay(4);
            mvwprintw(game, 1, gameX / 2 - 11, "                     ", currentPosition);
            int comCanPlay = play_card_com();
            mvwprintw(game,2,gameX/2-8,"Player %d's turn!",currentPosition);

            if (comCanPlay == 0){
                mvwprintw(game,3,gameX/2-8,"Player %d skipped!", index);
            }
            else {
                mvwprintw(game,3,gameX/2-8,"                 ", index);
                mvwprintw(game,4,gameX/2-8,"Player %d played:",index);
                mvwprintw(game,5+(index-1),1,"Player %d: %d cards", index, players[index].length);
                canPlay = 0;
            }
            setPlayedCard();
            wrefresh(parent);
        }
        else{
            if(current_card.name==PLUS && is_stacking == 1 && canPlay == 0) {
                mvwprintw(game, startY + 8, startX, "Skip");
                wrefresh(game);
            }
            mvwprintw(parent,2,x/2-8,"   Your turn!   ",currentPosition);
            wrefresh(parent);
            int mode = userInput();
            mvwprintw(game,1,gameX/2-7,"                 ", index);
            if(mode == 0) {
                mvwprintw(parent, 3, gameX / 2 - 8, "                 ");
            }
            else if(mode == 1){
                mvwprintw(game,3,gameX/2-8,"You skipped!     ");
            }
            else{
                break;
            }
            mvwprintw(parent,startY+8,startX,"    ");
            wrefresh(parent);
        }
    }
    if(players[currentPosition].score < winning_score && players[currentPosition].length == 0){
        wclear(game);
        wrefresh(game);
        mvwprintw(game,gameY/2-5,gameX/2 - 14,"Player %d win this round!!!",currentPosition);
        mvwprintw(game,gameY/2 -3,gameX/2 - 15,"Another round?     Yes     No");
        int c;
        MEVENT event;
        flushinp();
        while(1){
            c=wgetch(game);
            if (KEY_MOUSE == c) {
                /* Mouse event. */
                if (OK == getmouse(&event)) {
                    if(event.y == gameY/2-3 && event.x >= gameX/2 + 4 && event.x < gameX + 7){
                        int canRun = play_another_game();
                        playGame(parent,x);
                        break;
                    }
                    else if(event.y == gameY/2-3 && event.x >= gameX/2 + 12 && event.x < gameX + 14){
                        saveScreen();
                        break;
                    }
                }
            }
        }
    }
}
int userInput(){
    MEVENT event;
    int c;
    int returnMode = 0;
    int count = 0;
    flushinp();
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
                                mvwprintw(game, 5, gameX / 2 - 7, "Invalid Card!");
                            }
                        }
                    }
                }
                else{
                    if ((event.y >= startY && event.y <= startY + 2)  &&
                        (event.x >= startX && event.x <= ((startX + 4) + 6 * 12))) {
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
                                mvwprintw(game, 5, gameX / 2 - 7, "Invalid Card!");
                            }
                        }
                    }
                    else if ((event.y >= startY + 4 && event.y <= startY + 6) &&
                            (event.x >= startX && event.x <= (startX + 4) + 6 * (players[0].length - 1))) {
                        if ((event.x - startX + 1) % 6 != 0) {
                            int index = (event.x - startX) / 6 + 13;
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
                                mvwprintw(game, 5, gameX / 2 - 7, "Invalid Card!");
                            }
                        }
                    }
                }
                if(current_card.name==PLUS && is_stacking == 1 && canPlay == 0){
                   if(event.y == startY+8  && event.x >= startX && event.x <= startX + 3) {
                       haveToDraw = 1;
                       play_card_user(0);
                       mvwprintw(game, startY + 8, startX, "    ");
                       mvwprintw(game, 2, gameX / 2 - 8, "Player %d's turn!", currentPosition);
                       break;
                   }
                }
                else {
                    if (count < 1) {
                        if (event.y >= (gameY / 2 - 5) && event.y <= (gameY / 2 - 2) &&
                                                          event.x >= startX && event.x <= startX + 5) {
                            drawCard(1);
                            printCard();
                            count++;
                            mvwprintw(game, startY + 8, startX, "Skip");
                            wrefresh(game);
                        }
                    }
                    else if (event.y == startY + 8 &&
                             event.x >= startX && event.x <= startX + 3) {
                        next_player();
                        returnMode = 1;
                        mvwprintw(game, 2, gameX / 2 - 8, "Player %d's turn!", currentPosition);
                        mvwprintw(game, 5, gameX / 2 - 7, "             ");
                        wrefresh(game);
                        break;
                    }
                }
                if(event.y == (gameY-5) && event.x >=gameX-4 && event.x <= gameX){
                    saveScreen();
                    returnMode = 2;
                    break;
                }

            }
        }
    }
    return returnMode;
}

int choose_card_color(){
    int length = 4;
    int strLength = 12;
    WINDOW *card[length];
    char sentence[] = {'C','H','O','O','S','E',' ','C','O','L','O','R'};
    for (int i=0; i<strLength; i++){
        mvwprintw(game,i+4,gameX - 9,"%c",sentence[i]);
    }
    for (int i=0;i<length;i++){
        card[i] = subwin(game, 3, 5, (3 + i*4), gameX -5);
        wbkgd(card[i],COLOR_PAIR(i+1));
        wrefresh(card[i]);
    }
    wrefresh(game);
    int c, color;
    MEVENT event;
    while(1){
        c=wgetch(game);
        if (KEY_MOUSE == c) {
            /* Mouse event. */
            if (OK == getmouse(&event)) {
                if(event.y>=3 && event.y <= 5 &&
                   event.x >= gameX-5 && event.x < gameX){
                    color = 0;
                    break;
                }
                else if (event.y>=7 && event.y <= 9 &&
                         event.x >= gameX-5 && event.x < gameX) {
                    color = 1;
                    break;
                }
                else if (event.y>=11 && event.y <= 13 &&
                         event.x >= gameX-5 && event.x < gameX) {
                    color = 2;
                    break;
                }
                else if (event.y>=15 && event.y <= 17 &&
                         event.x >= gameX-5 && event.x < gameX) {
                    color = 3;
                    break;
                }
            }
        }

    }
    for (int i=0;i<length;i++){
        wbkgd(card[i],COLOR_PAIR(6));
        wclear(card[i]);
        wrefresh(card[i]);
        delwin(card[i]);
    }
    for (int i=0; i<strLength; i++){
        mvwprintw(game,i+4,gameX - 9," ");
    }
    return color;
}

void saveScreen(){
    wclear(game);
    wrefresh(game);
    mvwprintw(game,gameY/2 - 5, gameX/2 - 13 , "Do you want to save game?");
    mvwprintw(game,gameY/2 - 3, gameX/2 - 13 , "    Yes             No   ");
    MEVENT event;
    int c;
    while(1) {
        c = wgetch(game);
        if (KEY_MOUSE == c) {
            /* Mouse event. */
            if (OK == getmouse(&event)) {
                if (event.y == (gameY / 2 - 3) &&
                    event.x >= gameX/2 -9 && event.x < gameX/2-6) {
                    save_game();
                    break;
                }
                else if(event.y == (gameY / 2 - 3) &&
                        event.x >= gameX/2 + 7 && event.x < gameX/2+9) {
                    break;
                }
            }
        }
    }
    int length = 13;
    for(int i=0; i<length;i++){
        if (line1[i]!=NULL){
            wclear(line1[i]);
            delwin(line1[i]);
        }
        if (line2[i]!=NULL){
            wclear(line2[i]);
            delwin(line2[i]);
        }
    }
    wclear(playedCard);
    delwin(playedCard);
}
void errorScreen(){

}
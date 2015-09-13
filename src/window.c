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
extern int variation;
extern int turns_left;
extern int stacking;

char *nameCard[15] = {"0","1","2","3","4","5","6","7","8","9","S", "R", "+","W"}; /** <This is character array that display name of cards */
WINDOW *line1[13]; /** < This is the array that holds the sub windows displaying the cards of the user and those windows are displayed at the first line */
WINDOW *line2[13];  /** < This is the array that holds the sub windows displaying the cards of the user and those windows are displayed at the second line */
WINDOW *playedCard; /** < This is the sub window that display the card on the table (the card that is discarded) */
WINDOW *game; /** <This is the window that displayed when user play the game */

int startX; /** < This the integer that store the x value(the column) which is the left alignment for the users' card to display */
int startY;/** < This is the inter that store the y value (the line) that is the top alignment of the users' cards  */
int gameX, gameY; /** < gameX is the width(columns) of the game screen, gameY is the height(lines) of the game screen*/
int haveToDraw = 0; /** < This integer show if the user have to draw cards or not*/
int canPlay = 0; /** < This is the integer that represent if the user can discard or not*/
int winning_score; /** < This represents the winning score that user has set */
int nextRound; /** < This integer represents if the user want a next round or not*/



void setMenuScreen(WINDOW *menu, int maxy, int maxx){
    //Display the necessary data on the window
    mvwprintw(menu,maxy/2-3,maxx/2-4,"New Game");
    mvwprintw(menu,maxy/2-1,maxx/2-4,"Continue");
    mvwprintw(menu,maxy/2+1,maxx/2-3,"Credit");
    mvwprintw(menu,maxy/2+3,maxx/2-2,"Quit");
}

int askNumberPLayer(WINDOW *question) {
    MEVENT event;
    int x, y;
    //Get the lines and columns of the window and set it to x, y
    getmaxyx(question, y, x);

    //Display the necessary data
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
    mvwprintw(question, y-1,x-4, "Back");
    int c;
    int returnNum = -1;

    //To erase all the input before
    flushinp();

    //The loop for get the user input
    //This loop will be broken if user click to the right place that display the data
    while (1) {
        //Get the user input and set it to integer c
        c = wgetch(question);
        //If c is Key_Mouse and c is OK then do the following
        //event.x and event.y is the column and row that user clicked
        if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
                //If user click back
                if ((event.y == y - 1) && (event.x >= x - 4 && event.x < x )) {
                    delwin(question);
                    returnNum = -1;
                    break;
                }
                    //Set the returnNum equal to the value that user click
                else if ((event.y == y / 2 + 1) && (event.x == x / 2 - 8)) {
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
    //clear all the display of game window to set it again
    wclear(game);
    wrefresh(game);

    //Display the necessary data
    mvwprintw(game, gameY/2 - 5,gameX/2 - 12 , "Choose your variation: ");
    mvwprintw(game, gameY/2 - 3, gameX/2 - 5, "Standard");
    mvwprintw(game, gameY/2 - 1, gameX/2 - 7, "Stacking UNO");
    mvwprintw(game, gameY/2 + 1, gameX/2 - 5, "Time Bomb");
    mvwprintw(game, gameY-1,gameX-4, "Back");
    int c;
    int returnNum = -1;

    //To erase all the input before
    flushinp();

    //The loop for get the user input
    //This loop will be broken if user click to the right place that display the data
    while (1) {

        //Get the user input and set it to integer c
        c = wgetch(game);

        //If c is Key_Mouse and c is OK then do the following
        //event.x and event.y represent the column and row that user clicked
        if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
                //If the user click back and then delete the window and set returnNum to -1
                if ((event.y == gameY-1) && (event.x >= gameX - 4 && event.x < gameX )) {
                    delwin(game);
                    returnNum = -1;
                    break;
                }
                    //Set the returnNum to the value that user click
                else if ((event.y == gameY / 2 - 3) &&
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
                         (event.x >= gameX / 2 - 5) && (event.x < gameX/2 + 4)) {
                    returnNum = 2;
                    break;
                }
            }

        }
    }
    return returnNum;
}
int winScore(){
    MEVENT event;

    //Clear all the display of game screen to set it again
    wclear(game);
    wrefresh(game);

    //Display the necessary data
    mvwprintw(game, gameY/2 - 3,gameX/2 - 12 , "Choose your winning score: ");
    mvwprintw(game, gameY/2 , gameX/2 - 12, "300     500     750    1000");
    mvwprintw(game, gameY-1,gameX-4, "Back");
    int c;
    int returnNum = 0;

    //To erase all the input before
    flushinp();

    //The loop for get the user input
    //This loop will be broken if user click to the right place that display the data
    while (1) {

        //Get the user input and set it to integer c
        c = wgetch(game);

        //If c is Key_Mouse and c is OK then do the following
        //event.x and event.y represent the column and row that user clicked
        if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
                //If the user click back and then delete the window and set returnNum to -1
                if ((event.y == gameY - 1 && (event.x >= gameX - 4 && event.x < gameX))) {
                    delwin(game);
                    returnNum = -1;
                    break;
                }
                    //Set the winning score equal to the value that user clicked
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
    nextRound = 0;

    //Create game window
    game = newwin(y,x,0,0);
    wbkgd(game,COLOR_PAIR(6));
    keypad(game, TRUE);
    gameX = x;
    gameY = y;

    //If user started a new game, call the functions to ask how many players, the variation and the winning score
    //If user pass all the questions, then go to setGame
    //If user click back at one point, then return to main menu
    if(ifContinue == 0) {
        int numPlay = askNumberPLayer(game);
        if(numPlay != -1) {
            int typePlay = typeGame();
            if(typePlay != -1) {
                int set_win_score = winScore();
                if(set_win_score != -1 ){
                    setGame(x, y, ifContinue, numPlay, typePlay);
                }
            }
        }

    }

        //If user want to continue, go straight to set game
    else{
        setGame(x,y,ifContinue,0,0);
    }

    //If user want to start another round, then run gameScreen again with the value ifContinue euqal to 2
    if(nextRound == 1){
        gameScreen(y,x,2);
    }
}


void setGame(int x, int y, int ifContinue, int numPLay, int typePlay){
    //Clear the display of the game window
    wclear(game);
    wrefresh(game);

    //Create a sub window for the card that is discarded
    playedCard = subwin(game, 3, 5, y / 2 - 5, x / 2 - 2);

    //ifContinue is equal to 0 then start the new game
    if (ifContinue == 0) {
        int run = startGame(numPLay, typePlay);
        if(run!=0){
            return;
        }
    }
        //ifContinue is equal to 1 then call function continue_save_game to continue the last saved game
    else if(ifContinue == 1){
        int run = continue_saved_game();
        if(run!=0){
            errorScreen();
            return;
        }
    }

    //Display necessary on game window
    mvwprintw(game, y - 1, x - 4, "Back");
    if (variation == 0) {
        mvwprintw(game, 0, 1, "Standard Mode");
    }
    else if(variation == 1){
        mvwprintw(game, 0, 1, "Stacking UNO Mode");
    }
    else{
        mvwprintw(game, 0, 1, "Time Bomb Mode");
    }

    //Call the function printCard to display the user's cards
    printCard();

    //Create a sub window to represent the discard pile
    WINDOW *drawCard = subwin(game, 3, 5, y / 2 - 5, startX);
    wbkgd(drawCard, COLOR_PAIR(5));
    mvwprintw(drawCard, 1, 1, "UNO");
    wrefresh(drawCard);

    playGame(game, x, ifContinue);
    wrefresh(game);

    //If user don't want another round then stop the game
    if(nextRound != 1) {
        stop_game();
    }

}


void creditScreen(int y, int x) {
    MEVENT event;

    //Create the window credit and display data
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

    //To erase all the input before
    flushinp();

    //The loop for get the user input
    //This loop will be broken if user click to the right place that display the data
    while (1) {

        //Get the user input and set it to integer c
        c = wgetch(game);

        //If c is Key_Mouse and c is OK then do the following
        //event.x and event.y represent the column and row that user clicked
        if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
                //If the user click back and then delete the window and back to the main menu
                if ((event.y == y - 5) && (event.x >= x - 10 && event.x < x - 5)) {
                    wrefresh(credit);
                    wclear(credit);
                    delwin(credit);
                    break;
                }
            }
        }
    }
}


int startGame(int num, int type)
{
    //Call the function to create the create cards
    //Go to error screen if there is a error during the process
    int canRun = create_cards(type);
    if (canRun == 0){
        //Shuffle the pile
        canRun = shuffle(108);
        if(canRun == 0){
            //Create the player
            canRun = create_players(num);
            if(canRun != 0){
                errorScreen();
            }
        }
        else
        {
            errorScreen();
        }
    }
    else{
        errorScreen();
    }

    //Return if the process ran successfully
    return canRun;

}

void printCard(){
    Deck *current = players[0].cards;

    //The column that will display a card
    int positionX = gameX/2 - 34;
    //The row that will display the card
    int positionY = gameY - 13;

    startX = positionX;
    startY = positionY;

    int i=0;
    while(current!=NULL && i<26){
        if(i<13) {
            //Each card is represented by a sub window
            //There first 13 sub windows are stored in line1 array
            line1[i] = subwin(game, 3, 5, positionY, positionX);

            //Clear the window before set new value
            wclear(line1[i]);
            wrefresh(line1[i]);

            //Set the background color for the window
            wbkgd(line1[i], COLOR_PAIR((current->card.color) + 1));

            //Print the card name to the window
            if (current->card.name < PLUS) {
                mvwprintw(line1[i], 1, 2, "%s", nameCard[current->card.name]);
            }
                //if the card name is PLUS, then check the color to print out +2 or +4
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
                //Display the wild black card
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

            //Increase the positionX to print next card
            positionX += 6;
            current = current->next;
            //If i reached 12, which means 13 cards displayed then jump to line2
            if(i==12){
                positionX = startX;
                positionY += 4;
            }
        }
        else if(i>12 && i<26)
        {
            //The following code is used for displaying the 14th to 26th card of user
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

    //If user has more than 26 cards, then there is not enough spaces to display
    //Print how many cards left after display the first 26 cards
    if(players[0].length > 26){
        mvwprintw(game, startY + 9, startX + 65, "%d cards left!",players[0].length-26);
    }
        //Erase the display if user has less than 26 cards
    else{
        mvwprintw(game, startY + 9, startX + 65, "               ");
    }

    //This code is used to clear the display of the card that has been discarded
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

    //Clear the current display of the window
    wclear(playedCard);
    wrefresh(playedCard);

    //set the window background
    wbkgd(playedCard,COLOR_PAIR((current_card.color) + 1));

    //Display the current card (card that has been discarded) name on playedCard window
    if(current_card.name != PLUS) {
        mvwprintw(playedCard, 1, 2, "%s", nameCard[current_card.name]);
    }
    else {
        //If the current card is PLUS BLACK, then the color of the current will be chosen randomly
        if(current_card.color ==BLACK)
        {
            current_card.color = pt_rand(2);
            //After choosing the current card color then display it
            wbkgd(playedCard,COLOR_PAIR((current_card.color) + 1));
            mvwprintw(playedCard, 1, 1, "%s", nameCard[current_card.name]);
            mvwprintw(playedCard, 1, 3, "4");
        }
            //This case is used for if the current card color is set to the color which is different from black
            //But actually the current card is PLUS BLACK
            //Then have to check the discard pile to set the name
        else if(discard_pile->card.color == BLACK && discard_pile->card.name == PLUS){
            wbkgd(playedCard,COLOR_PAIR((current_card.color) + 1));
            mvwprintw(playedCard, 1, 1, "%s", nameCard[current_card.name]);
            mvwprintw(playedCard, 1, 3, "4");
        }
            //Set the display for the PLUS card which has the color different from BLACK
        else
        {
            mvwprintw(playedCard, 1, 1, "%s", nameCard[current_card.name]);
            mvwprintw(playedCard, 1, 3, "2");
        }
    }
    wrefresh(playedCard);
}

void playGame(WINDOW *parent,int x, int ifContinue) {
    //If user starts the new game, then take the first card of the remain to discard and start the game
    if(ifContinue == 0) {
        Deck *current = remaining_pile;
        current_card = remaining_pile->card;
        remaining_pile = remaining_pile->next;
        free(current);

        discard_pile = malloc(sizeof(Deck));
        discard_pile->card = current_card;
        discard_pile->next = NULL;
        processCard();

        mvwprintw(game, 2, gameX / 2 - 8, "Player %d's turn!", currentPosition);
    }
        //If user continue the game, then display whose turn is next
    else{
        if(currentPosition == 0) {
            mvwprintw(game, 2, gameX / 2 - 8, "    Your turn!   ", currentPosition);
        }
        else{
            mvwprintw(game, 2, gameX / 2 - 8, "Player %d's turn!", currentPosition);
        }
    }

    //Display the card that has been discarded
    setPlayedCard();

    //Print number of cards that each player has
    for(int i=1;i<numPlayers;i++){
        mvwprintw(game,4+(i-1),1,"P%d: %d cards ", i,players[i].length);
    }

    //Print the score that each player has
    for(int i=0;i<numPlayers;i++){
        mvwprintw(game,16+i,1,"P%d: %d",i,players[i].score);
    }

    wrefresh(parent);

    //Call function to get the players play game
    comPlay(parent,x);
}


void comPlay (WINDOW *parent, int x){
    int index;

    //Play the game until there is a winner
    while (players[currentPosition].length > 0) {
        index = currentPosition;
        //Print the number of stack if the variation is stacking or time bomb
        if(variation == 1 || variation == 2) {
            mvwprintw(game, 0, gameX - 24, "Number of stack: %d ", stacking);
        }
        //If the variation is time bomb, print how many turns left the bomb will explode
        if(variation == 2){
            mvwprintw(game, 1, gameX - 24, "Turn(s) to explode: %d ", turns_left);
        }
        wrefresh(game);

        //Clear the display at the line 1
        mvwprintw(game, 1, gameX / 2 - 11, "                       ", currentPosition);

        //If the player at the current position is the computer, then call the function to make the AI play
        if (players[currentPosition].type >= COMPUTER) {
            delay(3);
            mvwprintw(game, 1, gameX / 2 - 11, "                     ", currentPosition);
            int comCanPlay = play_card_com();
            //Display whose turn is next
            mvwprintw(game,2,gameX/2-8,"Player %d's turn!",currentPosition);

            //If the player can't play this turn, then print which player skip
            if (comCanPlay == 0){
                mvwprintw(game,3,gameX/2-8,"Player %d skipped!", index);
            }
                //If the player can play, then print whose turn is next and print which player just played
            else {
                mvwprintw(game,3,gameX/2-8,"                 ", index);
                mvwprintw(game,4,gameX/2-8,"Player %d played:",index);
                mvwprintw(game,4+(index-1),1,"P%d: %d cards ", index, players[index].length);
                canPlay = 0;
                setPlayedCard();
            }
            wrefresh(parent);
        }
            //If the player is the user
        else{
            //If the variation is Stacking, and the current card name now is PLUS
            //Or if the variation is Time bomb,and the bomb have to explode this turn
            //Then display Skip
            if((current_card.name==PLUS && variation == 1 && stacking != 0) ||
               (variation == 2 && turns_left == 0 && stacking != 0)){
                mvwprintw(game, startY + 9, startX, "Skip");
                wrefresh(game);
            }

            mvwprintw(parent,2,x/2-8,"   Your turn!   ",currentPosition);
            wrefresh(parent);

            //Required user input
            int mode = userInput();

            //Erase the display at the line 5
            mvwprintw(parent, 5, gameX / 2 - 7, "             ");

            //If the user can play then print nothing at line 3
            if(mode == 0) {
                mvwprintw(parent, 3, gameX / 2 - 8, "                 ");
            }
                //Else if user can't play, print that user skip
            else if(mode == 1){
                mvwprintw(game,3,gameX/2-8,"You skipped!     ");
            }
                //if user click back then break the loop
            else{
                break;
            }

            //This code is used to erase the display of the word Skip
            mvwprintw(parent,startY+9,startX,"    ");
            wrefresh(parent);
        }

    }

    //If there is the player win, then set the index to be the different position from the current position
    if(currentPosition != 0){
        index = currentPosition-1;
    }
    else{
        index = currentPosition+1;
    }

    //If the winner score doesn't reach the score and the length of the current position is 0
    //and other players still have cards
    //Then display the screen that ask if the user want to play another round
    if(players[currentPosition].score < winning_score && players[currentPosition].length == 0
            && players[index].cards != NULL){
        wclear(game);
        wrefresh(game);
        mvwprintw(game,gameY/2-5,gameX/2 - 14,"Player %d win this round!!!",currentPosition);
        mvwprintw(game,gameY/2 -3,gameX/2 - 15,"Another round?     Yes     No");
        int c;
        MEVENT event;

        //To erase all the input before
        flushinp();

        //The loop for get the user input
        //This loop will be broken if user click to the right place that display the data
        while (1) {

            //Get the user input and set it to integer c
            c = wgetch(game);

            //If c is Key_Mouse and c is OK then do the following
            //event.x and event.y represent the column and row that user clicked
            if (KEY_MOUSE == c) {
                /* Mouse event. */
                if (OK == getmouse(&event)) {
                    if(event.y == gameY/2-3 && event.x >= gameX/2 + 4 && event.x < gameX/2 + 7){
                        int canRun = play_another_game();
                        if (canRun == 0) {
                            nextRound = 1;
                        }
                        else{
                            errorScreen();
                        }
                        break;
                    }
                        //If user don't want another round then display the save screen
                    else if(event.y == gameY/2-3 && event.x >= gameX/2 + 12 && event.x < gameX/2 + 14){
                        saveScreen();
                        break;
                    }
                }
            }
        }
    }

        //If the winner's score is more than or equal to the winning score then display who win overall
        //User can click anywhere to go back to main menu
    else if(players[currentPosition].score >= winning_score){
        wclear(game);
        wrefresh(game);
        mvwprintw(game,gameY/2-5,gameX/2 - 14,"Player %d win overall!!!",currentPosition);
        mvwprintw(game,gameY/2-3,gameX/2 - 20,"(Click anywhere to go back to Main Menu)",currentPosition);
        int c;
        MEVENT event;
        flushinp();
        while(1){
            c=wgetch(game);
            if (KEY_MOUSE == c && OK == getmouse(& event)) {
                break;
            }
        }
    }
}

int userInput(){
    MEVENT event;
    int c;

    //returnMode represents if user can play card or not, or user clicked back
    int returnMode = 0;

    //The count represents if user click draw pile or not
    int count = 0;

    //To erase all the input before
    flushinp();

    //The loop for get the user input
    //This loop will be broken if user click to the right place that display the data
    while (1) {

        //Get the user input and set it to integer c
        c = wgetch(game);

        //If c is Key_Mouse and c is OK then do the following
        //event.x and event.y represent the column and row that user clicked
        if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
                //If user's cards is less than or equal to 13 t
                // Then only get the event if user clicked on the line and column that have the card's display
                if(players[0].length < 13) {
                    if ((event.y >= startY && event.y <= startY + 2) &&
                        (event.x >= startX && event.x <= (startX + 4) + 6 * (players[0].length - 1))) {
                        if ((event.x - startX + 1) % 6 != 0) {

                            //index represents the position of the card that user clicked
                            int index = (event.x - startX) / 6;

                            //Check if the card user clicked is playable
                            canPlay = play_card_user(index);
                            //If the user can play, then display whose turn is next
                            if (canPlay >= 1) {
                                mvwprintw(game,2,gameX/2-8,"Player %d's turn!",currentPosition);
                                mvwprintw(game,4,gameX/2-8,"You played:     ");
                                printCard();
                                //If player play a card which is not PLUS BLACK
                                if(canPlay == 1) {
                                    setPlayedCard();
                                }
                                else{
                                    wclear(playedCard);
                                    wbkgd(playedCard,COLOR_PAIR((current_card.color) + 1));
                                    mvwprintw(playedCard, 1, 1, "%s", nameCard[current_card.name]);
                                    mvwprintw(playedCard, 1, 3, "4");
                                    wrefresh(playedCard);
                                }
                                wrefresh(game);
                                break;
                            }
                                //If user can't play then display
                            else {
                                mvwprintw(game, 5, gameX / 2 - 7, "Invalid Card!");
                            }
                        }
                    }
                }
                    //If the user has more than 13 cards then receive the input of the user at the windows on line1 and line2
                else{
                    if ((event.y >= startY && event.y <= startY + 2)  &&
                        (event.x >= startX && event.x <= ((startX + 4) + 6 * 12))) {
                        if ((event.x - startX + 1) % 6 != 0) {
                            int index = (event.x - startX) / 6;
                            canPlay = play_card_user(index);
                            if (canPlay >= 1) {
                                mvwprintw(game,2,gameX/2-8,"Player %d's turn!",currentPosition);
                                mvwprintw(game,4,gameX/2-8,"You played:     ");
                                printCard();
                                if(canPlay == 1) {
                                    setPlayedCard();
                                }
                                else{
                                    wclear(playedCard);
                                    wbkgd(playedCard,COLOR_PAIR((current_card.color) + 1));
                                    mvwprintw(playedCard, 1, 1, "%s", nameCard[current_card.name]);
                                    mvwprintw(playedCard, 1, 3, "4");
                                    wrefresh(playedCard);
                                }

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
                            if (canPlay >= 1) {
                                mvwprintw(game,2,gameX/2-8,"Player %d's turn!",currentPosition);
                                mvwprintw(game,4,gameX/2-8,"You played:     ");
                                printCard();
                                if(canPlay == 1) {
                                    setPlayedCard();
                                }
                                else{
                                    wclear(playedCard);
                                    wbkgd(playedCard,COLOR_PAIR((current_card.color) + 1));
                                    mvwprintw(playedCard, 1, 1, "%s", nameCard[current_card.name]);
                                    mvwprintw(playedCard, 1, 3, "4");
                                    wrefresh(playedCard);
                                }

                                wrefresh(game);
                                break;
                            }
                            else {
                                mvwprintw(game, 5, gameX / 2 - 7, "Invalid Card!");
                            }
                        }
                    }
                }

                //If the variation is Stacking, and the current card name now is PLUS
                //Or if the variation is Time bomb,and the bomb have to explode this turn
                //Then get the input of user if user click Skip
                if((current_card.name==PLUS && variation == 1 && canPlay == 0) ||
                   (variation == 2 && turns_left == 0 && stacking != 0)){
                    //If the user click skip then set the integer haveToDraw to 1 and process the card
                   if(event.y == startY+9  && event.x >= startX && event.x <= startX + 3) {
                       haveToDraw = 1;
                       play_card_user(0);
                       mvwprintw(game, startY + 9, startX, "    ");
                       mvwprintw(game, 2, gameX / 2 - 8, "Player %d's turn!", currentPosition);
                       break;
                   }
                }
                    //If the user choose standard mode
                else {
                    //If the user hasn't click draw card, get the input if user click it
                    if (count < 1) {
                        //If the user click draw card, then set the count value to 1
                        //And display Skip
                        if (event.y >= (gameY / 2 - 5) && event.y <= (gameY / 2 - 2) &&
                                                          event.x >= startX && event.x <= startX + 5) {
                            drawCard(1);
                            printCard();
                            count++;
                            mvwprintw(game, startY + 9, startX, "Skip");
                            wrefresh(game);
                        }
                   }
                        //If the user has clicked draw card, then only get the input if user click skip
                        //If user click skip, then set returnMode to 1 and call next player to play
                    else if (event.y == startY + 9 &&
                             event.x >= startX && event.x <= startX + 3) {
                        next_player();
                        returnMode = 1;
                        mvwprintw(game, 2, gameX / 2 - 8, "Player %d's turn!", currentPosition);
                        mvwprintw(game, 5, gameX / 2 - 7, "             ");
                        wrefresh(game);
                        break;
                    }
                }

                //If user click back, then display save screen and set returnMode to 2
                if(event.y == (gameY-1) && event.x >=gameX-4 && event.x <= gameX && count < 1){
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
    // When the user play a WILD BLACK or DRAW 4 card, this will create a color
    // panel for the user to choose the color for next playing turn
    int length = 4;
    int strLength = 12;
    WINDOW *card[length];
    char sentence[] = {'C','H','O','O','S','E',' ','C','O','L','O','R'};
    // Print CHOOSE COLOR in a vertical line
    for (int i=0; i<strLength; i++){
        mvwprintw(game,i+4,gameX - 7,"%c",sentence[i]);
    }
    // Print 4 color red, blue, green and yellow window
    for (int i=0;i<length;i++){
        card[i] = subwin(game, 3, 5, (3 + i*4), gameX -5);
        wbkgd(card[i],COLOR_PAIR(i+1));
        wrefresh(card[i]);
    }
    wrefresh(game);

    // Wait for user input
    int c, color;
    MEVENT event;
    flushinp();
    while(1){
        c=wgetch(game);
        if (KEY_MOUSE == c) {
            /* Mouse event. */
            // Base on where user clicks, return the selected color
            if (OK == getmouse(&event)) {
                if(event.y>=3 && event.y <= 5 &&
                   event.x >= gameX-5 && event.x < gameX){
                    // red
                    color = 0;
                    mvwprintw(game,0,gameX/2-16,"                               ");
                    for (int i=0; i<strLength; i++){
                        mvwprintw(game,i+4,gameX - 7," ",sentence[i]);
                    }
                    break;
                }
                else if (event.y>=7 && event.y <= 9 &&
                         event.x >= gameX-5 && event.x < gameX) {
                    // blue
                    color = 1;
                    mvwprintw(game,0,gameX/2-16,"                               ");
                    for (int i=0; i<strLength; i++){
                        mvwprintw(game,i+4,gameX - 7," ",sentence[i]);
                    }
                    break;
                }
                else if (event.y>=11 && event.y <= 13 &&
                         event.x >= gameX-5 && event.x < gameX) {
                    // green
                    color = 2;
                    mvwprintw(game,0,gameX/2-16,"                               ");
                    for (int i=0; i<strLength; i++){
                        mvwprintw(game,i+4,gameX - 7," ",sentence[i]);
                    }
                    break;
                }
                else if (event.y>=15 && event.y <= 17 &&
                         event.x >= gameX-5 && event.x < gameX) {
                    // yellow
                    color = 3;
                    mvwprintw(game,0,gameX/2-16,"                               ");
                    for (int i=0; i<strLength; i++){
                        mvwprintw(game,i+4,gameX - 7," ",sentence[i]);
                    }
                    break;
                }
                else{
                    // user click some where else
                    mvwprintw(game,0,gameX/2-16,"You must choose the card color!");
                }
            }
        }
    }
    // clear and delete the color panel
    for (int i=0;i<length;i++){
        wbkgd(card[i],COLOR_PAIR(6));
        wclear(card[i]);
        wrefresh(card[i]);
        delwin(card[i]);
    }

    // clear the choose color line
    for (int i=0; i<strLength; i++){
        mvwprintw(game,i+4,gameX - 9," ");
    }
    return color;
}

void saveScreen(){
    // When the user wish to get back to the menu screen while playing, this 
    // will create a screen to ask whether the user want to save the game or not
    wclear(game);
    wrefresh(game);
    mvwprintw(game,gameY/2 - 5, gameX/2 - 13 , "Do you want to save game?");
    mvwprintw(game,gameY/2 - 3, gameX/2 - 13 , "    Yes             No   ");
    MEVENT event;
    int c;
    flushinp();

    // wait for user input
    while(1) {
        c = wgetch(game);
        if (KEY_MOUSE == c) {
            /* Mouse event. */
            if (OK == getmouse(&event)) {
                if (event.y == (gameY / 2 - 3) &&
                    event.x >= gameX/2 -9 && event.x < gameX/2-6) {
                    save_game();
                    // if the user click yes
                    break;
                }
                else if(event.y == (gameY / 2 - 3) &&
                        event.x >= gameX/2 + 7 && event.x < gameX/2+9) {
                    // if the user click no
                    break;
                }
            }
        }
    }
}
void errorScreen(){
    //Clear the display of game window
    wclear(game);
    wrefresh(game);

    //Display the error message
    mvwprintw(game,gameY/2 - 1,gameX/2 - 16,"Sorry you can't start the game!");
    mvwprintw(game,gameY/2 + 1,gameX/2 - 20,"(Click anywhere to go back to Main Menu)",currentPosition);
    int c;
    MEVENT event;

    //To erase all the input before
    flushinp();

    //The loop for get the user input
    //This loop will be broken if user click anywhere
    while (1) {

        //Get the user input and set it to integer c
        c = wgetch(game);

        if (KEY_MOUSE == c && OK == getmouse(& event)) {
            break;
        }
    }
}
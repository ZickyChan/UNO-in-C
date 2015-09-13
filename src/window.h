//
// Created by bit on 9/5/15.
//
#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "function.h"


#ifndef UNO_WINDOW_H
#define UNO_WINDOW_H

/**
 * Set a new window when start or continue the game
 * Basically
 * Turn the Deck (linked-list) into an array and feed it to quick_sort function
 * @param length the length of the player's deck of cards
 */
void gameScreen(int y, int x, int ifContinue);

void creditScreen(int y, int x);
int askNumberPLayer(WINDOW *question);
int typeGame();
void setMenuScreen(WINDOW *menu, int maxy, int maxx);
void startGame(int num, int type);
void setGame(int x, int y, int ifContinue, int numPLay, int typePlay);
void printCard();
void setPlayedCard();
void playGame(WINDOW *parent,int x, int ifContinue);
void comPlay(WINDOW *parent, int x);
int userInput();
int choose_card_color();
void saveScreen();
int winScore();
void delWins();

#endif //UNO_WINDOW_H

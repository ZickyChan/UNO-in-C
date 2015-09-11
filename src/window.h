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

void gameScreen(int y, int x, int ifContinue);
void creditScreen(int y, int x);
int askNumberPLayer(WINDOW *question);
int typeGame();
void setMenuScreen(WINDOW *menu, int maxy, int maxx);
void startGame(int num, int type);
void printCard();
void setPlayedCard();
void playGame(WINDOW *parent,int x);
void comPlay(WINDOW *parent, int x);
int userInput();
int choose_card_color();
void saveScreen();
int winScore();

#endif //UNO_WINDOW_H

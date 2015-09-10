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

void gameScreen(int y, int x);
void creditScreen(int y, int x);
void setMenuScreen(WINDOW *menu, int maxy, int maxx);
void startGame(int num);
void printCard();
void setPlayedCard();
void playGame(WINDOW *parent,int x);
void comPlay(WINDOW *parent, int x);
int userInput();
int choose_card_color();

#endif //UNO_WINDOW_H

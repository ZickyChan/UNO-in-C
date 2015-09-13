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
 * This function will also call other function to get the necessary input for users
 * @param y is the height(lines) of the window which will be created
 * @param x is the width(columns) of the window which will be created
 * @param ifContinue is used to recognize that player want to continue or start a new game
 */
void gameScreen(int y, int x, int ifContinue);

/**
 * Create a window to display credit details
 * @param y is the height(lines) of the window which will be created
 * @param x is the width(columns) of the window which will be created
 */
void creditScreen(int y, int x);

/*
 * Display the question that will ask how many players the user want to create
 * And receive the input from user
 * It will return the number of players
 * @param question is the window will display the question
 */
int askNumberPLayer(WINDOW *question);

/*
 * Display the question that will ask which variation user want to play on WINDOW game
 * And receive the input from user
 * It will return the variation of the game
 */
int typeGame();

/*
 * Display the question that will ask user to choose the winning score
 * And receive the input from user
 * It will return the winning score
 */
int winScore();

/*
 * Display menu for the standard screen
 * @param menu is the window that will display the menu (in this case will be the standard screen)
 * @param maxx is the number of columns of the window menu
 * @param maxy is the number of lines of the window menu
 */
void setMenuScreen(WINDOW *menu, int maxy, int maxx);

/*
 * This function will call all the necessary functions in function.c to start a game
 * Such as create cards, create player, shuffle the cards
 * @param num is the number of players
 * @param type is the variation of the game
 */
int startGame(int num, int type);

/*
 * This function is the main control of the game
 * It will call all the functions to run the game
 * @param x is the number of columns(width) of the window game
 * @param y is the number of lines(height) of the window game
 * @param ifContinue is used to check if user want to start a new game, continue or play another round
 * @param numPlay is the number of players (it is only used when start the game)
 * @param typePlay is the variation of the game (it is only used when start the game)
 */
void setGame(int x, int y, int ifContinue, int numPLay, int typePlay);

/*
 * This function is used to print the cards of the player 0(user)
 */
void printCard();

/*
 * This function is used to print the card that is discarded
 */
void setPlayedCard();

/*
 * This function is used to set up the beginning display of the game
 * And also call the function that players start to play
 * @param parent is the window game which the main window to run the game and it holds the sub windows for the displayment
 * @param x is the width (columns) of the parent
 * @param ifContinue to know that if the game is a new game, continue or another round
 */
void playGame(WINDOW *parent,int x, int ifContinue);

/*
 * This function is used to play the game
 * It will call the AI to process the card and play the cards
 * When there is the player 0's(users') turn, it call call the function userInput
 * @param parent is the window game which the main window to run the game and it holds the sub windows for the displayment
 * @param x is the width (columns) of the parent
 */
void comPlay(WINDOW *parent, int x);

/*
 * This function is used for player 0 (user) to choose the card to play or draw cards
 * Or back to menu
 * It will return that integer that represents if the user discard, click back to menu or skip
 */
int userInput();

/*
 * This function is used for player 0 (user) to choose the color for the black card
 * It will return the color that player 0 (user) chose
 */
int choose_card_color();

/*
 * This function is used to change the display of game window to ask if player 0 (user) want to save the game
 */
void saveScreen();

/*
 * This function is used to delete all the windows before end the standard screen
 */
void delWins();

/*
 *This function is used to display the error screen when there is not enough memory space to malloc
 */
void errorScreen();

#endif //UNO_WINDOW_H

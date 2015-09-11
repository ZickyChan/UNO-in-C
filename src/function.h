//
// Created by bit on 8/14/15.
//

#ifndef UNO_FUNCTION_H
#define UNO_FUNCTION_H

#define LINESIZE 1000
#define MAX_CARDS_NUM 108
#define TURNS_TO_EXPLODE 3

typedef enum playerType{PLAYER, COMPUTER, COMPUTER1, COMPUTER2} Type;
typedef enum direction {CLOCKWISE, COUNTER_CLOCKWISE} Direction;
typedef enum color {RED, BLUE, GREEN, YELLOW, BLACK} Color;
typedef enum card_name {ZERO, ONE, TWO, THREE, FOUR, FIVE,
    SIX, SEVEN, EIGHT, NINE, SKIP, REVERSE, PLUS, WILD} CardName;

/**
 * Card struct.
 */
typedef struct card {
    Color color;      /**< Color code for card */
    CardName name;    /**< Name code for card */
} Card;

/**
 * Deck struct.
 */
typedef struct deck {
    Card card;            /**< The current card this deck holds */
    struct deck * next;   /**< Pointer to the next deck item */
} Deck;

/**
 * Player struct.
 */
typedef struct player{
    int length;           /**< The length of the deck on the player's hand */
    int score;            /**< Current score */
    Deck *cards;          /**< Pointer to the player's deck on hand */
    Type type;            /**< The current card this deck holds */
}Player;  

void swap (Card *a, Card *b);
int shuffle(int length);
void print_card_name(Card a);
int create_cards(int rule);
int create_players(int num);
int set_up();
Deck* check_if_allNum(Deck *last);
void dealCard();
void sort(int length);
void quick_sort (Card a[], int length);
void drawCard(int numDraw);
int is_playable(Card c);
void processCard();
void next_player();
void free_deck(Deck* d);
int calc_point(Card c);
int pt_rand(int nbits);
void discard(Deck* previous, Deck* current);
int play_card_com();
int play_card_user(int index);
void update_score();
void save_game();
void delay(unsigned int secs);
void calc_cards();
void continue_saved_game();
int play_another_game();
#endif //UNO_FUNCTION_H

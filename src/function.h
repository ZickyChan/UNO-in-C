//
// Created by bit on 8/14/15.
//

#ifndef UNO_FUNCTION_H
#define UNO_FUNCTION_H

#define LINESIZE 325
#define MAX_CARDS_NUM 108
#define TURNS_TO_EXPLODE 3

typedef enum playerType{PLAYER, COMPUTER, COMPUTER1, COMPUTER2} Type;
typedef enum direction {CLOCKWISE, COUNTER_CLOCKWISE} Direction;
typedef enum color {RED, BLUE, GREEN, YELLOW, BLACK} Color;
typedef enum card_name {ZERO, ONE, TWO, THREE, FOUR, FIVE,
    SIX, SEVEN, EIGHT, NINE, SKIP, REVERSE, PLUS, WILD} CardName;

typedef struct card {
    Color color;
    CardName name;
} Card;

typedef struct deck {
    Card card;
    struct deck * next;
} Deck;

typedef struct player{
    int length;
    int score;
    Deck *cards;
    Type type;
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
#endif //UNO_FUNCTION_H

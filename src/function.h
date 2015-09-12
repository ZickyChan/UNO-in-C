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
 * Linked-list 
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

// Utils
/**
 * Swapping two Cards
 * @param *a address of the first card.
 * @param *b address of the sencond card.
 */
void swap (Card *a, Card *b);

/**
 * Shuffle the cards in remaining_pile
 * @param length current length of remaining_pile
 * @return the shuffle was successful or not
 */
int shuffle(int length);

/**
 * Print the card's name (Name + Color)
 * @param a card to be printed
 */
void print_card_name(Card a);

/**
 * Sort the cards on the current player's hand
 * Base on the type of the player and how many cards they have, the cards will be sorted differently
 * Turn the Deck (linked-list) into an array and feed it to quick_sort function
 * @param length the length of the player's deck of cards
 */
void sort(int length);

/**
 * Sort the cards using quick sort algorithm
 * @param a[] array of cards to be sorted
 * @param length the length of the array
 */
void quick_sort (Card a[], int length);

/**
 * Initialize all the cards, put it into the remaining pile and set up rule
 * @param rule which variation to be played
 * @return the initiation was successful or not
 */
int create_cards(int rule);

/**
 * Initialize players including user and computer players
 * @param num number of players to be created
 * @return the initiation was successful or not
 */
int create_players(int num);

/**
 * Let all players draw 1 card and decide who is the dealer
 * @return the set up was successful or not
 */
int set_up();

/**
 * Check if all the players drew a number card, any player that drew other cards
 * will have to redraw until they draw a number card
 * @param last pointer to the last card of the remaining_pile
 * @return pointer to the last card of the remaining_pile
 */
Deck* check_if_allNum(Deck *last);

/**
 * Deal 7 cards to players, starting from the dealer
 */
void dealCard();

/**
 * Deal card(s) to the current player
 * @param numDraw number of card(s) to be drawn
 */
void drawCard(int numDraw);

/**
 * Check if a card is currently playable or not
 * @param c card to be checked
 * @return whether c is playable or not
 */
int is_playable(Card c);

/**
 * Process the newly played card
 * Base on which rule is activated, the card will be processed differently
 */
void processCard();

/**
 * Update the currentPosition to the index of the next player
 */
void next_player();

/**
 * Recursively free the given deck
 * @param d deck to be freed
 */
void free_deck(Deck* d);

/**
 * Calculate the points given for one particular card
 * @param c card to be calculated
 * @return points given
 */
int calc_point(Card c);

/** 
 * Generate a random int between 0 and 2^(nbits-1)
 * @param nbits the number of bits to use
 * @return generated random int
 */
int pt_rand(int nbits);

/**
 * Put the played card into the discard_pile and update player's deck
 * @param previous pointer to the previous card of the played card
 * @param current pointer to the played card
 */
void discard(Deck* previous, Deck* current);

/**
 * Let the computer player plays a card
 * @return the computer player played a card or not
 */
int play_card_com();

/**
 * Let the user tries to play the chosen card
 * @param index the index of the chosen card
 * @return the user player played a card or not
 */
int play_card_user(int index);

/**
 * Update the score of the winning player at the end of each round
 */
void update_score();

/**
 * Save the game into a text file
 */
void save_game();

/**
 * Delay the game when the computer player plays
 * @param secs number of seconds to delay
 */
void delay(unsigned int secs);

/**
 * Read the save file, load all the players and cards.
 */
void continue_saved_game();

/**
 * Get all the cards back to the remaining pile
 */
int play_another_game();

void stop_game();
#endif //UNO_FUNCTION_H

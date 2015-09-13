//
// Created by bit on 8/14/15.
//

//http://stackoverflow.com/questions/1716013/why-is-scanf-causing-infinite-loop-in-this-code

#include "function.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "window.h"

// Global variables
Player *players;          /**< array of players */
Card current_card;        /**< last played card on table */
Deck *remaining_pile;     /**< remaining cards to draw */
Deck *discard_pile;       /**< discarded cards */
int currentPosition;      /**< index of current player in turn */
int numPlayers;           /**< number of players */
int direct = CLOCKWISE;   /**< current direction of the game */
int stacking=0;           /**< current stacked draws */
int variation=0;               /**< game mode, 0 for standard, 1 for stacking, 2 for time bomb */
int turns_left = -1;      /**< turns left until the 'bomb' explodes */

extern int haveToDraw;    
extern int winning_score; 
extern WINDOW *game;      
extern int gameX;
extern int gameY;


char *card_name[15] = {"0","1","2","3","4","5","6","7","8","9","Skip",
                       "Reverse", "Plus","Wild"};
char *color[5] = {"Red","Blue","Green","Yellow","Black"};


void swap(Card *a, Card *b) {
    Card temp = *a;
    *a = *b;
    *b = temp;
}

int shuffle(int length) {
    // reset seed
    srand(time(NULL));
    // copy the cards in remaining pile into a Card array
    Deck *current = remaining_pile;
    Card *array = malloc(sizeof(Card)*length);
    if (array==NULL) {
      free_deck(remaining_pile);
      return 1;
    }
    for (int i=0; i<length; i++) {
        array[i] = current->card;
        current = current->next;
    }
    
    // loop through the cards array and swap them randomly, using Fisher-Yates shuffle
    for (int i=0; i<length; i++) {
        // find a random index to swap with the current one
        int random_index = rand() % (length-i) +i;
        swap(&array[random_index],&array[i]);
    }

    // Put cards in array back to the remaining pile in the shuffled order
    current = remaining_pile;
    for (int i=0; i<length-1; i++) {
        current->card = array[i];
        current = current->next;
    }
    //To make sure that there is no memory spaces assigned to the next deck of the last element of the whole cards
    // Set the last elements's next deck to null
    current->card = array[length-1];
    current->next = NULL;
    free(array);
    return 0;
}

void print_card_name(Card a) {
    printf("%s %s\n",card_name[a.name], color[a.color]);
}

void sort(int length) {
  // Copy the cards in the current player's hand into array
  Deck *current = players[currentPosition].cards;
  Card *array = malloc(sizeof(Card)*length);
  for (int i=0; i<length; i++) {
      array[i] = current->card;
      current = current->next;
  }

  // Sort the array using quick sort algorithm
  quick_sort(array,length);

  // If the player has to many cards, all the playable cards will be placed on 
  // the top of that player's deck
  if (length>=26) {
    int index = 0;
    for (int i=0; i<length; i++) {
      if (is_playable(array[i])) {
        swap(&array[i], &array[index]);
        index++;
      }
    }
  }
  
  // Put cards back into player's deck
  current = players[currentPosition].cards;
  for (int i=0; i<length-1; i++) {
      current->card = array[i];
      current = current->next;
  }

  //To make sure that there is no memory spaces assigned to the next 
  //deck of the last element of the whole cards
  // Set the last elements's next deck to null
  current->card = array[length-1];
  current->next = NULL;
  free(array);
}

void quick_sort (Card a[], int length) {
  int left, right;
  Card pivot;

  // If the current player is the user or type 1 AI computer
  // Cards will be sorted by the color first and then by the name in ascending order
  if (players[currentPosition].type == PLAYER || players[currentPosition].type == COMPUTER1) {
    if (length > 1) {
      //Choose a pivot which is a random value, here I chose pivot is the mid element.
      pivot = a[length / 2];
      //The left is the first index, the right is the last index
      for (left = 0, right = length - 1; ; left++, right--) {
        //Find the element starting from the left index which is greater than pivot
        while ((a[left].color < pivot.color) || ((a[left].color == pivot.color)&&(a[left].name < pivot.name)))
            //Keep increasing the left index if a[left] < pivot
            left++;

        //Find the element starting from the right index which is smaller than pivot
        while ((pivot.color < a[right].color) || ((pivot.color == a[right].color) && (pivot.name < a[right].name) ))
            //Keep decreasing the right index if a[right] > pivot
            right--;

        //If left >= right then quit the loop
        if (left >= right)
            break;
        //Swap a[left] and a[right]
        swap(&a[left], &a[right]);
      }

      //Sort the array which contents the first element to the element at left index
      quick_sort(a, left);
      //Sort the array which contents the rest elements
      quick_sort(a + left, length - left);
    }
  } 
  // If the current player is type 2 AI computer
  // Cards will be sorted by name first and then by color in ascending order
  else if (players[currentPosition].type == COMPUTER2){
    // Apply the same principle but in reverse order
    if (length > 1) {
      pivot = a[length / 2];
      for (left = 0, right = length - 1; ; left++, right--) {
        while ((a[left].name > pivot.name) || ((a[left].name == pivot.name)&&(a[left].color > pivot.color)))
            left++;
        while ((pivot.name > a[right].name) || ((pivot.name == a[right].name) && (pivot.color > a[right].color) ))
            right--;
        if (left >= right)
            break;
        swap(&a[left], &a[right]);
      }
      quick_sort(a, left);
      quick_sort(a + left, length - left);
    }
  }
  // If the current player is type 0 AI
  // Cards will not be sorted
}

int create_cards(int rule){
    // Create all the cards and put them in remaining_pile iteratively
    // If it can't malloc at any point, we will free the deck and return 1
    Card card;
    card.color = RED;
    card.name = ZERO;
    remaining_pile = malloc(sizeof(Deck));
    if (remaining_pile == NULL)
      return 1;
    remaining_pile->card = card;
    remaining_pile->next = malloc(sizeof(Deck));
    Deck *current = remaining_pile->next;
    if (current == NULL) {
      free(remaining_pile);
      return 1;
    }
    for (int i=BLUE;i<=YELLOW; i++) {
        card.color = i;
        card.name = ZERO;
        current->card = card;
        current->next = malloc(sizeof(Deck));
        current = current->next;
        if (current == NULL) {
          free_deck(remaining_pile);
          return 1;
        }
    }
    for (int i=RED;i<=YELLOW; i++) {
        for (int j=ONE;j<WILD; j++) {
            for (int k = 0; k<= 1; k++) {
                card.color = i;
                card.name = j;
                current->card = card;
                current->next = malloc(sizeof(Deck));
                current = current->next;
                if (current == NULL) {
                  free_deck(remaining_pile);
                  return 1;
                }
            }
        }
    }
    for (int i=PLUS; i<=WILD;i++) {
        for (int j=0; j<4; j++) {
            //If the last element is assigned, make sure that there is no memory spaces assigned to the next deck of the last element of the whole cards
            // Set the last elements's next deck to null
            if(j==3 && i == WILD){
                card.color = BLACK;
                card.name = i;
                current->card = card;
                current->next=NULL;
            }
            else {
                card.color = BLACK;
                card.name = i;
                current->card = card;
                current->next = malloc(sizeof(Deck));
                current = current->next;
                if (current == NULL) {
                  free_deck(remaining_pile);
                  return 1;
                }
            }
        }
    }
    // Set the global game mode
    variation = rule;
    return 0;
}

int create_players(int num){
  // Allocate an array of players in the heap
  // Number of players is from user input
  numPlayers = num;
  players = malloc(sizeof(Player) * numPlayers);
  if (players == NULL) {
    free_deck(remaining_pile);
    return 1;
  }
  for (int i = 0; i < numPlayers; i++) {
        players[i].length = 0;
        players[i].score = 0;
        // The player at position 0 is always the user
        // Other players AI type will be distributed by computing i modulus 3
        players[i].type= (i==0) ? PLAYER:(i%3+1);
        players[i].cards = NULL;
    }
  return set_up();
}

int set_up() {
  // Let all players draw 1 card and put it into the bottom of remaining_pile
  discard_pile = NULL;
  Deck *last = remaining_pile;
  while (last->next != NULL) {
    last = last->next;
  }
  for (int i = 0; i < numPlayers; i++) {
    players[i].cards = malloc(sizeof(Deck));
    if (players[i].cards == NULL) {
      for (int j=0; j<i; j++) {
        free_deck(players[j].cards);
      }
      free_deck(remaining_pile);
      return 1;
    }
    players[i].cards->card = remaining_pile->card;
    Deck *previous = remaining_pile;
    remaining_pile = remaining_pile->next;
    previous->next = NULL;
    players[i].cards->next = NULL;
    last->next = previous;
    last = last->next;
  }

  // Check if all players drew a number card, players who drew non-number card
  // will have to draw again.
  last = check_if_allNum(last);

  // Find the dealer
  Player dealer;
  int check_duplicate = 2;
  int *duplicate_position = malloc(sizeof(int) * numPlayers);
  if (duplicate_position==NULL) {
    for (int i=0; i<numPlayers; i++) {
      free_deck(players[i].cards);
    }
    free_deck(remaining_pile);
    return 1;
  }
  for (int i = 0; i < numPlayers; i++) {
    duplicate_position[i] = i + 1;
  }
  int notNullLength = numPlayers;
  while (check_duplicate > 1) {
    check_duplicate = 0;
    dealer = players[duplicate_position[0] - 1];
    for (int i = 1; i < notNullLength; i++) {
      if (((dealer.cards)->card).name < ((players[duplicate_position[i] - 1].cards)->card).name) {
        dealer = players[duplicate_position[i] - 1];
      }
    }
    int j = 0;
    for (int i = 0; i < notNullLength; i++) {
      if (((dealer.cards)->card).name == ((players[duplicate_position[i] - 1].cards)->card).name) {
        duplicate_position[j] = duplicate_position[i];
        check_duplicate++;
        j++;
      }
    }
    notNullLength = j;
    for (int i = j; i < numPlayers; i++) {
      duplicate_position[i] = 0;
    }
    for (int i = 0; i < notNullLength; i++) {
      if (duplicate_position[1] == 0) {
        break;
      }
      else {
        free(players[duplicate_position[i] - 1].cards);
        players[duplicate_position[i] - 1].cards = malloc(sizeof(Deck));
        players[duplicate_position[i] - 1].cards->card = remaining_pile->card;
        players[duplicate_position[i] - 1].cards->next = NULL;
        Deck *previous = remaining_pile;
        remaining_pile = remaining_pile->next;
        previous->next = NULL;
        last->next = previous;
        last = last->next;
      }
    }
    last = check_if_allNum(last);
  }
  currentPosition = duplicate_position[0]-1;
  for (int i=0; i<numPlayers; i++) {
    free_deck(players[i].cards);
  }
  free(duplicate_position);
  dealCard();
  return 0;
}

Deck* check_if_allNum(Deck *last){
  int allNum = 0;
  while (allNum<numPlayers){
    // When all players drew a number card, quit the loop
    allNum = 0;
    for (int i = 0; i<numPlayers; i++) {
      if (players[i].cards->card.name <= NINE) {
        allNum++;
      }
      else {
        // If players[i]'s card is not a number card
        // redraw
        free(players[i].cards);
        players[i].cards = malloc(sizeof(Deck));
        players[i].cards->card = remaining_pile->card;
        players[i].cards->next = NULL;
        Deck *previous = remaining_pile;
        remaining_pile = remaining_pile->next;
        previous->next = NULL;
        last->next = previous;
        last = last->next;
      }
    }
  }
  // Update the pointer to the last card of remaining_pile
  return last;
}

void dealCard(){
  // Deal 7 cards to each player, 1 by 1
  // Use currentPosition and next player to deal card to players in order
  for(int j = 0; j < 7;j++) {
    for (int i = 0; i < numPlayers; i++) {
      if (j==0) {
        players[currentPosition].length++;
        Deck temp = *remaining_pile;
        Deck *current = remaining_pile;
        remaining_pile = remaining_pile->next;
        free(current);
        players[currentPosition].cards = malloc(sizeof(Deck));
        players[currentPosition].cards->card = temp.card;
        players[currentPosition].cards->next = NULL;
        next_player();
      } else {
        drawCard(1);
        next_player();
      }
    }
  }

  // Use currentPosition and next player to sort card of players in order
  // Because our sort function take the deck of the current player to sort only
  for (int i = 0; i < numPlayers; i++){
    sort(players[currentPosition].length);
    next_player();
  }
}

int play_card_com() {
  // This function is for AI computer player
  // Loop through the current player's deck and play the first playable card
  // current and previous are used to keep track of what card will be played and later
  // update the player's deck
  Deck *current = players[currentPosition].cards;
  Deck *previous = NULL;
  int is_played = 0;
  while (current != NULL) {
    if (is_playable(current->card) == 1) {
      // if current card is playable, update is_played and current_card on table
      current_card = current->card;
      is_played = 1;
      break;
    } else {
      // continue looping
      previous = current;
      current = current->next;
    }
  }

  if (is_played == 1) {
    // Standard mode
    // If player played some card, put that card into discard_pile
    discard(previous,current);
    if(players[currentPosition].length>0)
      // If the player hasn't won (length of deck > 0), process the played card and continue playing
      processCard();
  }
  else if (is_played == 0 && variation == 1 && stacking != 0){
    // Stacking mode
    // if player can't play and stacking count is not 0, it must draw and update 
    // the stack count and UI, skip to next player
    drawCard(stacking);
    stacking = 0;
    mvwprintw(game, 4 + (currentPosition - 1), 1, "P%d: %d cards ", currentPosition, players[currentPosition].length);
    next_player();
  }
  else if (is_played == 0 && variation == 2 && turns_left == 0) {
    // Timebomb mode
    // if player can't play and turns_left is 0 meaning it's time to "explode"
    // the player will have to draw current stacking count and update stack count
    // turns_left, UI and skip to next player
    drawCard(stacking);
    stacking = 0;
    turns_left--;
    mvwprintw(game,4+(currentPosition-1),1,"P%d: %d cards ", currentPosition,players[currentPosition].length);
    next_player();
  } else {
    // If player can't play and doesn't need to draw
    // Draw 1 card and check for a playable card to play
    drawCard(1);
    mvwprintw(game,4+(currentPosition-1),1,"P%d: %d cards ", currentPosition,players[currentPosition].length);
    previous = NULL;
    current = players[currentPosition].cards;
    while (current != NULL) {
      if (is_playable(current->card)) {
        current_card = current->card;
        is_played++;
        break;
      } else {
        previous = current;
        current = current->next;
      }
    }
    if(is_played == 0){
      if (variation == 2 && turns_left!=-1)
        turns_left--;
      next_player();
    }
    else{
      discard(previous,current);
      processCard();
    }
  }
  // Check if player played any card 
  return is_played;
}

int is_playable(Card c) {
  // Compare card c with current_card, based on which mode is playing to
  // determine wheter c is playable or not.
  if (variation == 1 && stacking != 0) {
    if (c.name == PLUS) {
      // Player need to play draw card that match the new color if it was draw 4 card
      if (discard_pile->card.color==BLACK && c.color != current_card.color)
        return 0;
      else
        return 1;
    }
    else 
      return 0;
  }
  else if (variation == 2 && turns_left == 0) {
    if (c.name == REVERSE || c.name == SKIP) {
      // Player try hopelessly to avoid drawing a bunch of cards
      turns_left++;
      return 1;
    } else
      return 0;
  }
  else if (variation == 2 && players[currentPosition].length == 1) {
    // In timebomb mode, player can't win a game by playing a Black card last
    if (c.color == current_card.color)
      return 1;
    else if (c.color == BLACK)
      return 0;
    else if (c.name == current_card.name)
      return 1;
    else 
      return 0;
  }
  else {
    // Standard rule
    if(c.color == current_card.color) {
      // same color
      return 1;
    } else if (c.name == current_card.name) {
      // same name
      return 1;
    } else if (c.color == BLACK) {
      // wild or draw 4
      return 1;
    } else {
      // can't play
      return 0;
    }
  }
}

void discard(Deck *previous, Deck *current){
    // Update player's deck length, UI and update score if the player wins.
    players[currentPosition].length--;
    if (players[currentPosition].length == 1) {
        if(currentPosition != 0) {
            mvwprintw(game, 1, gameX / 2 - 11, "Player %d says: UNO!!!", currentPosition);
        }
        else{
            mvwprintw(game, 1, gameX / 2 - 11, "   You say: UNO!!!   ", currentPosition);
        }
    } else if (players[currentPosition].length == 0)
        update_score();

    // Put the played card into discard_pile
    if (previous == NULL) 
        // If the played card is the first card on player's deck
        players[currentPosition].cards = current->next;
    else
        // Cut current card out of player's deck
        previous->next = current->next;
    current->next = discard_pile;
    discard_pile = current;
}

void drawCard(int numDraw){
    // Make the current player draw the given number of cards 1 by 1
    for (int i =0; i<numDraw; i++) {
        if(remaining_pile==NULL){
          // If there is no card left in remaining_pile
          // Take all the cards from discard_pile, put them in remaining_pile
          remaining_pile = discard_pile;
          discard_pile = NULL;
          int pile_len = 0;
          Deck *current = remaining_pile;
          while (current != NULL) {
              pile_len++;
              current = current->next;
          }
          // shuffle again
          shuffle(pile_len);
        }
        Deck *current = malloc(sizeof(Deck));
        current->card = remaining_pile->card;
        current->next = players[currentPosition].cards;
        players[currentPosition].cards = current;
        current = remaining_pile;
        remaining_pile = remaining_pile->next;
        free(current);
        players[currentPosition].length++;
    }
    // after drawing, sort the deck again
    sort(players[currentPosition].length);
}

void processCard(){
  // update game status according to the played card and playing mode
    if (variation == 2 && turns_left!=-1) {
      // every turn in time bomb mode and bomd is deployed, update turns_left
      turns_left--;
    }
    if(current_card.name == REVERSE){
      // Change the direction of the game
        direct = (direct+1)%2;
    }
    else if(current_card.name == PLUS){
        if (variation == 1) { // Stacking mode
            // Add stacking when the game is in stacking mode
            stacking += (current_card.color == BLACK) ? 4:2;
            if (players[currentPosition].type == PLAYER && current_card.color == BLACK) {
                // if player played a plus 4 card, update the UI so that the player
                // can choose the color
                current_card.color = choose_card_color();
            }
        }
        else if (variation == 2) { // Time bomb mode
          // In time bomb mode, this will deploy the bomb if it hasn't
          turns_left = (turns_left==-1) ? 2:turns_left;

          // Update stack count
          stacking += (current_card.color == BLACK) ? 4:2;
          if (players[currentPosition].type == PLAYER && current_card.color == BLACK) {
              current_card.color =  choose_card_color();
          }
        } else { // Standard mode
            if (current_card.color == BLACK) {
                if (players[currentPosition].type >= COMPUTER) {
                    // If current player is AI computer, it will pick the color randomly
                    current_card.color = pt_rand(2);
                } else {
                    current_card.color = choose_card_color();
                }
            }
            // next player have to draw cards
            next_player();
            if (current_card.color == BLACK) {
                drawCard(4);
                // Update UI according to player type
                if(players[currentPosition].type==PLAYER){
                   printCard();
                } else{
                    mvwprintw(game,4+(currentPosition-1),1,"P%d: %d cards ", currentPosition,players[currentPosition].length);
                }
            }
            else{
                drawCard(2);
                if(players[currentPosition].type==PLAYER){
                   printCard();
                }
                else{
                    mvwprintw(game,4+(currentPosition-1),1,"P%d: %d cards ", currentPosition,players[currentPosition].length);
                }
            }
        }
    }
    else if (current_card.name == SKIP) {
        // Skip this player
        next_player();
    }
    else if (current_card.name == WILD){
        if (players[currentPosition].type>=COMPUTER){
            current_card.color = pt_rand(2);
        }
        else{
            current_card.color = choose_card_color();
        }
    }
    // Update currentPosition to next player
    next_player();
}

void next_player() {
    // increase or decrease currentPosition by one base on the direction of the game
    // modulus with numPlayers to not going out of range 
    if (direct == CLOCKWISE)
        currentPosition = (currentPosition + 1) % numPlayers;
    else
        currentPosition = ((currentPosition - 1) + numPlayers) % numPlayers;
}

void free_deck(Deck* d) {
  // Recursively free the deck
  if (d != NULL) {
    free_deck(d->next);
    free(d);
  }
}

int calc_point(Card c) {
  // Calculate points base on the rule
  if (c.name <= NINE)
    // number cards will return their face value point 
    return c.name;
  else if (c.color == BLACK)
    // Black cards
    return 50;
  else
    // Skip, Reverse and Draw 2
    return 20;
}

int pt_rand(int nbits) {
    int mask;
    if (0 < nbits && nbits < sizeof(int)*8) {
        // the least significant nbits bits will be 1, others will be 0
        mask = ~(~((unsigned int) 0) << nbits); // << is left flip
    }
    else {
        // the mask will be all ones
        mask = ~((unsigned int) 0);
    }
    // get the next random number and keep only nbits bits
    return rand() & mask;
}

int play_card_user(int index) {
    // Play cards base on user choice
    if(haveToDraw == 1){
        // In stacking or time bomb mode, if user clicks skip, he/she hast to draw
        // update game status accordingly
        drawCard(stacking);
        stacking = 0;
        haveToDraw = 0;
        if (variation == 2) {
          turns_left--;
        }
        printCard();
        next_player();
        return 0;
    }
    else {
        // Loop through the user deck to the particular index and check if that
        // card is playable
        Deck *previous = NULL;
        Deck *current = players[currentPosition].cards;
        for (int i = 0; i < index; i++) {
            previous = current;
            current = current->next;
        }
        int canPlay = is_playable(current->card);
        // Update UI 
        if (canPlay == 1) {
            current_card = current->card;
            if(current_card.color == BLACK && current_card.name == PLUS){
                canPlay++;
            }
            discard(previous, current);
            if (players[currentPosition].length > 0)
                processCard();
        }
        return canPlay;
    }

}

void update_score() {
  // When a player win a round, update the score of that player base on the rule
  int score = players[currentPosition].score;
  for (int i=0; i<numPlayers; i++) {
    if (i != currentPosition) {
      Deck *current = players[i].cards;
      while (current != NULL) {
        score += calc_point(current->card);
        current = current->next;
      }
    }
  }
  players[currentPosition].score = score;
}

void save_game() {
  // Create file in folder saves
  // Reference: http://stackoverflow.com/questions/16153477/how-to-create-a-file-in-a-specific-directory
  char serv_name[1000];
  mkdir("newdir", S_IRWXU | S_IRWXG | S_IRWXO);
  snprintf(serv_name, sizeof(serv_name), "saves/uno.save");
  FILE *f = fopen(serv_name, "w");
  if (f==NULL) {
    perror("Client: \n");
    exit(1);
  }

  // Write all the necessary data into the created file
  // game mode, game status, players' data and deck, remaining_pile and discard_pile
  if (variation == 1)
    fprintf(f, "1\t%d\n", stacking);
  else if (variation == 2)
    fprintf(f, "2\t%d\t%d\n", stacking, turns_left);
  else 
    fprintf(f, "0\n");
  fprintf(f, "%d\t%d\t%d\t%d\t%d\t%d\n", numPlayers, currentPosition, direct, current_card.color, current_card.name, winning_score);
  for (int i=0; i<numPlayers; i++) {
    fprintf(f, "%d\t%d\t%d", players[i].length, players[i].score, players[i].type);
    Deck *current = players[i].cards;
    while (current != NULL) {
      fprintf(f, "\t%d\t%d", current->card.color, current->card.name);
      current = current->next;
    }
    fprintf(f, "\n");
  }

  Deck *current = remaining_pile;
  while (current != NULL) {
    if (current->next != NULL) {
        fprintf(f, "%d\t%d\t",current->card.color, current->card.name);
    } else {
        fprintf(f, "%d\t%d",current->card.color, current->card.name);
    }
    current = current->next;
  }
  fprintf(f,"\n");
  current = discard_pile;
  while (current != NULL) {
    if (current->next != NULL) {
      fprintf(f, "%d\t%d\t",current->card.color, current->card.name);
    } else {
      fprintf(f, "%d\t%d",current->card.color, current->card.name);
    }
    current = current->next;
  }
  fprintf(f,"\n");
  fclose(f);
}

void delay(unsigned int secs) {
  // Delay function to simulate computer thinking method

  // Initialize 2 clocks
  clock_t start_c = clock();
  clock_t end_c = clock();

  // Keep renewing end_c until secs past
  while ((end_c-start_c)/1000000.0 < secs) {
    end_c = clock();
  }
}

int continue_saved_game() {
  // Open save file and read data
  // References: http://stackoverflow.com/questions/12499219/using-fgets-and-strtok-to-read-in-a-file-line-by-line-in-c
  // At any point when it is unable to malloc, return 1
  char input[LINESIZE];
  FILE *f = fopen("saves/uno.save","r");

  // Read first line, update game mode and specific game status
  fgets(input, LINESIZE, f);
  // Use strtok to take the integer from the read line
  variation = atoi(strtok(input, "\t"));
  if (variation == 1) {
    stacking = atoi(strtok(NULL, "\t")); 
  } else if (variation == 2) {
    stacking = atoi(strtok(NULL, "\t"));
    turns_left = atoi(strtok(NULL, "\t"));
  }

  // Read second line, read standard game status
  fgets(input, LINESIZE, f);
  numPlayers = atoi(strtok(input, "\t"));
  currentPosition = atoi(strtok(NULL, "\t"));
  direct = atoi(strtok(NULL, "\t"));
  current_card.color = atoi(strtok(NULL, "\t"));
  current_card.name = atoi(strtok(NULL, "\t"));
  winning_score = atoi(strtok(NULL, "\t"));

  // Create players, and players' deck
  players = malloc(sizeof(Player)*numPlayers);
  if (players == NULL)
    return 1;
  for (int i=0; i<numPlayers; i++) {
    fgets(input, LINESIZE, f);
    players[i].length = atoi(strtok(input, "\t"));
    players[i].score = atoi(strtok(NULL,"\t"));
    players[i].type = atoi(strtok(NULL,"\t"));
    if (players[i].length > 0) {
      // If there were cards in players' deck when save
      players[i].cards = malloc(sizeof(Deck));
      if (players[i].cards == NULL) {
        for (int j=0; j<i; j++) {
          free_deck(players[j].cards);
        }
        free(players);
        return 1;
      }
    } else {
      players[i].cards = NULL;
    }
    Deck *current = players[i].cards;
    for (int j=0; j<players[i].length; j++) {
      current->card.color = atoi(strtok(NULL,"\t"));
      current->card.name = atoi(strtok(NULL,"\t"));
      if (j < players[i].length-1) {
        current->next = malloc(sizeof(Deck));
        if (current->next == NULL) {
          free_deck(players[i].cards);
          for (int j=0; j<i; j++) {
            free_deck(players[j].cards);
          }
          free(players);
          return 1;
        }
        current = current->next;
      } else {
        current->next = NULL;
        break;
      }
    }
  }

  // Re-create remaining_pile
  fgets(input, LINESIZE, f);
  if (strcmp(input,"\n")!=0) {
    // If there were cards in remaining_pile when save
    remaining_pile = malloc(sizeof(Deck));
    if (remaining_pile == NULL) {
      for (int i=0; i<numPlayers; i++) {
        free_deck(players[i].cards);
      }
      free(players);
      return 1;
    }
    Deck *current = remaining_pile;
    char *token = strtok(input,"\t");
    current->card.color = atoi(token);
    token = strtok(NULL,"\t");
    current->card.name = atoi(token);
    current->next = NULL;
    token = strtok(NULL,"\t");
    while (token != NULL) {
      current->next = malloc(sizeof(Deck));
      if (current->next == NULL) {
        for (int i=0; i<numPlayers; i++) {
          free_deck(players[i].cards);
        }
        free(players);
        free_deck(remaining_pile);
        return 1;
      }
      current = current->next;
      current->card.color = atoi(token);
      token = strtok(NULL,"\t");
      current->card.name = atoi(token);
      token = strtok(NULL,"\t");
      current->next = NULL;
    }
  } else {
    remaining_pile = NULL;
  }

  // Re-create discard_pile
  fgets(input,LINESIZE,f);
  if (strcmp(input,"\n")!=0) {
    // If there were cards in discard_pile when save
    discard_pile = malloc(sizeof(Deck));
    if (discard_pile == NULL) {
      for (int i=0; i<numPlayers; i++) {
        free_deck(players[i].cards);
      }
      free(players);
      free_deck(remaining_pile);
      return 1;
    }
    Deck *current = discard_pile;
    char *token = strtok(input,"\t");
    current->card.color = atoi(token);
    token = strtok(NULL,"\t");
    current->card.name = atoi(token);
    current->next = NULL;
    token = strtok(NULL,"\t");
    while (token != NULL) {
      current->next = malloc(sizeof(Deck));
      if (current->next == NULL) {
        for (int i=0; i<numPlayers; i++) {
          free_deck(players[i].cards);
        }
        free(players);
        free_deck(remaining_pile);
        free_deck(discard_pile);
        return 1;
      }
      current = current->next;
      current->card.color = atoi(token);
      token = strtok(NULL,"\t");
      current->card.name = atoi(token);
      token = strtok(NULL,"\t");
      current->next = NULL;
    }
  } else {
    discard_pile = NULL;
  }
  fclose(f);
  return 0;
}

int play_another_game() {
  // Put cards in discard_pile and players' hand back into remaining_pile and
  // set up again
  Deck *current = remaining_pile;
  while (current->next != NULL) {
    current = current->next;
  }
  for (int i=0; i<numPlayers; i++) {
    players[i].length = 0;
    if (players[i].cards != NULL) {
      current->next = players[i].cards;
      players[i].cards = NULL;
      while (current->next != NULL) 
        current = current->next;
    }
  }
  current->next = discard_pile;
  return set_up();
}

void stop_game() {
  // Free all allocated memories
  free_deck(remaining_pile);
  free_deck(discard_pile);
  for (int i=0; i<numPlayers; i++) {
    free_deck(players[i].cards);
  }
  free(players);
}
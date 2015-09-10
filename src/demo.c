#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "function.h"

extern Player *players;
extern Card current_card;
extern Deck *remaining_pile;
extern Deck *discard_pile;
extern int currentPosition;
extern int numPlayers;

int main() {
  /*//This code to make the random number in the computer will be different, not return to the default
  //So that we could have different array while running the code
  time_t current_time = time(NULL);
  srand(current_time);*/

  create_cards(0);
  shuffle(108);
  create_players(4);
  /*current = head;
  while (current !=NULL) {
    print_card_name(current->card);
    current = current->next;
  }*/
  Deck *current = remaining_pile;
  current_card = remaining_pile->card;
  remaining_pile = remaining_pile->next;
  free(current);
  discard_pile = malloc(sizeof(Deck));
  discard_pile->card = current_card;
  discard_pile->next = NULL;
  print_card_name(current_card);
  int abc = 0;
  processCard();
  printf("currentPosition %d\n", currentPosition);
  while (players[currentPosition].length > 0) {
    abc++;
    printf("Player %d's turn: len %d\n",currentPosition,players[currentPosition].length);
    Deck *current = players[currentPosition].cards;
    while (current!=NULL) {
      print_card_name(current->card);
      current = current->next;
    }
    int temp = currentPosition;
    if (play_card_com()==1) {
      printf("Player %d played: ",temp);
      print_card_name(current_card);
    }
    else {
      printf("No playable card.\n");
    }
    printf("Current card on the table is: ");
    print_card_name(current_card);
    printf("\n");
    if (abc == 20) {
      break;
    }
  }
  save_game();
  printf("save game\n");
  continue_saved_game();
  printf("continue\n");
  while (players[currentPosition].length > 0) {
    printf("Player %d's turn: len %d\n",currentPosition,players[currentPosition].length);
    Deck *current = players[currentPosition].cards;
    while (current!=NULL) {
      print_card_name(current->card);
      current = current->next;
    }
    int temp = currentPosition;
    if (play_card_com()==1) {
      printf("Player %d played: ",temp);
      print_card_name(current_card);
    }
    else {
      printf("No playable card.\n");
    }
    printf("Current card on the table is: ");
    print_card_name(current_card);
    printf("\n");
  } 
}
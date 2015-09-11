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


Player *players;
Card current_card;
Deck *remaining_pile;
Deck *discard_pile;
int currentPosition;
int numPlayers;
int direct = CLOCKWISE;
int stacking=0;
int is_stacking=0;
int is_time_bomb = 0;
int turns_left = -1;

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
    for (int i=0; i<length; i++) {
        // find a random index to swap with the current one
        int random_index = rand() % (length-i) +i;
        swap(&array[random_index],&array[i]);
    }
    free_deck(remaining_pile);
    remaining_pile = malloc(sizeof(Deck));
    current = remaining_pile;
    for (int i=0; i<length-1; i++) {
        current->card = array[i];
        current->next = malloc(sizeof(Deck));
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
  Deck *current = players[currentPosition].cards;
  Card *array = malloc(sizeof(Card)*length);
  for (int i=0; i<length; i++) {
      array[i] = current->card;
      current = current->next;
  }

  quick_sort(array,length);
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
  } else if (players[currentPosition].type == COMPUTER2){
    if (length > 1) {
      //Choose a pivot which is a random value, here I chose pivot is the mid element.
      pivot = a[length / 2];
      //The left is the first index, the right is the last index
      for (left = 0, right = length - 1; ; left++, right--) {
        //Find the element starting from the left index which is greater than pivot
        while ((a[left].name > pivot.name) || ((a[left].name == pivot.name)&&(a[left].color > pivot.color)))
            //Keep increasing the left index if a[left] < pivot
            left++;

        //Find the element starting from the right index which is smaller than pivot
        while ((pivot.name > a[right].name) || ((pivot.name == a[right].name) && (pivot.color > a[right].color) ))
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
}

int create_cards(int rule){
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
    if (rule == 1) {
      is_stacking = 1;
    } else if (rule == 2) {
      is_time_bomb = 1;
    }
    return 0;
}

int create_players(int num){
  numPlayers = num;
  players = malloc(sizeof(Player) * numPlayers);
  if (players == NULL) {
    free_deck(remaining_pile);
    return 1;
  }
  for (int i = 0; i < numPlayers; i++) {
        players[i].length = 0;
        players[i].score = 0;
        players[i].type= (i==0) ? PLAYER:(i%3+1);
        players[i].cards = NULL;
    }
  return set_up();
}

int set_up() {
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
    (players[i].cards)->next = NULL;
    last->next = previous;
    last = last->next;
  }
  last = check_if_allNum(last);
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
    allNum = 0;
    for (int i = 0; i<numPlayers; i++) {
      if (((players[i].cards)->card).name <= NINE) {
        allNum++;
      }
      else {
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
  return last;
}

void dealCard(){
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

  for (int i = 0; i < numPlayers; i++){
    sort(players[currentPosition].length);
    next_player();
  }
}

int play_card_com() {
  Deck *current = players[currentPosition].cards;
  Deck *previous = NULL;
  int is_played = 0;
  while (current != NULL) {
    if (is_playable(current->card) == 1) {
      current_card = current->card;
      is_played = 1;
      break;
    } else {
      previous = current;
      current = current->next;
    }
  }
  if (is_played == 1) {
    discard(previous,current);
    if(players[currentPosition].length>0)
      processCard();
  }
  else if (!is_played && is_stacking && stacking != 0){
    drawCard(stacking);
    stacking = 0;
    next_player();
  }
  else if (!is_played && is_time_bomb && turns_left == 0) {
    drawCard(stacking);
    stacking = 0;
    turns_left--;
    next_player();
  } else {
    drawCard(1);
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
      if (is_time_bomb && turns_left!=-1)
        turns_left--;
      next_player();
    }
    else{
      discard(previous,current);
      if(players[currentPosition].length>0)
          processCard();
    }
  }
  return is_played;
}

int is_playable(Card c) {
  if (is_stacking && stacking != 0) {
    if (c.name == PLUS) 
      return 1;
    else 
      return 0;
  }
  else if (is_time_bomb && turns_left == 0) {
    if (c.name == REVERSE || c.name == SKIP) {
      turns_left++;
      return 1;
    } else
      return 0;
  } else if (is_time_bomb && players[currentPosition].length == 1) {
    if (c.color == current_card.color)
      return 1;
    else if (c.color == BLACK)
      return 0;
    else if (c.name == current_card.name)
      return 1;
    else 
      return 0;
    }
  } else {
    if(c.color == current_card.color) {
      return 1;
    }
    else if (c.name == current_card.name) {
      return 1;
    } else if (c.color == BLACK) {
      return 1;
    } else {
      return 0;
    }
  }
}

void discard(Deck *previous, Deck *current){
    players[currentPosition].length--;
    if (players[currentPosition].length == 1) {
        //printf("UNO\n");
        if(currentPosition != 0) {
            mvwprintw(game, 5, gameX / 2 - 11, "Player %d says: UNO!!!", currentPosition);
        }
        else{
            mvwprintw(game, 5, gameX / 2 - 11, "   You say: UNO!!!   ", currentPosition);
        }
    }
    else if (players[currentPosition].length == 0)
        printf("Player %d wins!\n", currentPosition);
    if (previous == NULL) {
        players[currentPosition].cards = current->next;
        free(current);
        current = malloc(sizeof(Deck));
        current->card = current_card;
        current->next = discard_pile;
        discard_pile = current;
    } else {
        previous->next = current->next;
        free(current);
        current = malloc(sizeof(Deck));
        current->card = current_card;
        current->next = discard_pile;
        discard_pile = current;
    }
}

void drawCard(int numDraw){
    for (int i =0; i<numDraw; i++) {
        if(remaining_pile==NULL){
            remaining_pile = discard_pile;
            discard_pile = NULL;
            int pile_len = 0;
            Deck *current = remaining_pile;
            while (current != NULL) {
                pile_len++;
                current = current->next;
            }
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
    sort(players[currentPosition].length);
}

void processCard(){
    if (is_time_bomb && turns_left!=-1) {
      turns_left--;
    }
    if(current_card.name == REVERSE){
        direct = (direct+1)%2;
    }
    else if(current_card.name == PLUS){
        if (is_stacking == 1) {
            stacking += (current_card.color == BLACK) ? 4:2;
        }
        else if (is_time_bomb) {
          turns_left = (turns_left==-1) ? 2:turns_left;
          stacking += (current_card.color == BLACK) ? 4:2;
        }
        else {
            if (current_card.color == BLACK) {
                if (players[currentPosition].type >= COMPUTER) {
                    current_card.color = pt_rand(2);
                }
                    //for player
                else {
                    current_card.color = choose_card_color();
                }
            }
            next_player();
            if (current_card.color == BLACK) {
                drawCard(4);
                if(players[currentPosition].type==PLAYER){
                   printCard();
                }
            }
            else{
                drawCard(2);
                if(players[currentPosition].type==PLAYER){
                   printCard();
                }
            }
        }
    }
    else if (current_card.name == SKIP) {
        next_player();
    }
    else if (current_card.name == WILD){
        if (players[currentPosition].type>=COMPUTER){
            current_card.color = pt_rand(2);
        }
            //for player
        else{
            current_card.color = choose_card_color();
        }
    }
    next_player();
}

void next_player() {
    if (direct == CLOCKWISE)
        currentPosition = (currentPosition + 1)%numPlayers;
    else
        currentPosition = ((currentPosition - 1) + numPlayers) % numPlayers;
}

void free_deck(Deck* d) {
  if (d != NULL) {
    free_deck(d->next);
    free(d);
  }
}

int calc_point(Card c) {
  if (c.name <= NINE)
    return c.name;
  else if (c.name < PLUS)
    return 20;
  else if (c.name == PLUS) {
    if (c.color == BLACK)
      return 50;
    else 
      return 20;
  }
  else
    return 50;
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
    if(haveToDraw == 1){
        drawCard(stacking);
        stacking = 0;
        haveToDraw = 0;
        printCard();
        next_player();
        return 0;
    }
    else {
        Deck *previous = NULL;
        Deck *current = players[currentPosition].cards;
        for (int i = 0; i < index; i++) {
            previous = current;
            current = current->next;
        }
        if (is_playable(current->card) == 1) {
            current_card = current->card;
            discard(previous, current);
            if (players[currentPosition].length > 0)
                processCard();
        }
        return is_playable(current->card);
    }

}

void update_score() {
  int score = 0;
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
  char serv_name[1000];
  snprintf(serv_name, sizeof(serv_name), "saves/uno.save", getpid());
  FILE *f = fopen(serv_name, "w");
  if (f==NULL) {
    perror("Client: \n");
    exit(1);
  }
  if (is_stacking)
    fprintf(f, "1\t%d\n", stacking);
  else if (is_time_bomb)
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
  free_deck(remaining_pile);
  free_deck(discard_pile);
  for (int i=0; i<numPlayers; i++) {
    free_deck(players[i].cards);
  }
  free(players);
  fclose(f);
}

void delay(unsigned int secs) {
  clock_t start_c = clock();
  clock_t end_c = clock();
  while ((end_c-start_c)/1000000.0 < secs) {
    end_c = clock();
  }
}

void calc_cards() {
  int sum = 0;
  if (players != NULL) {
    for (int i=0; i<numPlayers; i++) {
      Deck *current = players[i].cards;
      while (current != NULL) {
        sum++;
        printf("%d\n", sum);
        current = current->next;
      }
    }
  }
  Deck *current = discard_pile;
  while (current != NULL) {
    sum++;
    printf("%d\n", sum);
    current = current->next;
  }
  current = remaining_pile;
  while (current != NULL) {
    sum++;
    printf("%d\n", sum);
    current = current->next;
  }
}

void continue_saved_game() {
  char input[LINESIZE];
  FILE *f = fopen("saves/uno.save","r");
  fgets(input, LINESIZE, f);
  int rule = atoi(strtok(input, "\t"));
  if (rule == 1) {
    is_stacking = 1;
    stacking = atoi(strtok(NULL, "\t")); 
  } else if (rule == 2) {
    is_time_bomb = 1;
    stacking = atoi(strtok(NULL, "\t"));
    turns_left = atoi(strtok(NULL, "\t"));
  }
  fgets(input, LINESIZE, f);
  numPlayers = atoi(strtok(input, "\t"));
  currentPosition = atoi(strtok(NULL, "\t"));
  direct = atoi(strtok(NULL, "\t"));
  current_card.color = atoi(strtok(NULL, "\t"));
  current_card.name = atoi(strtok(NULL, "\t"));
  winning_score = atoi(strtok(NULL, "\t"));

  players = malloc(sizeof(Player)*numPlayers);
  for (int i=0; i<numPlayers; i++) {
    fgets(input, LINESIZE, f);
    players[i].length = atoi(strtok(input, "\t"));
    players[i].score = atoi(strtok(NULL,"\t"));
    players[i].type = atoi(strtok(NULL,"\t"));
    if (players[i].length > 0) {
      players[i].cards = malloc(sizeof(Deck));
    } else {
      players[i].cards = NULL;
    }
    Deck *current = players[i].cards;
    for (int j=0; j<players[i].length; j++) {
      current->card.color = atoi(strtok(NULL,"\t"));
      current->card.name = atoi(strtok(NULL,"\t"));
      if (j < players[i].length-1) {
        current->next = malloc(sizeof(Deck));
        current = current->next;
      } else {
        current->next = NULL;
        break;
      }
    }
  }
  fgets(input, LINESIZE, f);
  if (strcmp(input,"\n")!=0) {
    remaining_pile = malloc(sizeof(Deck));
    Deck *current = remaining_pile;
    char *token = strtok(input,"\t");
    current->card.color = atoi(token);
    token = strtok(NULL,"\t");
    current->card.name = atoi(token);
    current->next = NULL;
    token = strtok(NULL,"\t");
    while (token != NULL) {
      current->next = malloc(sizeof(Deck));
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

  fgets(input,LINESIZE,f);
  if (strcmp(input,"\n")!=0) {
    discard_pile = malloc(sizeof(Deck));
    Deck *current = discard_pile;
    char *token = strtok(input,"\t");
    current->card.color = atoi(token);
    token = strtok(NULL,"\t");
    current->card.name = atoi(token);
    current->next = NULL;
    token = strtok(NULL,"\t");
    while (token != NULL) {
      current->next = malloc(sizeof(Deck));
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
}

int play_another_game() {
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
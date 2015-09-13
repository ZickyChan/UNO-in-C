README file

Data-structure:
Card struct contain two enum Card color and Card name.
We use linked list to store the deck of cards. Deck will contain a card (value) and the pointer to the next Deck.
Player struct contain Deck’s length, player’s score and player’s Deck of cards.

Algorithm:
Shuffle the remaining pile with Fisher-Yates shuffle algorithm.
Sort cards with quick sort and playable sort (implemented by us).
We have a global variable current_card to store the last played card on table, check the chosen card color and name with that current_card color and name to determine whether it is playable or not.

AI players:
All AI players will play the first card they found that is playable but base on the sort function there will be 3 types of AI:
Type 1: non-sorted Deck meaning this AI type player will play cards at random.
Type 2: cards are sorted by color and name in ascending order meaning this AI type will keep the “function card” like skip, reverse, draw 2 for later and Black cards are used last for easy win.
Type 3: cards are sorted by name and color in descending order meaning this AI type will always play the “functional card” first.

Variations (Game modes):
Stacking UNO: players can stack the draw 2 and draw 4 cards until some player can’t play another draw card then that player has to draw all the stacked cards.
Timebomb UNO: When a player plays a draw card, the next player will not draw immediately but it will deploy a “bomb” and in 3 turns this “bomb” will explode and make the that player draws. In 3 turns when the “bomb” is counting down, players can play draw cards to add to the stack. When the “bomb” explodes, players can use skip or reverse to make “bomb” explodes at another location.

Extra function:
We use mouse event to get the user input instead of normal keyboard input.

** ISSUES:
The terminal is automatically set at full screen. However, the user can type F11 to resize the terminal. If the terminal resizes, the displayment will be broken. It doesn’t cause the program crash but the program can’t run correctly.
The displayment is set probably with our laptop size. It could be mixed up in other laptops. It could be solved by change the font size of the terminal to make sure that the terminal have 105 columns and 26 lines. (Go to conf/xfce4/terminal/terminalrc to change the font size)

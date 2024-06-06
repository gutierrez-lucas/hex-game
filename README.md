# Hex Game

## Get project
Download form GitLab GUI or enter from a terminal:
```
git clone https://gitlab.com/GutierrezJLucas/hex-game.git
```
## Run
Using Linux in a terminal enter:
```
./main.o
```
Using Windows in a terminal enter:
```
.\main.exe
```
## Compilation and run

Using GCC and Linux compile and run using:
```
gcc main.cpp -lstdc++ -Wall -o main.o && ./main.o
```

Using g++ and Windows compile and run using:
```
g++ main.cpp -static -w -Wall -Wextra -o main.exe ; .\main.exe
```

## Description
This proyects implements a HEX multi-player game in C++. Meaning that it doesn't has any IA and you can use it to play with another person or for testing pourpuses. \
You can check the game rules [here](https://www.wikiwand.com/en/Hex_(board_game)).

## Usage
It can be used in two modes: testing and manual.

### Manual
Use the follow function to call a new manual game, selecting the board size as parameter:

```
start_new_game(size);
```

And an ASCCI board will be drawn and each player will be eable to select the position that wants to take. 
Player 1 will be marked in the board by a red cross, and Player 2 by a blue circle. 

![For example:](https://i.imgur.com/n6D660W.png)

### Testing
A fixed game can be loaded an instantly evaluated. 
The game must be loaded in a file named *preSavedHex.txt* .
The game must be call as follows:
```
load_pre_saved_game();
```
The file must have the following format:
1. 1st line: size of one dimention the board (11 for 11x11)
2. 2 to N line: '"index of row" "index of column" "value"'

"value" being 1 for Player1 and 2 for Player2. 

*preSavedHex.txt* for 3x3:
>3\
0 0 1\
0 1 2\
0 2 2\
1 0 1\
1 1 1\
1 2 1\
2 0 2\
2 1 2\
2 2 1\
3 0 0\
3 1 2\
3 2 2
>

Example for a 5x5 match won by Player2:

![example](https://i.imgur.com/vMDeVv9.png)




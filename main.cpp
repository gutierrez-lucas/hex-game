#include <iostream>
#include "hex.h"
#include <string>
#include <sstream>
#include <limits>

void load_pre_saved_game(){
    // a pre set game can be evaluated by charging its values in preSavedHex.txt 
    // the first line must be the size of the game. if 11x11 the first line must be 11
    // from there on a tuple must be specified in the follow format: "row column value" (without quotes)
    // value = 1 for PLAYER A and = 2 for PLAYER B
    hexClass example_hex("preSavedHex.txt");
    example_hex.print_who_won();
}

void start_new_game(uint8_t dimension){
    hexClass my_hex(dimension);
    int input_row, input_column;
    enum PLAYER player = PLAYER_A;

    std::cout << "NEW GAME"<<std::endl;
    std::cout << "STATUS: START"<<std::endl;
    my_hex.print_hexa();
    std::cout << "Player " << player << ": select row and column: " << std::endl;

    while(my_hex.get_who_won() == NONE){     

        std::cin >> input_row;
        std::cout << "\033[F";
        std::cin >> input_column;

        std::cout << "\033[F" << "\033[K";
        for(uint8_t i=0;i<dimension*2+1;i++) std::cout << "\033[1A";
        std::cout << "\033[F";
        switch(my_hex.set_position(player, input_row, input_column)){
            case(VALID):
                std::cout << "\033[F" << "Player" << player << " - movement(" << input_row << ",";
                std::cout << input_column <<") STATUS: " << "\033[32m" << "OK          " << "\033[0m" << std::endl;
                player == PLAYER_A ? player = PLAYER_B : player = PLAYER_A;
                break;
            case(INVALID):
                std::cout << "\033[F" << "Player" << player << " - movement(" << input_row << ",";
                std::cout << input_column <<") STATUS: " << "\033[31m" << "INVALID     " << "\033[0m" << std::endl;
                break;
            case(SWAPED):
                std::cout << "\033[F" << "Player" << player << " - movement(" << input_row << ",";
                std::cout << input_column <<") STATUS: " << "\033[32m" << "SWAPED     " << "\033[0m" << std::endl;
                player == PLAYER_A ? player = PLAYER_B : player = PLAYER_A;
                break;                
            case(REPEATED):
                std::cout << "\033[F" << "Player" << player << " - movement(" << input_row << ",";
                std::cout << input_column <<") STATUS: " << "\033[31m" << "ALREADY SET" << "\033[0m" << std::endl;
                break;
            case(WIN):
                std::cout << "\033[F" << "Player" << player << " - movement(" << input_row << ",";
                std::cout << input_column << "\033[32m" << ") WON THE GAME!!           " << "\033[0m" << std::endl;                
                my_hex.print_hexa();
                std::cout << "\033[K";
                my_hex.print_who_won();
                std::cout << "\033[K" << "\033[B" << "\033[K" << "\033[B" << "\033[K" << "\033[A" << "\033[A";
                return;
            default: break;
        }
        my_hex.print_hexa();
        std::cout << "\033[K";
        std::cout << "Player " << player << ": select row and column: " << std::endl;
        std::cout << "\033[K" << "\033[B" << "\033[K" << "\033[A";
    }
}

int main(){
    load_pre_saved_game();

    int size;
    while(true){
        for(;;){
            std::cout << "Select game size (ex.: 11 for 11x11) -> ";
            if (std::cin >> size) {
                break;
            } else {
                std::cout << "Please enter a valid integer" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        if(size > 11)std::cout << "Game size cannot be bigger than 11x11" << std::endl;
        else if(size == 0)std::cout << "Game size cannot be 0" << std::endl;
        else if(size < 3)std::cout << "Game size cannot be smaller than 3x3" << std::endl;
        else break;
    }
    start_new_game(size);
    return 0;
}
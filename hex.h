#include "graph.h"
// if something is not working in the find_track validation, uncomment the following line to get more info
// #define DEBUG_FIND_TRACK
enum VALID_STATUS{VALID, INVALID, REPEATED, EMPTY, WIN, SWAPED};
enum PLAYER{PLAYER_A=1, PLAYER_B, NONE};

class hexClass{
    public:

        hexClass(uint8_t new_dimension){
            board_size = new_dimension;
            graphClass cGraph(board_size, 0);
            create_checked_path();
            clear_checked_path();
            hex_graph = cGraph;
        }

        hexClass(const char* filename){
            graphClass cGraph(filename);
            board_size = cGraph.get_size();
            hex_graph = cGraph;
            std::cout << "Example match: " << (int)board_size << std::endl;

            create_checked_path();
            clear_checked_path();
            print_hexa();
            validate_game(PLAYER_A);
            validate_game(PLAYER_B);
        }

        void print_normal(){  // print matrix graph representation
            std::cout << hex_graph << std::endl;
        }

        void print_hexa(){   // print ASCII board representation
            std::cout << "  ";
            for(int i=0;i<hex_graph.get_size()-1;i++) std::cout << std::setfill(' ') << std::setw(2) << i << " -";
            std::cout << " " << hex_graph.get_size()-1 << std::endl;
            for(int row = 0; row < hex_graph.get_size(); row++){
                std::cout << std::setfill(' ') << std::setw(2) << row << " ";
                for(int column = 0; column < hex_graph.get_size(); column++){
                    if(column == 0){
                        int repeat = 0;
                        while(repeat++ != 2*(row)) std::cout << " ";
                    }
                    switch(hex_graph.get_weight(row, column)){
                        case(PLAYER_A):
                            std::cout << "\033[31m" << "x " << "\033[0m";
                            break;
                        case(PLAYER_B):
                            std::cout << "\033[34m" << "o " << "\033[0m";
                            break;
                        case(0):
                            std::cout << ". ";
                            break;
                        default: break;

                    }
                    if(column != hex_graph.get_size() - 1) std::cout << "- ";
                }
                std::cout << std::endl;
                std::cout << "\033[L";

                if(row < hex_graph.get_size()-1){
                    for(int column = 0; column <= (hex_graph.get_size() - 1) * 2; column++){
                        if(column == 0){
                            std::cout << "   ";
                            int repeat = 0;
                            while(repeat++ != 2*row+1) std::cout << " ";
                        }
                        if(column%2 != 0){
                            std::cout << "/ ";
                        }else{
                            std::cout << "\\ ";
                        }
                    }
                }
                std::cout << std::endl;
            }
        }

        VALID_STATUS set_position(enum PLAYER player, uint8_t row, uint8_t column){
            VALID_STATUS status = validate_movement(row, column);
            if(status == VALID){
                movements++;
                hex_graph.set_weight(row, column, player);
                return(validate_game(player));
            }else if(status == REPEATED && movements == 1){
                status = SWAPED;
                movements++;
                hex_graph.set_weight(row, column, player);
            }
            return(status);
        }

        PLAYER get_who_won(){ return(won_the_game);}

        void print_who_won(){
            if(won_the_game == PLAYER_A){
                std::cout << "PLAYER 1 WON THE GAME" << std::endl;
            }else if(won_the_game == PLAYER_B){
                std::cout << "PLAYER 2 WON THE GAME" << std::endl;
            }else{
                std::cout << "NO ONE WON THE GAME" << std::endl;
            }
            std::cout << std::endl << std::endl;
        }

    private:

        VALID_STATUS find_track(PLAYER player, uint8_t row, uint8_t column){           
            bool border_condition1, border_condition2, border_condition3, border_condition4; 
            // this conditions will prevent a segfault when the borders of the board are being evaluated
            column - 1  >= 0                         ?   border_condition1=false : border_condition1=true;
            row    + 1  <= hex_graph.get_size() - 1  ?   border_condition2=false : border_condition2=true;
            column + 1  <= hex_graph.get_size() - 1  ?   border_condition3=false : border_condition3=true;
            row    - 1  >= 0                         ?   border_condition4=false : border_condition4=true;

            if(hex_graph.get_weight(row, column)!=player){
#ifdef DEBUG_FIND_TRACK                
                std::cout << "base " << (int)row << "," << (int)column << "  !set" << std::endl;
#endif                
                return EMPTY;
            }else{
        // if, starting from a 0 row for A or 0 column for B, is posible to follow a track to the other end then that player won
            checked_path[row][column] = true;
                if((player == PLAYER_A && row == hex_graph.get_size()-1) || (player == PLAYER_B && column == hex_graph.get_size()-1)){
#ifdef DEBUG_FIND_TRACK                
                    std::cout << "base " << (int)row << "," << (int)column << "  set ==> WIN" << std::endl;
#endif                    
                    return WIN;
                }else{
#ifdef DEBUG_FIND_TRACK                
                    std::cout << "base " << (int)row << "," << (int)column << "  set";
#endif                    
                }
            }
        // if the current node is taken then evaluate all the next posible ones. 

        // 6 movements are posible in the non border cases. to limit those border ones each border condition has been
        // previously evaluated. if those conditions where left to evaluate in the movement conditional, the would provoke a segfault

        // POSIBLE MOVEMENTS
        // I -> down and left
            if(!border_condition1 && !border_condition2  && hex_graph.get_weight(row+1, column-1) == player){
                if(checked_path[row+1][column-1] == true){
#ifdef DEBUG_FIND_TRACK                
                    std::cout << "-> xA(" << (int)row+1 << "," << (int)column-1 << ")" << std::endl;
#endif                    
                }else{

#ifdef DEBUG_FIND_TRACK                
                std::cout << "-> A(" << (int)row+1 << "," << (int)column-1 << ")" << std::endl;
#endif                
                if(find_track(player, row+1, column-1)==WIN) return WIN;
                }
            }

        // II -> down and right
            if(!border_condition2 && hex_graph.get_weight(row+1, column) == player){
                if(checked_path[row+1][column] == true){
#ifdef DEBUG_FIND_TRACK                
              std::cout << "-> xB(" << (int)row+1 << "," << (int)column << ")" << std::endl;
#endif
                }else{

#ifdef DEBUG_FIND_TRACK                
                std::cout << "-> B(" << (int)row+1 << "," << (int)column << ")" << std::endl;
#endif                
                if(find_track(player, row+1, column)==WIN) return WIN;
                } 
            }

        // III -> right
            if(!border_condition3 && hex_graph.get_weight(row, column+1) == player){
                if(checked_path[row][column+1] == true){
#ifdef DEBUG_FIND_TRACK                
                    std::cout << "-> xC(" << (int)row << "," << (int)column+1 << ")" << std::endl;
#endif
                }else{
                
#ifdef DEBUG_FIND_TRACK                
                    std::cout << "-> C(" << (int)row << "," << (int)column+1 << ")" << std::endl;
#endif                
                    if(find_track(player, row, column+1)==WIN) return WIN;
                } 
                
            }

        // IV -> up and right
                if((!border_condition3 && !border_condition4)  && (hex_graph.get_weight(row-1, column+1) == player)){
                    if(checked_path[row-1][column+1] == true){
#ifdef DEBUG_FIND_TRACK                
                        std::cout << "-> xD(" << (int)row-1 << "," << (int)column+1 << ")" << std::endl;
#endif
                    }else{
#ifdef DEBUG_FIND_TRACK                
                        std::cout << "-> D(" << (int)row-1 << "," << (int)column+1 << ")" << std::endl;
#endif             
                        if(find_track(player, row-1, column+1)==WIN) return WIN;
                    } 
                }


        // V -> up and left
                if((!border_condition4 && !border_condition1)  && (hex_graph.get_weight(row-1, column-1) == player)){
                    if(checked_path[row-1][column-1] == true){
#ifdef DEBUG_FIND_TRACK                
                        std::cout << "-> xE(" << (int)row-1 << "," << (int)column-1 << ")" << std::endl;
#endif
                    }else{
#ifdef DEBUG_FIND_TRACK                
                        std::cout << "-> E(" << (int)row-1 << "," << (int)column-1 << ")" << std::endl;
#endif             
                        if(find_track(player, row-1, column-1)==WIN) return WIN;
                    } 
                }                

        // VI -> left
            if((!border_condition1)  && (hex_graph.get_weight(row, column-1) == player)){
                if(checked_path[row][column-1] == true){
#ifdef DEBUG_FIND_TRACK                
                    std::cout << "-> xF(" << (int)row << "," << (int)column-1 << ")" << std::endl;
#endif
                }else{
                    
#ifdef DEBUG_FIND_TRACK                
                std::cout << "-> F(" << (int)row << "," << (int)column-1 << ")" << std::endl;
#endif             
                if(find_track(player, row, column-1)==WIN) return WIN;
                } 
            }            

        // if no posible track can be followed for this node then return false
#ifdef DEBUG_FIND_TRACK                
                std::cout << "-> FAIL" << std::endl;            
#endif                

            return INVALID;
        }

        VALID_STATUS validate_game(PLAYER player){
        // in order to evaluate if a player won the match a track from the top to the bottom for player A
        // and from left to right for player B will be searched. any track that does not start from those references
        // will be dismissed. 
        // note that the evaluation for player B is the same than from player A if the board is rotated 90 degrees clockwise
        // that can be done swaping the row and the column in the evaluation
            for(uint8_t i=0; i<= hex_graph.get_size()-1; i++){
                if(player == PLAYER_A){
                    if(find_track(PLAYER_A, 0, i) == WIN){
                        won_the_game=PLAYER_A; 
#ifdef DEBUG_FIND_TRACK                
                        print_checked_path(); 
#endif
                        return WIN;
                        }
                }else if(player == PLAYER_B){
                    if(find_track(PLAYER_B, i, 0) == WIN){
                        won_the_game=PLAYER_B; 
#ifdef DEBUG_FIND_TRACK                
                        print_checked_path(); 
#endif
                        return WIN;}
                }else{
                    return INVALID;                    
                }
                clear_checked_path(); // after each iteration, clear the checked path matrix
#ifdef DEBUG_FIND_TRACK                
                print_checked_path();
                std::cout << std::endl;
#endif                
            }
            return VALID;
        }

        VALID_STATUS validate_movement(uint8_t row, uint8_t column){
            if(row > hex_graph.get_size()-1) return(INVALID);
            if(column > hex_graph.get_size()-1) return(INVALID);
            if(hex_graph.get_weight(row, column)!=0) return(REPEATED);
            return(VALID);
        }

        void create_checked_path(){
            checked_path = new bool*[board_size];
            for(int i=0; i<= board_size; i++) checked_path[i] = new bool[ board_size];
        }
        void clear_checked_path(){
            for(int i=0;i<=hex_graph.get_size()-1;i++){
                for(int j=0;j<=hex_graph.get_size()-1;j++){
                    checked_path[i][j] = false;
                } 
            }
        }
        void print_checked_path(){
            std::cout << "Current checked path: " << std::endl;
            for(int i=0;i<=hex_graph.get_size()-1;i++){
                for(int j=0;j<=hex_graph.get_size()-1;j++){
                    std::cout << checked_path[i][j] << " ";
                    checked_path[i][j] = false;
                } 
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        uint8_t board_size;
        graphClass hex_graph;
        PLAYER won_the_game = NONE;
        bool** checked_path;  // matrix for storing already processed nodes while validating a game
        int movements = 0;      
};
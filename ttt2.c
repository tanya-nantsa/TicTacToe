#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ttt.h"

void init_boards(){
    for (int i = 0; i<HSIZE; i++){ //goes through entire htable and sets init to 0
        htable[i].init = 0;
    }
}

int depth( Board board ){          //return total number of markers on the board
    int total = 0;
    for (int i = 0; i < BSIZE; i++){
        if (board[i]=='X' || board[i] == 'O'){
            total++;
        }
    }
    return total;
}

char winner( Board board ){
    int x_count = 0;
    int o_count = 0;
    for (int i=0; i<8; i++){
        for (int j = 0; j<3; j++){
            if (board[pos2idx[WINS[i][j]]] == 'X'){ //counts the X's in a winnning position
                x_count++;
            } else if (board[pos2idx[WINS[i][j]]] == 'O'){ //counts the O's in a winnning position
                o_count++;
            }
        }
        if (x_count == 3){            //if 3 X's in winning positions returns X
            return 'X';
        } else if (o_count == 3){    //if 3 O's in winning positions returns O
            return 'O';
        }
        x_count = 0;
        o_count = 0;
    }
    if (depth(board) == 9){         //returns '-' if game is over and no winner
        return '-';
    } else {
        return '?';                 //returns '?' if game is in progress
    }
}

char turn( Board board ){ //uses modular divisionn to determind whose turn it is
    if ((depth(board) % 2 == 1) && (depth(board) != 9)){
        return 'O';
    } else if ((depth(board) % 2 == 0) && (depth(board) != 9)){
        return 'X';
    } else {
        return '-';
    }
}

void init_board( Board board ){             //sets the variables of a structure at index
    int hash = board_hash(board);           //   determinded by board_hash
    htable[hash].init = 1;
    htable[hash].turn = turn(board);
    htable[hash].depth = depth(board);
    htable[hash].winner = winner(board);
    strcpy(htable[hash].board, board);
}

void join_graph( Board board ){             //function create and join all the nodes of all
    int hash = board_hash(board);           //   possible tic-tac-toe games
    char new_board[BSIZE];
    for (int i = 0; i<9; i++){              //loops through all posible positions
        if (board[pos2idx[i]] == 'X' || board[pos2idx[i]] == 'O') { //if there's a piece in that position
            htable[hash].move[i] = -1;                              // set to -1
        } else if (htable[hash].winner == 'O' || htable[hash].winner == 'X') {
            htable[hash].move[i] = -1;     //if node has a winnner then set all child nodes to -1
        } else {
            strcpy(new_board, board);      //copy board, place marker in new spot on new board
            new_board[pos2idx[i]] = turn(board);
            int new_hash = board_hash(new_board);//compute hash of new board and store in the move array
            htable[hash].move[i] = new_hash;
            if (htable[new_hash].init == 0){//check if the hash table already contains an entry
                init_board(new_board);      //then create new entry
                join_graph(new_board);
            }
        }
    }
}

int block_X_win( int move2, int board){ //in the case that the computer is always X
    char movec;
    char * blank_board = (char *)malloc(25);
    blank_board = "0|1|2\n-+-+-\n3|4|5\n-+-+-\n6|7|8\n";
    char * the_board = (char *)malloc(25);
    int position = 0;
    
    movec = move2 + '0';
    strcpy(the_board, htable[(htable[board].move[move2])].board);

    for (int x = 0; x < 30; x++) {
        if (movec == blank_board[x]) {
            position = x;
        }
    }
    the_board[position] = 'X';
    
    if (winner(the_board) == 'X'){
//      free(the_board);
//      free(blank_board);
        return (1);
    }

//    free(the_board);
//    free(blank_board);
    return (0); //does not block win
}

int block_O_win( int move2, int board){ //in the case that the computer is always X
    char movec;
    char * blank_board = (char *)malloc(25);
    blank_board = "0|1|2\n-+-+-\n3|4|5\n-+-+-\n6|7|8\n";
    char * the_board = (char *)malloc(25);
    int position = 0;
    
    movec = move2 + '0';
    strcpy(the_board, htable[(htable[board].move[move2])].board);

    for (int x = 0; x < 30; x++) {
        if (movec == blank_board[x]) {
            position = x;
        }
    }
    the_board[position] = 'O';
    
    if (winner(the_board) == 'O'){
//      free(the_board);
//      free(blank_board);
        return (1);
    }

//    free(the_board);
//    free(blank_board);
    return (0); //does not block win
}


int best_move_X(int board){
    int comp_score[9];
    
    for (int x = 0; x < 9; x++){//clear comp_score after iteration
        comp_score[x]=0;
    }
    
    for (int comp_move=0; comp_move<9; comp_move++){
        
        if (htable[board].move[comp_move]==-1){ //put that part into a variable
            comp_score[comp_move] = 0;
        } else {
            if (block_O_win(comp_move, board)==1){ //**print the score and fix problem
                //strcmp
                comp_score[comp_move]+= 50; //if move blocks other player from winning
            }
            if (comp_move == 0 || comp_move == 2 || comp_move == 6 || comp_move == 8) {
                comp_score[comp_move]+= 3; //add 3 pts if in corner
            }
            if (comp_move == 1 || comp_move == 3 || comp_move == 5 || comp_move == 7) {
                comp_score[comp_move]+= 2; //add 2 pts if at side
            }
            if (comp_move == 4){
                comp_score[comp_move]+= 4; //add 4 pts if in centre
            }
            if (htable[htable[board].move[comp_move]].winner == 'X'){
                comp_score[comp_move]+= 100; //if this is a winning move add 100 pts
            }
            
        }
    }
    int max = comp_score[0];
    int maxi = 0;
    for (int i = 0; i< 9; i++){
        if (max < comp_score[i]){
            max = comp_score[i];
            maxi = i;
        }
    }
    return maxi;
}

int best_move_O(int board){
    int comp_score[9];
    
    for (int x = 0; x < 9; x++){//clear comp_score after iteration
        comp_score[x]=0;
    }
    
    for (int comp_move=0; comp_move<9; comp_move++){
        
        if (htable[board].move[comp_move]==-1){
            comp_score[comp_move] = 0;
        } else {
            if (block_X_win(comp_move, board)==1){
                comp_score[comp_move]+= 50; //if move blocks other player from winning
            }
            if (comp_move == 0 || comp_move == 2 || comp_move == 6 || comp_move == 8) {
                comp_score[comp_move]+= 3; //add 3 pts if in corner
            }
            if (comp_move == 1 || comp_move == 3 || comp_move == 5 || comp_move == 7) {
                comp_score[comp_move]+= 2; //add 2 pts if at side
            }
            if (comp_move == 4){
                comp_score[comp_move]+= 4; //add 4 pts if in centre
            }
            if (htable[htable[board].move[comp_move]].winner == 'O'){
                comp_score[comp_move]+= 100; //if this is a winning move add 100 pts
            }
            
        }
    }
    int max = comp_score[0];
    int maxi = 0;
    for (int i = 0; i< 9; i++){
        if (max < comp_score[i]){
            max = comp_score[i];
            maxi = i;
        }
    }
    return maxi;
}

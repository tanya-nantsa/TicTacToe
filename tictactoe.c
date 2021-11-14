
/*
 * Fixed version of alphatoe.c
 * Credit: Samuel Guilbeault
 */

#include "ttt.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INPUT 512

int main(int argc, char **argv)
{
    char human;
    char input[MAX_INPUT];
    
    init_boards();
    init_board(START_BOARD);
    join_graph(START_BOARD);
    system("clear");
    //add the Welcome page
    do {
        printf("Welcome to the Game Tic-tac-toe!\n");
        printf("X or O? "); //for now test being O
        fgets(input, MAX_INPUT, stdin);
    
        if (input[0] == 'X' || input[0] == 'O') {
            human = input[0];
        } else if (input[0] == 'x' || input[0] == 'o'){
            human = input[0] - 32;
        } else {
            system("clear");
            printf("Error: Must enter either X or O\n");
        }
    } while (input[0] != 'X' && input[0] != 'O' && input[0] != 'x' && input[0] != 'o');
    
    int board = 0;
    
    while (htable[board].winner == '?') // while no winner
    {
        system("clear");
        //printf("%d\n", board);
        if (human == htable[board].turn)
        {
            int next_board;
            char movec;
            do
            {
                
                int movei;
                print_node(htable[board]);
                printf("Your move [0-8]: ");
                fgets(input, MAX_INPUT, stdin);
                movec = input[0];
                movei = (int)(movec - '0');
                next_board = htable[board].move[movei];
                if (next_board == -1 || movec < '0' || movec > '8' || input[1] != '\n')
                {
                    system("clear");
                    printf("Illegal move, try again!\n");
                }
            } while (next_board == -1 || movec < '0' || movec > '8' || input[1] != '\n');
            board = next_board;
        }
        else
        {
            if (human == 'O'){
                print_node(htable[board]);
                printf("Computer is playing\n");
                sleep(2);
                system("clear");
                board = htable[board].move[best_move_X(board)];
                print_node(htable[board]);
            }
            else if (human == 'X') {
                print_node(htable[board]);
                printf("Computer is playing\n");
                sleep(2);
                system("clear");
                board = htable[board].move[best_move_O(board)];
                print_node(htable[board]);
            }
        } // computers turn
    }   // while loop
    
    system("clear");
    print_node(htable[board]);
    printf("Game over, Player 1!\n");
    
    if (htable[board].winner == human){
        printf("You won!\n");
        return (1);
    }
    else if (htable[board].winner == '-'){
        printf("The game was a tie!\n");
        return (0);
    }
    else{
        printf("You lost!\n");
        return (-1);
    }
}

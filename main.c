#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printers.h"
#include "piece_functions.h"
#include "helpers.h"


/*
Compile:
gcc main.c helpers.c printers.c piece_functions.c -o board2

Make Library:
gcc -c -fPIC main.c -o main.o
gcc -c -fPIC helpers.c -o helpers.o
gcc -c -fPIC printers.c -o printers.o
gcc -c -fPIC piece_functions.c -o piece_functions.o
gcc -shared -o libboard.so main.o helpers.o printers.o piece_functions.o
*/



// uppercase char pieces are white
// lowercase char pieces are black
// NOTE: N or n is knight

int main(void)
{
    char (*board)[DIM] = calloc(HEIGHT * DIM, sizeof(char));
    if (board == NULL)
    {
        printf("Not enough memory for chess board.\n");
        return 1;
    }
    populate(board);
    print_board(board);

    pos_node *valids_head;
    while (1)
    {
        char *input = malloc(sizeof(char) * 60);
        char piece;
        int from_rank, from_file;
        int cur_turn = board[12][0];
        while (1)
        {
            if (in_check(board, cur_turn))
            {
                if (checkmate(board))
                {
                    print_winner(cur_turn);
                    free(board);
                    return 0;
                }
                print_check(cur_turn);
            }
            printf("\n\nType QUIT to quit the game.\n");
            printf("piece: ");
            fgets(input, sizeof(char) * 60, stdin);
            input[strlen(input) - 1] = '\0';
            int *coords = input_formatter(input);
            int quit = quitter(input);
            if (quit)
            {
                free(input);
                free(board);
                return 0;
            }
            if (coords != NULL)
            {
                from_file = coords[0];
                from_rank = coords[1];
                piece = board[from_rank][from_file];
                free(coords);
                break;
            }
            free(coords);
        }

        char selecting_white = 1;
        if (piece >= 'a' && piece <= 'z')
        {
            selecting_white = 0;
        }

        if (selecting_white == cur_turn)
        {
            valids_head = get_valids(board, from_rank, from_file);
            printf("\n****************\n");
            printf("(%c): %c%i\n", board[from_rank][from_file], from_file + 'a', DIM - from_rank);
            printf("****************\n");
            print_valids(valids_head);
            printf("****************\n\n");

            int invalid = 1;
            if (valids_length(valids_head) != 0)
            {
                int quit;
                while (invalid)
                {
                    int to_rank, to_file;
                    printf("\nType QUIT to choose another piece.\n");
                    printf("to: ");
                    fgets(input, sizeof(char) * 60, stdin);
                    input[strlen(input) - 1] = '\0';
                    int *coords = input_formatter(input);
                    quit = quitter(input);
                    if (quit)
                    {
                        invalid = 0;
                    }
                    else if (coords != NULL)
                    {
                        to_file = coords[0];
                        to_rank = coords[1];
                        invalid = move(board, valids_head, from_rank, from_file, to_rank, to_file);
                    }
                    free(coords);
                }
                if (!quit)
                {
                    if (board[12][0])
                    {
                        board[12][0] = 0;
                    }
                    else
                    {
                        board[12][0] = 1;
                    }
                }
            }
            free_valids(valids_head);
            print_board(board);
        }
        else
        {
            printf("Select the current color: ");
            if (cur_turn)
            {
                printf("White\n");
            }
            else
            {
                printf("Black\n");
            }
        }
        free(input);
    }
    free(board);
    return 0;
}

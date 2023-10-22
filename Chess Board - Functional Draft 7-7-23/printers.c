#include <stdio.h>

#include "printers.h"
#include "helpers.h"
#include "piece_functions.h"


void print_castle_not_moved(char board[][DIM])
{
    printf("\n");
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i ", board[14][i * 3 + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_board(char board[][DIM])
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("*****************\n");
    if (board[12][0])
    {
        printf("      White      \n");
    }
    else
    {
        printf("      Black      \n");
    }
    printf("*****************\n");
    printf("  ");
    printf("\n");
    for (int i = DIM; i >= 0; i--)
    {
        if (i != 0)
        {
            printf("%i ", i);
        }
        else
        {
            printf("  ");
        }
        for (int j = 0; j < DIM; j++)
        {
            if (i != 0)
            {
                printf("%c ", board[8 - i][j]);
            }
            else
            {
                printf("%c ", j + 'a');
            }
        }
        printf("\n");
    }
    
    int blk_cas_idx = get_cas_idx(board, 0) - 1;
    printf("\nBlack Casualties: ");
    for (int i = 0; i <= blk_cas_idx; i++)
    {
        printf("%c", board[8 + blk_cas_idx / 8][blk_cas_idx % 8]);
    }
    int wht_cas_idx = get_cas_idx(board, 1) - 1;
    printf("\nWhite Casualties: ");
    for (int i = 0; i <= wht_cas_idx; i++)
    {
        printf("%c", board[10 + wht_cas_idx / 8][wht_cas_idx % 8]);
    }
    printf("\n");
}

void print_valids(pos_node *valids_head)
{
    pos_node *cur_node = valids_head;
    int pos_num = 0;
    while(cur_node->next != NULL)
    {
        if (pos_num != 0)
        {
            printf("(%i): %c%i\n", pos_num, cur_node->file + 'a', DIM - cur_node->rank);
        }
        cur_node = cur_node->next;
        pos_num ++;
    }
    printf("(%i): %c%i\n", pos_num, cur_node->file + 'a', DIM - cur_node->rank);
}

void print_check(int white)
{
    if (white)
    {
        printf("WHITE IS IN CHECK\n");
    }
    else
    {
        printf("BLACK IS IN CHECK\n");
    }
}

void print_winner(int white_lost)
{
    if (white_lost)
    {
        printf("BLACK WINS!\n");
        return;
    }
    printf("WHITE WINS!\n");
}
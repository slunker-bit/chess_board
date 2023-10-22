#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "piece_functions.h"
#include "helpers.h"
#include "printers.h"


// direction is integer for file change ... so 1 for right and -1 for left
int can_en_passant(char board[][DIM], int direction, int orig_rank, int orig_file)
{
    int cur_pawn = board[orig_rank][orig_file];
    int potential_pawn = board[orig_rank][orig_file + direction];

    // returning position eligibility
    return (toupper(potential_pawn) == 'P' 
        && is_opp(cur_pawn, potential_pawn) 
        && board[12][1] 
        && board[12][2] == orig_rank 
        && board[12][3] == orig_file  + direction
        && board[12][4] == 2);
}

void pawn_promotion(char board[][DIM], int rank, int file)
{
    int cur_pawn = board[rank][file];
    int white = is_white(cur_pawn);
    int last_rank = 0;
    char prom_options[] = {'Q', 'R', 'B', 'K'};
    if (!white)
    {
        last_rank = 7;
        prom_options[0] = 'q';
        prom_options[1] = 'r';
        prom_options[2] = 'b';
        prom_options[3] = 'k';
    }
    if (rank != last_rank)
    {
        return;
    }

    char choice;
    int invalid = 1;
    while (invalid)
    {
        printf("\nPromote pawn to one of the following: %c, %c, %c, %c\n", prom_options[0], prom_options[1], prom_options[2], prom_options[3]);
        scanf("%c", &choice);
        for (int i = 0; i < 4; i++)
        {
            if (choice == prom_options[i])
            {
                invalid = 0;
            }
        }
    }
    board[rank][file] = choice;
}

pos_node *pawn(char board[][DIM], int orig_rank, int orig_file)
{
    pos_node *valids_head = make_head();

    char cur_pawn = board[orig_rank][orig_file];

    int homerank = 1;
    int rank_change = 1;
    if (cur_pawn == 'P')
    {
        homerank = 6;
        rank_change = -1;
    }
    if (board[orig_rank + rank_change][orig_file] == '-')
    {
        int can_attempt_two = !add_node(board, valids_head, cur_pawn, orig_rank + rank_change, orig_file);
        if (orig_rank == homerank && can_attempt_two && board[orig_rank + rank_change * 2][orig_file] == '-')
        {
            add_node(board, valids_head, cur_pawn, orig_rank + rank_change * 2, orig_file);
        }
    }

    if (is_opp(cur_pawn, board[orig_rank + rank_change][orig_file - 1]))
    {
        add_node(board, valids_head, cur_pawn, orig_rank + rank_change, orig_file - 1);
    }
    if (is_opp(cur_pawn, board[orig_rank + rank_change][orig_file + 1]))
    {
        add_node(board, valids_head, cur_pawn, orig_rank + rank_change, orig_file + 1);
    }

    // en passant
    if (can_en_passant(board, -1, orig_rank, orig_file))     // left position
    {
        add_node(board, valids_head, cur_pawn, orig_rank + rank_change, orig_file - 1);
    }
    else if (can_en_passant(board, 1, orig_rank, orig_file)) // right position
    {
        add_node(board, valids_head, cur_pawn, orig_rank + rank_change, orig_file + 1);
    }

    int is_check = in_check(board, is_white(cur_pawn));
    if (!is_check)
    {
        return valids_head;
    }
    return stop_check(board, valids_head, orig_rank, orig_file);
}

pos_node *knight(char board[][DIM], int orig_rank, int orig_file)
{
    pos_node *valids_head = make_head();

    // 2d array of position changes for knight
    // idx 0 array is rank changes
    // idx 1 array is file changes
    int changes[][8] = {
        {-1, -2, -2, -1, 1, 2, 2, 1},
        {-2, -1, 1, 2, 2, 1, -1, -2}
    };

    const int SIZE_ARR_CHANGES = 8;
    for (int i = 0; i < SIZE_ARR_CHANGES; i++)
    {
        int rank = orig_rank + changes[0][i];
        int file = orig_file + changes[1][i];
        add_node(board, valids_head, board[orig_rank][orig_file], rank, file);
    }
    
    int is_check = in_check(board, is_white(board[orig_rank][orig_file]));
    if (!is_check)
    {
        return valids_head;
    }
    return stop_check(board, valids_head, orig_rank, orig_file);
}

void update_castle_not_moved(char board[][DIM], int rank, int file)
{
    char piece = board[rank][file];
    int color = is_white(piece);
    if (toupper(piece) == 'K')
    {
        board[14][color * 3 + 1] = 0;
    }
    else if(toupper(piece) == 'R')
    {
        if (file == 0)
        {
            board[14][color * 3] = 0;
        }
        else if (file == 7)
        {
            board[14][color * 3 + 2] = 0;
        }
    }
}

static int can_castle(char board[][DIM], char copy[][DIM], char king_piece, int change, int orig_rank, int orig_file)
{
    int last_file = orig_file;
    while (last_file != -1 && last_file != 8)
    {
        last_file += change;
    }
    last_file -= change;

    int file = orig_file + change;
    while (file != last_file)
    {
        char cur_piece = board[orig_rank][file];
        if (cur_piece != '-')
        {
            return 0;
        }
        file += change;
    }
    
    int first_check = 0;
    pos_node *temp_head = make_head();

    add_node(board, temp_head, king_piece, orig_rank, orig_file + change);
    if (in_bounds(orig_rank, orig_file + change * 2))
    {
        move(copy, temp_head, orig_rank, orig_file, orig_rank, orig_file + change);
        if (check(copy, king_piece))
        {
            first_check++;
        }
    }
    copy_board(board, copy);
    free(temp_head);

    add_node(board, temp_head, king_piece, orig_rank, orig_file + change * 2);
    if (in_bounds(orig_rank, orig_file + change * 2))
    {
        move(copy, temp_head, orig_rank, orig_file, orig_rank, orig_file + change * 2);
        if (check(copy, king_piece) && first_check)
        {
            free(temp_head);
            return 0;
        }
    }
    copy_board(board, copy);
    free(temp_head);
    return 1;
}

int in_check(char board[][DIM], int color)
{
    int white = board[12][0];
    if (color != white)
    {
        return 0;
    }
    int rank = board[13][white * 2];
    int file = board[13][white * 2 + 1];
    return check(board, board[rank][file]);
}

int check(char board[][DIM], char call_piece)
{
    int white = board[12][0];
    if (is_white(call_piece) != white)
    {
        return 0;
    }
    int king_rank = board[13][white * 2];
    int king_file = board[13][white * 2 + 1];
    
    for (int rank = 0; rank < DIM; rank++)
    {
        for (int file = 0; file < DIM; file++)
        {
            char piece = board[rank][file];
            if (piece != '-' && is_white(piece) != white)
            {
                pos_node *valids_head = get_valids(board, rank, file);
                if (seek_pos(board, valids_head, king_rank, king_file))
                {
                    free_valids(valids_head);
                    return 1;
                }
                free_valids(valids_head);
            }
        }
    }
    return 0;
}

pos_node *king(char board[][DIM], int orig_rank, int orig_file)
{
    // head of linked list for all valid position moves
    char piece = board[orig_rank][orig_file];

    pos_node *valids_head = make_head();

    char (*copy)[DIM] = malloc(DIM * HEIGHT * sizeof(char));
    if (copy == NULL)
    {
        printf("Not enough memory to copy board.\n");
        return NULL;
    }
    copy_board(board, copy);


    for (int rank = orig_rank - 1; rank < orig_rank + 2; rank++)
    {
        for (int file = orig_file - 1; file < orig_file + 2; file++)
        {
            if (in_bounds(rank, file))
            {
                pos_node* temp_head = make_head();
                add_node(copy, temp_head, piece, rank, file);
                move(copy, temp_head, orig_rank, orig_file, rank, file);
                if (!(rank == orig_rank && file == orig_file) && !check(copy, piece))
                {
                    add_node(board, valids_head, piece, rank, file);
                }
                copy_board(board, copy);
                free_valids(temp_head);
            }
            else
            {
                break;
            }
        }
    }
    int king_color = is_white(piece);
    if (board[14][king_color * 3 + 1] && king_color == board[12][0])
    {
        if (board[14][king_color * 3])
        {
            board[14][king_color * 3] = 0;
            board[14][king_color * 3 + 1] = 0;
            if(can_castle(board, copy, piece, -1, orig_rank, orig_file))
            {
                add_node(board, valids_head, piece, orig_rank, orig_file - 2);
            }
        }
        if (board[14][king_color * 3 + 2])
        {
            board[14][king_color * 3 + 1] = 0;
            board[14][king_color * 3 + 2] = 0;
            if(can_castle(board, copy, piece, 1, orig_rank, orig_file))
            {
                add_node(board, valids_head, piece, orig_rank, orig_file + 2);
            }
        }
    }
    free(copy);
    return valids_head;
}

pos_node *shared_planes(char board[][DIM], int orig_rank, int orig_file, char piece_code)
{
    // head of linked list for all valid position moves
    pos_node *valids_head = make_head();

    int change[] = {-1, 1};
    const int SIZE_CHANGE = 2;
    int rank;
    int file;

    for (int i = 0; i < SIZE_CHANGE; i++)
    {
        for (int j = 0; j < SIZE_CHANGE; j++)
        {
            if (piece_code == 'R' || piece_code == 'Q')
            {
                rank = orig_rank;
                file = orig_file;
                while (1)
                {
                    if (i == 0)
                    {
                        rank += change[j];
                    }
                    else
                    {
                        file += change[j];
                    }
                    int bool_break = add_node(board, valids_head, board[orig_rank][orig_file], rank, file);
                    if (bool_break)
                    {
                        break;
                    }
                }
            }
            if (piece_code == 'B' || piece_code == 'Q')
            {
                rank = orig_rank;
                file = orig_file;
                while (1)
                {
                    rank += change[i];
                    file += change[j];
                    int bool_break = add_node(board, valids_head, board[orig_rank][orig_file], rank, file);
                    if (bool_break)
                    {
                        break;
                    }
                }
            }

        }
    }
    
    int is_check = in_check(board, is_white(board[orig_rank][orig_file]));
    if (!is_check)
    {
        return valids_head;
    }
    return stop_check(board, valids_head, orig_rank, orig_file);
}

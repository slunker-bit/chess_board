#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "piece_functions.h"
#include "printers.h"


static int get_start_rank(int white)
{
    int startrank = 8; // default casualty start rank for black dest casualties is 8
    if (white) // if dest is white, default casualty start rank on board changed to 10
    {
        startrank = 10;
    }
    return startrank;
}

static int update_casualties(char board[][DIM], char dest)
{
    int white = is_white(dest);
    int startrank = get_start_rank(white);
    int cas_idx = get_cas_idx(board, white);
    board[startrank + cas_idx / 8][cas_idx % 8] = dest;
}

pos_node *make_head(void)
{
    pos_node *head = malloc(sizeof(pos_node));
    if (head == NULL)
    {
        printf("Not enough memory to make head.\n");
        return NULL;
    }
    head->rank = -1;
    head->file = -1;
    head->next = NULL;
    return head;
}

void copy_board(char board[][DIM], char copy[][DIM])
{
    for (int rank = 0; rank < HEIGHT; rank++)
    {
        for (int file = 0; file < DIM; file++)
        {
            copy[rank][file] = board[rank][file];
        }
    }
}

int quitter(char *str)
{
    return strcmp(str, "QUIT") == 0;
}

int *input_formatter(char *input)
{
    if (strlen(input) != 2 || !(tolower(input[0]) >= 'a' && tolower(input[0]) <= 'h') || !(input[1] >= '1' && input[1] <= '8'))
    {
        return NULL;
    }
    int *coords = malloc(sizeof(int) * 2);
    if (coords == NULL)
    {
        printf("Not enough space to format input.\n");
        return NULL;
    }
    coords[0] = (int) (tolower(input[0]) - 'a');
    coords[1] = 7 - (int) (input[1] - '1');
    return coords;
}

pos_node *stop_check(char board[][DIM], pos_node *head, int rank, int file)
{
    pos_node *new_head = make_head();

    char (*copy)[DIM] = malloc(DIM * HEIGHT * sizeof(char));
    if (copy == NULL)
    {
        printf("Not enough memory to copy board.\n");
        return NULL;
    }
    copy_board(board, copy);

    char piece = board[rank][file];
    pos_node *temp = head;
    int white = is_white(piece);
    while (temp->next != NULL)
    {
        temp = temp->next;
        int to_rank = temp->rank;
        int to_file = temp->file;
        move(copy, head, rank, file, to_rank, to_file);
        if (!in_check(copy, white))
        {
            add_node(board, new_head, piece, to_rank, to_file);
        }
        copy_board(board, copy);
    }
    free(copy);
    free_valids(head);
    return new_head;
}

void populate_footer(char board[][DIM])
{

    // initialize default footer game info on rows 12, 13, and 14
    board[12][0] = 1; // game starts with white's turn

    board[13][0] = 0; // black king rank (1)
    board[13][1] = 4; // black king file (e)
    board[13][2] = 7; // white king rank (8)
    board[13][3] = 4; // white king file (e)

    // initialize all kings and rooks to viable for castle (not moved and not casualty)
    board[14][0] = 1;
    board[14][1] = 1;
    board[14][2] = 1;
    board[14][3] = 1;
    board[14][4] = 1;
    board[14][5] = 1;

}

void populate(char board[][DIM])
{
    char main_ranks[] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};

    
    for (int i = 0; i < HEIGHT; i++)
    {
        if (i == 0 || i == 7)
        {
            memcpy(board[i], main_ranks, sizeof(main_ranks));
        }
        else
        {
            char next = '-';
            if (i == 1 || i == 6)
            {
                next = 'p';
            }
            for (int j = 0; j < DIM; j++)
            {
                board[i][j] = next;
            }
        }
        if (i == 6 || i == 7)
        {
            for (int j = 0; j < DIM; j++)
            {
                board[i][j] -= 32;
            }
        }
    }
    populate_footer(board);
}

int is_white(char piece)
{
    if (piece >= 'a' && piece <= 'z')
    {
        return 0;
    }
    if (piece >= 'A' && piece <= 'Z')
    {
        return 1;
    }
    printf("ERROR: (%c) is not a valid piece.\n", piece);
    return -1;
}

int valids_length(pos_node *head)
{
    int count = 0;
    pos_node *temp = head;
    while (temp->next != NULL)
    {
        temp = temp-> next;
        count++;
    }
    return count;
}

int in_bounds(int rank, int file)
{
    return rank >= 0 && rank <= 7 && file >= 0 && file <= 7;
}

int is_opp(char piece, char other)
{
    if (other == '-')
    {
        return 0;
    }
    int both_wht = (piece >= 'A' && piece <= 'Z' && other >= 'A' && other <= 'Z');
    int both_blk = (piece >= 'a' && piece <= 'z' && other >= 'a' && other <= 'z');
    return !(both_wht || both_blk);
}

int add_node(char board[][DIM], pos_node *head, char piece, int rank, int file)
{
    if (!in_bounds(rank, file))
    {
        return 1;
    }
    int bool_is_opp = is_opp(piece, board[rank][file]);
    if ((board[rank][file] == '-' || bool_is_opp))
    {
        pos_node *valid = malloc(sizeof(pos_node));
        if (valid == NULL)
        {
            printf("Not enough memory for next node.\n");
            return 2;
        }
        valid->rank = rank;
        valid->file = file;
        valid->next = head->next;
        head->next = valid;
        if (!bool_is_opp)
        {
            return 0; // don't break loop
        }
    }
    return 1; // should break loop
}

int seek_pos(char board[][DIM], pos_node *head, int rank, int file)
{
    if (head == NULL)
    {
        return 0;
    }
    pos_node *temp = head;
    temp->rank = head->rank;
    temp->file = head->file;
    temp->next = head->next;
    
    while (temp->next != NULL)
    {
        temp = temp->next;
        if (temp->rank == rank && temp->file == file)
        {
            return 1;
        }
    }
    return 0;
}

int get_cas_idx(char board[][DIM], int white)
{
    int startrank = get_start_rank(white);
    int cas_idx = 0;
    while (board[startrank + cas_idx / 8][cas_idx % 8] != '-')
    {
        cas_idx++;
    }
    return cas_idx;
}


pos_node *get_valids(char board[][DIM], int rank, int file)
{
    char piece = board[rank][file];
    char type = toupper(board[rank][file]);
    pos_node *valids_head = NULL;
    switch (type)
    {
        case 'P':
            valids_head = pawn(board, rank, file);
            break;
        case 'N':
            valids_head = knight(board, rank, file);
            break;
        case 'K':
            valids_head = king(board, rank, file);
            break;
        default:
            valids_head = shared_planes(board, rank, file, type);
    }
    return valids_head;
}

// returns 0 if was valid move and successfully completed
// returns 1 if was invalid move and was not able to complete
int move(char board[][DIM], pos_node *head, int from_rank, int from_file, int to_rank, int to_file)
{
    //printf("Moving (%c) from (%i, %i) to (%i, %i)\n", board[from_rank][from_file], from_rank, from_file, to_rank, to_file);
    if (!in_bounds(to_rank, to_file) || !seek_pos(board, head, to_rank, to_file))
    {
        return 1;
    }


    char piece = board[from_rank][from_file];
    char dest = board[to_rank][to_file];
    // if not a space, then this is an opponent piece
    if (dest != '-')
    {
        update_casualties(board, dest);
    }
    board[from_rank][from_file] = '-';
    board[to_rank][to_file] = piece;
    
    char upper_piece = toupper(piece);
    if (upper_piece == 'K')
    {
        board[13][board[12][0] * 2] = to_rank;
        board[13][board[12][0] * 2 + 1] = to_file;
    }
    if (upper_piece == 'K' || upper_piece == 'R')
    {
        update_castle_not_moved(board, to_rank, to_file);
    }
    if (upper_piece == 'K' && abs(from_file - to_file) == 2)
    {
        if (from_file - to_file == -2)
        {
            board[from_rank][7] = '-';
            board[from_rank][5] = 'r' - board[12][0] * 32;
        }
        else
        {
            board[from_rank][0] = '-';
            board[from_rank][2] = 'r' - board[12][0] * 32;
        }
    }
    if (upper_piece == 'P')
    {
        board[12][1] = is_white(piece);
        board[12][2] = (char) to_rank;
        board[12][3] = (char) to_file;
        board[12][4] = abs(from_rank - to_rank);

        if (abs(from_rank - to_rank) == 1 && abs(from_file - to_file) == 1 && dest == '-')
        {
            int pawn_change = 1;
            if (piece == 'P')
            {
                pawn_change = -1;
            }
            update_casualties(board, board[to_rank - pawn_change][to_file]);
            board[to_rank - pawn_change][to_file] = '-';
        }
        pawn_promotion(board, to_rank, to_file);
    }
    return 0;
}

void free_valids(pos_node *valids_head)
{
    pos_node *cur_node = valids_head;
    pos_node *last_node;
    while (cur_node->next != NULL)
    {
        //printf("freeing\n");
        last_node = cur_node;
        cur_node = cur_node->next;
        free(last_node);
    }
    free(cur_node);
}


int checkmate(char board[][DIM])
{
    int color = board[12][0];
    for (int rank = 0; rank < DIM; rank++)
    {
        for (int file = 0; file < DIM; file++)
        {
            char piece = board[rank][file];
            if (piece != '-' && is_white(piece) == color)
            {
                pos_node *piece_head = get_valids(board, rank, file);
                if (valids_length(piece_head) != 0)
                {
                    print_valids(piece_head);
                    return 0;
                }
                free_valids(piece_head);
            }
        }
    }
    return 1;
}
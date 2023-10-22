#ifndef HELPERS_H
#define HELPERS_H

#include "constants.h"

pos_node *make_head(void);
void copy_board(char board[][DIM], char copy[][DIM]);
void populate_footer(char board[][DIM]);
int quitter(char *str);
int *input_formatter(char *input);
pos_node *stop_check(char board[][DIM], pos_node *head, int rank, int file);
int is_white(char piece);
int is_opp(char piece, char other);
int add_node(char board[][DIM], pos_node *head, char piece, int rank, int file);
pos_node *get_valids(char board[][DIM], int rank, int file);
void populate(char board[][DIM]);
int move(char board[][DIM], pos_node *head, int from_rank, int from_file, int to_rank, int to_file);
int valids_length(pos_node *head);
int in_bounds(int rank, int file);
void free_valids(pos_node *valids_head);
int checkmate(char board[][DIM]);
int get_cas_idx(char board[][DIM], int white);
int seek_pos(char board[][DIM], pos_node *head, int rank, int file);


#endif
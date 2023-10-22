#ifndef PIECE_FUNCTIONS_H
#define PIECE_FUNCTIONS_H

#include "constants.h"

void pawn_promotion(char board[][DIM], int rank, int file);
pos_node *pawn(char board[][DIM], int orig_rank, int orig_file);
pos_node *knight(char board[][DIM], int orig_rank, int orig_file);
void update_castle_not_moved(char board[][DIM], int rank, int file);
int in_check(char board[][DIM], int color);
int check(char board[][DIM], char call_piece);
pos_node *king(char board[][DIM], int orig_rank, int orig_file);
pos_node *shared_planes(char board[][DIM], int orig_rank, int orig_file, char piece_code);

#endif
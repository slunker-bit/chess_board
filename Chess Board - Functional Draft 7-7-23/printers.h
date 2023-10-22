#ifndef PRINTERS_H
#define PRINTERS_H

#include "constants.h"

void print_board(char board[][DIM]);
void print_valids(pos_node *valids_head);
void print_castle_not_moved(char board[][DIM]); 
void print_check(int white);
void print_winner(int white_lost);

#endif 
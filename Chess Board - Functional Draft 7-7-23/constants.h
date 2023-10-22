#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef struct pos_node
{
    int rank;
    int file;
    struct pos_node *next;

} pos_node;

// first 8 rows are the board
// next 2 rows are black casualties
// next 2 rows are white casualites
// last 3 rows are miscellaneous game info:
/*
--row idx 12 is for current turn color and pawn info for en passant:
idx (0) is int value for whether or not the current turn is white's turn (0 or 1)

[12][1 through 4] are corresponding values to the last pawn's move for en passant:
idx (1) is whether or not pawn is white (0 or 1)
idx (2) rank that pawn moved to
idx (3) file that pawn moves to
idx (4) is rank distance that pawn traveled (to show if traveled 2 ranks)

--row idx 13 is for either king's current position:
idx (0) black king's current rank
idx (1) black king's current file
idx (2) white king's current rank
idx (3) white king's current file

--row idx 14 is for castle info on whether or not king or rooks are available for castle:
(row referred to as "castle_not_moved" in update function)
idx (0) whether or not left black rook has NOT moved (1 if hasn's moved, 0 if has moved or is casualty)
idx (1) whether or not black king has NOT moved
idx (2) whether or not right black rook as NOT moved
idx (3) whether or not left white rook has NOT moved
idx (4) whether or not white king has NOT moved
idx (5) whether or not right whtie rook as NOT moved
*/
#define HEIGHT 15
#define DIM 8 // both width and height proper of the board

#endif
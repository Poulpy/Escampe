#ifndef MODEL_H
#define MODEL_H

#include "graphics.h"

typedef enum Border
{
    TOP, BOTTOM, LEFT, RIGHT
} Border;

typedef enum Type
{
    EMPTY, UNICORN, PALADIN
} Type;

typedef enum Coul
{
    BLACK, WHITE
} Coul;

typedef struct NumBox
{
    int x;
    int y;
} NumBox;

typedef struct Box
{
    int edging;/* 1, 2, 3 */
    Type type;
    Coul color;
} Box;

Box gameboard[6][6];

int  is_edging_valid(int lastEdging, NumBox start);
int  is_any_pawn_playable(Coul color, int* lastEdging);
int  out_of_range(NumBox pos);
int  can_override(NumBox start, NumBox end);
int  is_cell_occupied(NumBox pos);
int  is_unicorn_alive(Type start, Type end);
int  can_move(NumBox pawnCell);
int  in_range(NumBox pos);
int  get_edging(NumBox n);
int  eql(NumBox n1, NumBox n2);
int  contains(NumBox *ns, int len, NumBox n);
void init_gameboard();
void init_gamepawns_1();
void init_gamepawns_2();
void move_pawn(NumBox start, NumBox end);
void random_move(Coul color, NumBox *start, NumBox *end);
void uniq(NumBox *ns, int *len);
void print_numboxes(NumBox *n, int len);
void print_numbox(NumBox n);
void remove_numbox(NumBox *ns, int *len, NumBox n);
void append(NumBox *ns, int *len, NumBox n);
void remove_numboxes(NumBox *n1, int *len1, NumBox *n2, int len2);
void copy(NumBox *n1, int *offset, NumBox *n2, int len2);
void place_pawns(NumBox pawns[6], Coul color);
void get_neighbours(NumBox *cells, int *offset, NumBox pawn, int moves, NumBox forbidden, NumBox player);
NumBox *get_moves(int *moves_count, NumBox pawn);
NumBox *get_cells_by_color(Coul color);
Border opposite_border(Border bor);

#endif

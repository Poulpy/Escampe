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

typedef enum Gamemode
{
    PVP, PVC /* Player VS Player, Player VS Computer */
} Gamemode;

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

#define AI_COLOR WHITE

Box gameboard[6][6];

int  is_edging_valid(int lastEdging, NumBox start);
int  can_override(NumBox start, NumBox end);
int  is_cell_occupied(NumBox pos);
int  can_move(NumBox pawnCell);
int  in_range(NumBox pos);
int  get_edging(NumBox n);
int  eql(NumBox n1, NumBox n2);
int  contains(NumBox *ns, int len, NumBox n);
int  can_other_player_move(Coul player, int lastEdging);
int  is_AI_turn(Coul currentPlayer, Gamemode mode);
int  move_pawn(NumBox start, NumBox end);
void init_gameboard();
void init_gamepawns_1();
void init_gamepawns_2();
void random_move(Coul color, NumBox *start, NumBox *end);
void get_neighbours(NumBox neigh[4], NumBox cell);
void uniq(NumBox *ns, int *len);
void print_numboxes(NumBox *n, int len);
void print_numbox(NumBox n);
void remove_numbox(NumBox *ns, int *len, NumBox n);
void append(NumBox *ns, int *len, NumBox n);
void remove_numboxes(NumBox *n1, int *len1, NumBox *n2, int len2);
void copy(NumBox *n1, int *offset, NumBox *n2, int len2);
void set_pawns(NumBox pawns[6], Coul color);
void depth_first_search(NumBox *cells, int *offset, NumBox pawn, int moves, NumBox forbidden, NumBox player);
NumBox *get_moves(int *moves_count, NumBox pawn);
NumBox *get_cells_by_color(Coul color);
Border get_opposite_border(Border bor);
Coul get_other_player(Coul currentPlayer);

#endif


#ifndef MODEL_H
#define MODEL_H

#include "graphics.h"

typedef enum Border
{
    TOP, BOTTOM, LEFT, RIGHT
} Border;

typedef enum TYPE
{
    VIDE, LICORNE, PALADIN
} TYPE;

typedef enum COUL
{
    NOIR, BLANC
} COUL;

typedef enum Gamemode
{
    PVP, PVC /* Player VS Player, Player VS Computer */
} Gamemode;

typedef struct NUMBOX
{
    int x;
    int y;
} NUMBOX;

typedef struct BOX
{
    int lisere;/* 1, 2, 3 */
    TYPE typeP;
    COUL coulP;
} BOX;

#define AI_COLOR BLANC

BOX plateau[6][6];

int  is_lisere_valid(int lastEdging, NUMBOX start);
int  can_override(NUMBOX start, NUMBOX end);
int  is_cell_occupied(NUMBOX pos);
int  can_move(NUMBOX pieceCell);
int  in_range(NUMBOX pos);
int  get_lisere(NUMBOX n);
int  eql(NUMBOX n1, NUMBOX n2);
int  contains(NUMBOX *ns, int len, NUMBOX n);
int  can_other_player_move(COUL player, int lastEdging);
int  is_AI_turn(COUL currentPlayer, Gamemode mode);
int  move_piece(NUMBOX start, NUMBOX end);
void init_plateau();
void init_piece1_debug();
void init_piece2_debug();
void random_move(COUL coulP, NUMBOX *start, NUMBOX *end);
void get_neighbours(NUMBOX neigh[4], NUMBOX cell);
void uniq(NUMBOX *ns, int *len);
void print_numBoxes(NUMBOX *n, int len);
void print_numBox(NUMBOX n);
void remove_numBox(NUMBOX *ns, int *len, NUMBOX n);
void append(NUMBOX *ns, int *len, NUMBOX n);
void remove_numBoxes(NUMBOX *n1, int *len1, NUMBOX *n2, int len2);
void copy(NUMBOX *n1, int *offset, NUMBOX *n2, int len2);
void set_pieces(NUMBOX pieces[6], COUL coulP);
void depth_first_search(NUMBOX *cells, int *offset, NUMBOX piece, int moves,
                        NUMBOX forbidden, NUMBOX player);
NUMBOX *get_moves(int *moves_count, NUMBOX piece);
NUMBOX *get_cells_by_coulP(COUL coulP);
Border get_opposite_border(Border bor);
COUL   get_other_player(COUL currentPlayer);

#endif


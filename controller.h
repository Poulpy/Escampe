#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"

typedef enum Gamemode
{
    PVP, PVC /* Player VS Player, Player VS Computer */
} Gamemode;

void init_game(int *interface, Gamemode *mode, Border *bor);
void AI_game(int interface, NumBox *start, NumBox *end, Coul color, Type *type1, Type *type2, int *lastEdging);
void player_play_turn(int interface, NumBox *n1, NumBox *n2, Coul color, Type *type1, Type *type2, int *lastEdging);
void players_place_pawns(Border bor, int interface, Gamemode mode);
void position_pawns(NumBox pos[6], Border bor, Coul color, int interface);
void position_AI_pawns(NumBox pos[6], Border bor, int interface);
int  get_interface_choice(POINT click);
int  replay(POINT click);
int  is_on_player_side(POINT click, int interface, Coul color);
int  is_cell_valid(POINT click, int lastEdging, int interface);
int  player_choose_to_replay();
int  get_border_choice(POINT click);
int  player_choose_interface();
int  is_in_border(NumBox pos, Border bor, int interface);
int  is_on_board(POINT click);
int  is_between_points(POINT p1, POINT c1, POINT c2);
Border player_choose_border();
NumBox point_to_numbox(POINT p, int interface);
NumBox point_to_numbox_ig1(POINT p);
NumBox point_to_numbox_ig2(POINT p);
Gamemode player_choose_gamemode();
Gamemode get_gamemode_choice(POINT click);
POINT numbox_to_point(NumBox n, int interface);
POINT numbox_to_point_ig1(NumBox n);
POINT numbox_to_point_ig2(NumBox n);
COULEUR get_color_by_player(Coul color);

#endif


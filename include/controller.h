#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"


void init_game(int *interface, Gamemode *mode, Border *bor);
void players_place_pawns(Border bor, int interface, Gamemode mode);
void place_random_pawn(NumBox *cell, Border bor, int interface);
void player_place_pawn(NumBox *cell, Border bor, int interface);
void position_pawns(NumBox pawns[6], Border bor, Coul player, int interface, int is_ai);
int  get_interface_choice(POINT click);
int  is_on_player_side(NumBox cell, int interface, Coul color);
int  is_cell_valid(NumBox selectedCell, int lastEdging, int interface);
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
POINT numbox_to_point(NumBox n, int interface);
POINT numbox_to_point_ig1(NumBox n);
POINT numbox_to_point_ig2(NumBox n);
COULEUR get_color_by_player(Coul color);

#endif


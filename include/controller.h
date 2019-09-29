#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"


void init_game(int *interface, Gamemode *mode, Border *bor);
void players_place_pieces(Border bor, int interface, Gamemode mode);
void place_random_piece(NUMBOX *cell, Border bor, int interface);
void player_place_piece(NUMBOX *cell, Border bor, int interface);
void position_pieces(NUMBOX pieces[6], Border bor, COUL player, int interface, int is_ai);
int  get_interface_choice(POINT click);
int  is_on_player_side(NUMBOX cell, int interface, COUL coulP);
int  is_cell_valid(NUMBOX selectedCell, int lastEdging, int interface);
int  player_choose_to_replay();
int  get_border_choice(POINT click);
int  player_choose_interface();
int  is_in_border(NUMBOX pos, Border bor, int interface);
int  is_on_board(POINT click);
int  is_between_points(POINT p1, POINT c1, POINT c2);
Border player_choose_border();
NUMBOX point_to_numBox(POINT p, int interface);
NUMBOX point_ig1_to_numBox(POINT p);
NUMBOX point_ig2_to_numBox(POINT p);
Gamemode player_choose_gamemode();
POINT numBox_to_point(NUMBOX n, int interface);
POINT numBox_to_pointBG_ig1(NUMBOX n);
POINT numBox_to_pointBG_ig2(NUMBOX n);
COULEUR get_coulP_by_player(COUL coulP);

#endif


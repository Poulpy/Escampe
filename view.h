#ifndef VIEW_H
#define VIEW_H

#include "model.h"
#include "controller.h"

#define HIGHLIGHT_COLOR darkolivegreen
#define BACKGROUND_COLOR 0xBA963E

#define SHADE 0x141414

#define FIRST_COLOR (BACKGROUND_COLOR - SHADE)
#define SECON_COLOR (FIRST_COLOR - SHADE)
#define THIRD_COLOR (SECON_COLOR - SHADE)
#define FOURT_COLOR (THIRD_COLOR - SHADE)

#define WHITE_PLAYER_COLOR white
#define BLACK_PLAYER_COLOR 0x6d6fc6

#define CIRCLE_RADIUS 35
#define CELL_WIDTH 70
#define CELL_HEIGHT CELL_WIDTH

#define MARGIN 100

#define MID_WIDTH 3 * CELL_WIDTH + MARGIN
#define MID_HEIGHT 3 * CELL_HEIGHT + MARGIN

#define BOARD_HEIGHT 6 * CELL_HEIGHT
#define BOARD_WIDTH 6 * CELL_WIDTH

#define WIDTH BOARD_WIDTH + MARGIN * 2
#define HEIGHT BOARD_HEIGHT + MARGIN * 2

void draw_edging(POINT center, int number);
void draw_gameboard(int interface);
void draw_unicorn(POINT origin, COULEUR color);
void draw_paladin(POINT origin, COULEUR color);
void draw_edging(POINT bl_corner, int number);
void display_interface_choice();
void draw_pawn(NumBox pawn, int interface);
void display_gamemode_choice();
void display_endgame_menu(Coul color);
void erase_information();
void display_informations(Coul playerColor, int lastEdging);
void highlight_cell(NumBox cell, COULEUR color, int interface);
void highlight_cells(NumBox *cells, int len, COULEUR color, int interface);
void erase_highlighting(NumBox *cells, int len, int interface);
void erase_highlight(NumBox cell, int interface);
void display_turn_helper(COULEUR textColor, int lastEdging);
void display_border_choice();
void erase_highlightings(NumBox *moves, NumBox pawn, int moves_count, int interface);
void erase_pawn(NumBox pawn, int interface);
void draw_move(NumBox start, NumBox end, int interface);

#endif


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

#define BLANC_PLAYER_COLOR white
#define NOIR_PLAYER_COLOR 0x6d6fc6

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

void draw_lisere(POINT center, int number);
void affiche_plateau(int interface);
void affiche_licorne(POINT origin, COULEUR coulP);
void affiche_paladin(POINT origin, COULEUR coulP);
void draw_lisere(POINT bl_corner, int number);
void display_interface_choice();
void affiche_piece(NUMBOX piece, int interface);
void display_gamemode_choice();
void display_endgame_menu(COUL coulP);
void display_button(char *text, POINT bl_corner, POINT ur_corner, COULEUR back, COULEUR front, int textSize);
void erase_information();
void display_informations(COUL playerColor, int lastEdging);
void highlight_cell(NUMBOX cell, COULEUR coulP, int interface, int display);
void highlight_cells(NUMBOX *cells, int len, COULEUR coulP, int interface, int display);
void erase_highlight(NUMBOX cell, int interface, int display);
void erase_highlights(NUMBOX *cells, int len, int interface, int display);
void display_turn_helper(COULEUR textColor, int lastEdging);
void display_border_choice();
void erase_highlightings(NUMBOX *moves, NUMBOX piece, int moves_count, int interface);
void erase_piece(NUMBOX piece, int interface);
void draw_move(NUMBOX start, NUMBOX end, int interface);
void draw_tiny_plateau();
void erase_window_except_plateau();

#endif


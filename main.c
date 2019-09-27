/* Escampe project by HAUSER Joshua & REPAIN Paul */
/* IATIC3 2019 */

/* Libraries */

#include "controller.h"

/* Global variables */


/* Main */

int main()
{
    NumBox n1, n2;
    int interface, inGame = 1, lastEdging = 0, turns = 0;
    Coul color;
    Type type1, type2;
    Gamemode mode;
    Border bor;

    init_graphics(WIDTH, HEIGHT);
    affiche_auto_off();

    // Game loop
    do
    {
        color = WHITE;
        lastEdging = 0;
        init_game(&interface, &mode, &bor);
        players_place_pawns(bor, interface, mode);

        // Turn loop
        do
        {
            if (color == BLACK && can_any_pawn_move(WHITE, &lastEdging) && turns != 0) color = WHITE;
            else if (color == WHITE && can_any_pawn_move(BLACK, &lastEdging) && turns != 0) color = BLACK;

            display_informations(color, lastEdging);

            if (color == WHITE && mode == PVC) AI_game(interface, &n1, &n2, color, &type1, &type2, &lastEdging);
            else
                player_play_turn(interface, &n1, &n2, color, &type1, &type2, &lastEdging);

            affiche_all();
            turns++;
        } while (!is_unicorn_captured(type1, type2));

        display_endgame_menu(color);
        inGame = player_choose_to_replay();
        fill_screen(BACKGROUND_COLOR);
    } while (inGame);

    return 0;
}


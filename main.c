/* Escampe project by HAUSER Joshua & REPAIN Paul */
/* IATIC3 2019 */

/* Libraries */

#include "controller.h"

/* Main */

int main()
{
    int interface, moves_count, lastEdging, turn;
    int gameFinished, inGame;
    NumBox n1, n2, *moves;
    POINT click1, click2;
    Coul player;
    Gamemode mode;
    Border bor;

    init_graphics(WIDTH, HEIGHT);
    affiche_auto_off();

    // Game loop
    do
    {
        player = WHITE;
        lastEdging = 0;
        turn = 0;
        init_game(&interface, &mode, &bor);
        players_place_pawns(bor, interface, mode);

        // Turn loop
        do
        {
            if (turn++ != 0)
            {
                if (can_other_player_move(player, lastEdging))
                    player = get_other_player(player);
                else
                    lastEdging = 0;
            }

            display_informations(player, lastEdging);

            if (is_AI_turn(player, mode))
            {
                random_move(player, &n1, &n2);
                highlight_cell(n2, get_color_by_player(player), interface, 1);
                sleep(2);
                erase_highlight(n2, interface);
            }
            else
            {
                do
                {
                    click1 = wait_clic();
                    n1 = point_to_numbox(click1, interface);
                } while (!is_on_board(click1) ||
                         !is_cell_valid(n1, lastEdging, interface) ||
                         !is_on_player_side(n1, interface, player));

                moves = get_moves(&moves_count, n1);

                highlight_cell(n1, get_color_by_player(player), interface, 0);
                highlight_cells(moves, moves_count, HIGHLIGHT_COLOR, interface);

                do
                {
                    click2 = wait_clic();
                    n2 = point_to_numbox(click2, interface);
                    if(is_on_player_side(n2, interface, player) &&
                       is_cell_valid(n2, lastEdging, interface))
                    {
                        erase_highlightings(moves, n1, moves_count, interface);

                        n1 = point_to_numbox(click2, interface);
                        moves = get_moves(&moves_count, n1);

                        highlight_cell(n1, get_color_by_player(player), interface, 0);
                        highlight_cells(moves, moves_count, HIGHLIGHT_COLOR, interface);
                    }
                } while (!contains(moves, moves_count, n2));

                erase_highlightings(moves, n1, moves_count, interface);
            }

            gameFinished = move_pawn(n1, n2);
            draw_move(n1, n2, interface);
            lastEdging = gameboard[n2.y][n2.x].edging;

            affiche_all();
            // Change function to get pawns types
        } while (!gameFinished);

        display_endgame_menu(player);
        inGame = player_choose_to_replay();
        fill_screen(BACKGROUND_COLOR);
    } while (inGame);

    return 0;
}


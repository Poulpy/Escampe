/* Escampe project by HAUSER Joshua & REPAIN Paul */
/* IATIC3 2019 */

/* Libraries */

#include "include/controller.h"

/* Main */

int main()
{
    int interface, moves_count, lastEdging, turn, gameFinished, inGame;
    NUMBOX n1, n2, *moves;
    POINT click1, click2;
    COUL player;
    Gamemode mode;
    Border bor;

    init_graphics(WIDTH, HEIGHT);
    affiche_auto_off();

    // Game loop
    do
    {
        player = BLANC;
        lastEdging = 0;
        turn = 0;
        init_game(&interface, &mode, &bor);
        players_place_pieces(bor, interface, mode);

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
                highlight_cell(n1, get_coulP_by_player(player), interface, 1);
                sleep(1);
                highlight_cell(n2, HIGHLIGHT_COLOR, interface, 1);
                sleep(1);
                erase_highlight(n2, interface, 0);
            }
            else
            {
                do
                {
                    click1 = wait_clic();
                    n1 = point_to_numBox(click1, interface);
                } while (!is_on_board(click1) ||
                         !is_cell_valid(n1, lastEdging, interface) ||
                         !is_on_player_side(n1, interface, player));

                moves = get_moves(&moves_count, n1);

                highlight_cell(n1, get_coulP_by_player(player), interface, 0);
                highlight_cells(moves, moves_count, HIGHLIGHT_COLOR, interface, 1);

                do
                {
                    click2 = wait_clic();
                    n2 = point_to_numBox(click2, interface);
                    if(is_on_player_side(n2, interface, player) &&
                       is_cell_valid(n2, lastEdging, interface))
                    {
                        erase_highlight(n1, interface, 0);
                        erase_highlights(moves, moves_count, interface, 1);

                        n1 = point_to_numBox(click2, interface);
                        moves = get_moves(&moves_count, n1);

                        highlight_cell(n1, get_coulP_by_player(player), interface, 0);
                        highlight_cells(moves, moves_count, HIGHLIGHT_COLOR, interface, 1);
                    }
                } while (!contains(moves, moves_count, n2));

                erase_highlights(moves, moves_count, interface, 0);
            }

            erase_highlight(n1, interface, 0);

            gameFinished = move_piece(n1, n2);
            draw_move(n1, n2, interface);
            lastEdging = plateau[n2.y][n2.x].lisere;

        } while (!gameFinished);

        display_endgame_menu(player);
        inGame = player_choose_to_replay();
    } while (inGame);

    return 0;
}


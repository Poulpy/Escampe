#include "include/controller.h"

// Display the menu for chosing a border (TOP, BOTTOM, etc)
int get_border_choice(POINT click)
{
    int i, buttonWidth = 70;
    POINT squarePoints[4][2] = { { { MARGIN, BOARD_HEIGHT + MARGIN },
                                   { MARGIN + BOARD_WIDTH, BOARD_HEIGHT + MARGIN + buttonWidth } },
                                 { { MARGIN - buttonWidth, MARGIN },
                                   { MARGIN, BOARD_HEIGHT + MARGIN } },
                                 { { MARGIN + BOARD_WIDTH, MARGIN },
                                   { MARGIN + BOARD_WIDTH + buttonWidth, MARGIN + BOARD_HEIGHT } },
                                 { { MARGIN, MARGIN - buttonWidth },
                                   { MARGIN + BOARD_WIDTH, MARGIN } } };
    Border choice[] = { TOP, LEFT, RIGHT, BOTTOM };

    for (i = 0; i != 4; i++)
    {
        if (is_between_points(click, squarePoints[i][0], squarePoints[i][1]))
        {
            return choice[i];
        }
    }

    return -1;
}

// Checks if a point p1 is between 2 points c1 and c2
int is_between_points(POINT p1, POINT c1, POINT c2)
{
    if (c1.y < c2.y)
        return p1.x >= c1.x && p1.x <= c2.x && p1.y >= c1.y && p1.y <= c2.y;
    return p1.x >= c1.x && p1.x <= c2.x && p1.y <= c1.y && p1.y >= c2.y;
}

// Returns the coulP of a player
COULEUR get_coulP_by_player(COUL coulP)
{
    if (coulP == NOIR) return NOIR_PLAYER_COLOR;
    return BLANC_PLAYER_COLOR;
}

// Let the user place a piece in the plateau, given a click
// IT IS NOT added to plateau
void player_place_piece(NUMBOX *cell, Border bor, int interface)
{
    POINT click;

    do
    {
        click = wait_clic();
        *cell = point_to_numBox(click, interface);
    } while (!is_on_board(click) || !is_in_border(*cell, bor, interface));
}

// Let an AI place a piece in the plateau
// IT IS NOT added to plateau
void place_random_piece(NUMBOX *cell, Border bor, int interface)
{
    do
    {
        cell->x = alea_int(6);
        cell->y = alea_int(6);
    } while (!is_in_border(*cell, bor, interface));
}

// Pawns are placed for one player (AI or human)
// /The first piece placed is the unicorn
void position_pieces(NUMBOX pieces[6], Border bor, COUL player, int interface,
                    int is_ai)
{
    int i;
    COULEUR coulP = get_coulP_by_player(player);

    if (is_ai)
    {
        place_random_piece(&(pieces[0]), bor, interface);
    }
    else
    {
        player_place_piece(&(pieces[0]), bor, interface);
    }

    affiche_licorne(numBox_to_point(pieces[0], interface), coulP);
    affiche_all();

    for (i = 1; i != 6; i++)
    {
        do
        {
            if (is_ai) place_random_piece(&(pieces[i]), bor, interface);
            else player_place_piece(&(pieces[i]), bor, interface);
        } while (contains(pieces, i, pieces[i]));

        affiche_paladin(numBox_to_point(pieces[i], interface), coulP);
        affiche_all();
    }
}

// Check if a Point click is on the plateau
int is_on_board(POINT click)
{
    return click.x >= MARGIN && click.x <= (WIDTH - MARGIN) &&
           click.y >= MARGIN && click.y <= (HEIGHT - MARGIN);
}

// Let the player choose :
// - an interface
// - a gamemode
// - a border
// The plateau is also drawn
void init_game(int *interface, Gamemode *mode, Border *bor)
{
    fill_screen(BACKGROUND_COLOR);

    display_interface_choice();
    *interface = player_choose_interface();

    display_gamemode_choice();
    *mode = player_choose_gamemode();

    init_plateau();

    affiche_plateau(*interface);

    display_border_choice();
    *bor = player_choose_border();
    erase_window_except_plateau();
}

// Both players place their pieces
// The black player places his pieces first, according to the border bor
// The white player places his pieces on the opposite border
void players_place_pieces(Border bor, int interface, Gamemode mode)
{
    int players[] = { 0, 0 };
    NUMBOX white_pieces[6], black_pieces[6];

    if (PVC == mode)
    {
        if (AI_COLOR == NOIR) players[0] = 1;
        else                   players[1] = 1;
    }

    position_pieces(black_pieces, bor, NOIR, interface, players[0]);
    position_pieces(white_pieces, get_opposite_border(bor), BLANC, interface, players[1]);

    set_pieces(black_pieces, NOIR);
    set_pieces(white_pieces, BLANC);
}

// Let the player choose a border, Top, Bottom, Right, Left, given a click
Border player_choose_border()
{
    Border bor;
    POINT choice;

    do
    {
        choice = wait_clic();
        bor = get_border_choice(choice);
    } while (bor == -1);

    return bor;
}

// Converts a NUMBOX n to a Point, given an interface (1 or 2)
POINT numBox_to_point(NUMBOX n, int interface)
{
    if (interface == 1) return numBox_to_pointBG_ig1(n);
    else return numBox_to_pointBG_ig2(n);
}

// Converts a Point p to a NUMBOX, given an interface (1 or 2)
NUMBOX point_to_numBox(POINT p, int interface)
{
    if (interface == 1) return point_ig1_to_numBox(p);
    else return point_ig2_to_numBox(p);
}

// Converts a NUMBOX n to a Point for the interface 1
POINT numBox_to_pointBG_ig1(NUMBOX n)
{
    POINT p;

    p.x = MARGIN + n.x * CELL_WIDTH;
    p.y = MARGIN + n.y * CELL_HEIGHT;

    return p;
}

// Converts a Point p to a NUMBOX for the interface 1
NUMBOX point_ig1_to_numBox(POINT p)
{
    NUMBOX n;

    n.x = (p.x - MARGIN) / CELL_WIDTH;
    n.y = (p.y - MARGIN) / CELL_HEIGHT;

    return n;
}

// Converts a NUMBOX n to a Point for the interface 2
POINT numBox_to_pointBG_ig2(NUMBOX n)
{
    POINT p;

    p.x = MARGIN + (n.y * CELL_WIDTH);
    p.y = MARGIN + BOARD_HEIGHT - CELL_HEIGHT - (n.x * CELL_HEIGHT);

    return p;
}

// Converts a Point p to a NUMBOX for the interface 2
NUMBOX point_ig2_to_numBox(POINT p)
{
    NUMBOX n;

    n.y = (p.x - MARGIN) / CELL_WIDTH;
    n.x = (MARGIN + BOARD_HEIGHT - p.y) / CELL_HEIGHT;

    return n;
}

// Let the player choose an interface, 1 or 2, given a click
int player_choose_interface()
{
    int choice;

    do
    {
        choice = get_interface_choice(wait_clic());
    } while (choice == -1);

    return choice;
}

// Check if the user clicks on a button of interface 1 or 2
int get_interface_choice(POINT click)
{
    POINT p1, p2, p3, p4;
    p1.x = MARGIN / 2;
    p1.y = MID_HEIGHT + 20;
    p2.y = p1.y - 80;
    p2.x = p1.x + 250;

    p3 = p1;
    p3.x = MID_WIDTH;
    p4.y = p3.y - 80;
    p4.x = p3.x + 250;

    if (is_between_points(click, p1, p2)) return 1;
    else if (is_between_points(click, p3, p4)) return 2;
    return -1;
}

// Let the player choose a mode, Player VS Player or Player VS Computer
Gamemode player_choose_gamemode()
{
    POINT click = wait_clic();

    if (click.x >= 0 && click.x < MID_WIDTH) return PVP;
    else return PVC;
}

// Check if the piece on the NUMBOX cell belongs to the player coulP
int is_on_player_side(NUMBOX cell, int interface, COUL coulP)
{
    return is_cell_occupied(cell) && plateau[cell.y][cell.x].coulP == coulP;
}

// Let the player choose to play again or quit the game
int player_choose_to_replay()
{
    POINT click = wait_clic();
    return click.y >= 0 && click.x < MID_WIDTH;
}

// Check if the number of liseres of a cell is correct, and if the cell
// is occupied
int is_cell_valid(NUMBOX selectedCell, int lastEdging, int interface)
{
    return is_lisere_valid(lastEdging, selectedCell) &&
           is_cell_occupied(selectedCell);
}


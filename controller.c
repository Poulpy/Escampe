#include "controller.h"

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

COULEUR get_color_by_player(Coul color)
{
    if (color == BLACK) return BLACK_PLAYER_COLOR;
    return WHITE_PLAYER_COLOR;
}

void player_place_pawn(NumBox *cell, Border bor, int interface)
{
    POINT click;

    do
    {
        click = wait_clic();
        *cell = point_to_numbox(click, interface);
    } while (!is_on_board(click) || !is_in_border(*cell, bor, interface));
}

void place_random_pawn(NumBox *cell, Border bor, int interface)
{
    do
    {
        cell->x = alea_int(6);
        cell->y = alea_int(6);
    } while (!is_in_border(*cell, bor, interface));
}

// Pawns are placed for one player (AI or human)
void position_pawns(NumBox pawns[6], Border bor, Coul player, int interface, int is_ai)
{
    int i;
    COULEUR color = get_color_by_player(player);

    if (is_ai)
    {
        place_random_pawn(&(pawns[0]), bor, interface);
    }
    else
    {
        player_place_pawn(&(pawns[0]), bor, interface);
    }

    draw_unicorn(numbox_to_point(pawns[0], interface), color);
    affiche_all();

    for (i = 1; i != 6; i++)
    {
        do
        {
            if (is_ai) place_random_pawn(&(pawns[i]), bor, interface);
            else player_place_pawn(&(pawns[i]), bor, interface);
        } while (contains(pawns, i, pawns[i]));

        draw_paladin(numbox_to_point(pawns[i], interface), color);
        affiche_all();
    }
}

// Check if a point click is on the gameboard
int is_on_board(POINT click)
{
    return click.x >= MARGIN && click.x <= (WIDTH - MARGIN) &&
           click.y >= MARGIN && click.y <= (HEIGHT - MARGIN);
}

// Let the player choose :
// - an interface
// - a gamemode
// - a border
// The gameboard is also drawn
void init_game(int *interface, Gamemode *mode, Border *bor)
{
    fill_screen(BACKGROUND_COLOR);

    display_interface_choice();
    *interface = player_choose_interface();

    display_gamemode_choice();
    *mode = player_choose_gamemode();

    init_gameboard();

    draw_gameboard(*interface);

    display_border_choice();
    *bor = player_choose_border();
    erase_window_except_gameboard();
}

// Both players place their pawns
// The black player places his pawns first, according to the border bor
// The white player places his pawns on the opposite border
void players_place_pawns(Border bor, int interface, Gamemode mode)
{
    NumBox white_pawns[6], black_pawns[6];

    if (PVC == mode)
    {
        if (AI_COLOR == BLACK)
        {
            position_pawns(black_pawns, bor, BLACK, interface, 1);
            position_pawns(white_pawns, get_opposite_border(bor), WHITE, interface, 0);
        }
        else
        {
            position_pawns(black_pawns, bor, BLACK, interface, 0);
            position_pawns(white_pawns, get_opposite_border(bor), WHITE, interface, 1);
        }
    }
    else
    {
        position_pawns(black_pawns, bor, BLACK, interface, 0);
        position_pawns(white_pawns, get_opposite_border(bor), WHITE, interface, 0);
    }

    set_pawns(black_pawns, BLACK);
    set_pawns(white_pawns, WHITE);
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



POINT numbox_to_point(NumBox n, int interface)
{
    if (interface == 1) return numbox_to_point_ig1(n);
    else return numbox_to_point_ig2(n);
}

NumBox point_to_numbox(POINT p, int interface)
{
    if (interface == 1) return point_to_numbox_ig1(p);
    else return point_to_numbox_ig2(p);
}

POINT numbox_to_point_ig1(NumBox n)
{
    POINT p;

    p.x = MARGIN + n.x * CELL_WIDTH;
    p.y = MARGIN + n.y * CELL_HEIGHT;

    return p;
}

NumBox point_to_numbox_ig1(POINT p)
{
    NumBox n;

    n.x = (p.x - MARGIN) / CELL_WIDTH;
    n.y = (p.y - MARGIN) / CELL_HEIGHT;

    return n;
}

POINT numbox_to_point_ig2(NumBox n)
{
    POINT p;

    p.x = MARGIN + (n.y * CELL_WIDTH);
    p.y = MARGIN + BOARD_HEIGHT - CELL_HEIGHT - (n.x * CELL_HEIGHT);

    return p;
}

NumBox point_to_numbox_ig2(POINT p)
{
    NumBox n;

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

// Check if the pawn on the NumBox cell is of the same color
int is_on_player_side(NumBox cell, int interface, Coul color)
{
    return is_cell_occupied(cell) && gameboard[cell.y][cell.x].color == color;
}

// Let the player choose to play again or quit
int player_choose_to_replay()
{
    POINT click = wait_clic();
    return click.y >= 0 && click.x < MID_WIDTH;
}

int is_cell_valid(NumBox selectedCell, int lastEdging, int interface)
{
    return is_edging_valid(lastEdging, selectedCell) &&
           is_cell_occupied(selectedCell);
}


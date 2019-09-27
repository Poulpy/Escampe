#include "controller.h"

int get_border_choice(POINT click)
{
    int i, squareSize= 75;
    POINT squarePoints[4][2] = {{ (POINT) {MID_WIDTH - (squareSize / 2), HEIGHT - MARGIN},
                                 (POINT) {MID_WIDTH  + (squareSize / 2), HEIGHT - MARGIN - squareSize }},
                                { (POINT) {(MID_WIDTH) / 4 + (squareSize / 2), MID_HEIGHT + (squareSize / 2)},
                                 (POINT) {(MID_WIDTH) / 4 + (squareSize / 2) + squareSize, MID_HEIGHT - (squareSize / 2)}},
                                { (POINT) {MID_WIDTH + ((MID_WIDTH) / 2) - (squareSize / 2), MID_HEIGHT + (squareSize / 2)},
                                 (POINT) {MID_WIDTH + ((MID_WIDTH) / 2) + (squareSize / 2), MID_HEIGHT - (squareSize / 2)}},
                                { (POINT) {MID_WIDTH - (squareSize / 2), MARGIN + squareSize},
                                 (POINT) {MID_WIDTH + (squareSize / 2), MARGIN}}
                                };
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

int is_between_points(POINT p1, POINT c1, POINT c2)
{
    return p1.x >= c1.x && p1.x <= c2.x && p1.y <= c1.y && p1.y >= c2.y;
}

COULEUR get_color_by_player(Coul color)
{
    if (color == BLACK) return BLACK_PLAYER_COLOR;
    return WHITE_PLAYER_COLOR;
}


void position_pawns(NumBox pos[6], Border bor, Coul color, int interface)
{
    int i;
    POINT click;
    do
    {
        click = wait_clic();
        pos[0] = point_to_numbox(click, interface);
    } while (!is_in_border(pos[0], bor, interface) || !is_on_board(click));

    draw_unicorn(numbox_to_point(pos[0], interface), get_color_by_player(color));
    affiche_all();

    for (i = 1; i != 6; i++)
    {
        do
        {
            click = wait_clic();
            pos[i] = point_to_numbox(click, interface);
        } while (!is_in_border(pos[i], bor, interface) || contains(pos, i, pos[i]) || !is_on_board(click));
        draw_paladin(numbox_to_point(pos[i], interface), get_color_by_player(color));
        affiche_all();
    }
}

void position_AI_pawns(NumBox pos[6], Border bor, int interface)
{
    int i;

    do
    {
        pos[0].x = alea_int(6);
        pos[0].y = alea_int(6);
    } while (!is_in_border(pos[0], bor, interface));
    draw_unicorn(numbox_to_point(pos[0], interface), WHITE_PLAYER_COLOR);
    affiche_all();
    for (i = 1; i != 6; i++)
    {
        do
        {
            pos[i].x = alea_int(6);
            pos[i].y = alea_int(6);
        } while (!is_in_border(pos[i], bor, interface) || contains(pos, i, pos[i]));

        draw_paladin(numbox_to_point(pos[i], interface), WHITE_PLAYER_COLOR);
    }
    affiche_all();
}
int is_on_board(POINT click)
{
    return click.x >= MARGIN && click.x <= (WIDTH - MARGIN) && click.y >= MARGIN && click.y <= (HEIGHT - MARGIN);
}

/* Controller */

void init_game(int *interface, Gamemode *mode, Border *bor)
{
    display_interface_choice();
    *interface = player_choose_interface();

    display_gamemode_choice();
    *mode = player_choose_gamemode();

    init_gameboard();

    display_border_choice();
    *bor = player_choose_border();
    draw_gameboard(*interface);
}

void players_place_pawns(Border bor, int interface, Gamemode mode)
{
    NumBox white_pawns[6], black_pawns[6];

    position_pawns(black_pawns, bor, BLACK, interface);
    place_pawns(black_pawns, BLACK);

    if (mode == PVC) position_AI_pawns(white_pawns, opposite_border(bor), interface);
    else position_pawns(white_pawns, opposite_border(bor), WHITE, interface);

    place_pawns(white_pawns, WHITE);
}

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

int player_choose_interface()
{
    return get_interface_choice(wait_clic());
}

int get_interface_choice(POINT click)
{
    if (click.x >= 0 && click.x < MID_WIDTH) return 1;
    else return 2;
}

Gamemode player_choose_gamemode()
{
    return get_gamemode_choice(wait_clic());
}

Gamemode get_gamemode_choice(POINT click)
{
    if (click.x >= 0 && click.x < MID_WIDTH) return PVP;
    else return PVC;
}

int is_on_player_side(POINT click, int interface, Coul color)
{
    NumBox cell = point_to_numbox(click, interface);

    return (is_on_board(click) && (gameboard[cell.y][cell.x].type == PALADIN || gameboard[cell.y][cell.x].type == UNICORN ) && gameboard[cell.y][cell.x].color == color);
}


int player_choose_to_replay()
{
    return replay(wait_clic());
}

int replay(POINT click)
{
    return click.y >= 0 && click.x < MID_WIDTH;
}

void set_game_finished(int* finished)
{
     *finished = 1;
}

void player_play_turn(int interface, NumBox *n1, NumBox *n2, Coul color, Type *type1, Type *type2, int *lastEdging)
{
    int moves_count;
    POINT click1, click2;
    NumBox *moves;

    do
    {
        click1 = wait_clic();
    } while (!is_cell_valid(click1, *lastEdging, interface) || !is_on_player_side(click1, interface, color));

    *n1 = point_to_numbox(click1, interface);
    moves = highlight_player_and_moves(n1, color, &moves_count, interface);

    do
    {
        click2 = wait_clic();

        if(is_on_player_side(click2, interface, color) && is_cell_valid(click2, *lastEdging, interface))
        {
            erase_highlightings(moves, *n1, moves_count, interface);

            *n1 = point_to_numbox(click2, interface);
            moves = highlight_player_and_moves(n1, color, &moves_count, interface);
        }
        else *n2 = point_to_numbox(click2, interface);
    } while (!contains(moves, moves_count, *n2));

    *type1 = gameboard[n1->y][n1->x].type;
    *type2 = gameboard[n2->y][n2->x].type;

    erase_pawn(*n1, interface);
    erase_highlightings(moves, *n1, moves_count, interface);
    move_pawn(*n1, *n2);
    *lastEdging = gameboard[n2->y][n2->x].edging;
    draw_pawn(gameboard[n2->y][n2->x], numbox_to_point(*n2, interface));
}

void AI_game(int interface, NumBox *start, NumBox *end, Coul color, Type *type1, Type *type2, int *lastEdging)
{
    random_move(color, start, end);
    puts("debug");
    *type1 = gameboard[start->y][start->x].type;
    *type2 = gameboard[end->y][end->x].type;
    erase_pawn(*start, interface);
    puts("debug2");
    move_pawn(*start, *end);
    puts("debug3");
    *lastEdging = gameboard[end->y][end->x].edging;
    draw_pawn(gameboard[end->y][end->x], numbox_to_point(*end, interface));
}

int is_cell_valid(POINT click, int lastEdging, int interface)
{
    NumBox selectedCell = point_to_numbox(click, interface);
    return is_edging_valid(lastEdging, selectedCell) && is_cell_occupied(selectedCell);
}


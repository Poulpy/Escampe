/* Escampe project by HAUSER Joshua & REPAIN Paul */
/* IATIC3 2019 */

/* Libraries */

#include "graphics.h"

/* Constants */

#define DEBUG printf("**%d**\n", __LINE__);


#define EDGING_COLOR 0x22211d
#define HIGHLIGHT_COLOR green
#define BACKGROUND_COLOR 0xBA963E
#define WHITE_PLAYER_COLOR white
#define BLACK_PLAYER_COLOR blue

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

/* Enumerations */

typedef enum Border
{
    TOP, BOTTOM, LEFT, RIGHT
} Border;

typedef enum Type
{
    EMPTY, UNICORN, PALADIN
} Type;

typedef enum Coul
{
    BLACK, WHITE
} Coul;

typedef enum Gamemode
{
    PVP, PVC /* Player VS Player, Player VS Computer */
} Gamemode;

/* Structures */

typedef struct NumBox
{
    int x;
    int y;
} NumBox;

typedef struct Box
{
    int edging;/* 1, 2, 3 */
    Type type;
    Coul color;
} Box;

/* Global variables */

Box gameboard[6][6];

/* Functions prototypes */

/** Model **/

void init_gameboard();
void init_gamepawns_1();
void init_gamepawns_2();
void move_pawn(NumBox start, NumBox end);
void get_neighbours(NumBox *cells, int *offset, NumBox pawn, int moves, NumBox forbidden, NumBox player);
void random_move(Coul color, NumBox *start, NumBox *end);
void uniq(NumBox *ns, int *len);
int  is_edging_valid(int lastEdging, NumBox start);
int  is_any_pawn_playable(Coul color, int* lastEdging);
int  out_of_range(NumBox pos);
int  can_override(NumBox start, NumBox end);
int  is_cell_occupied(NumBox pos);
int  is_unicorn_alive(Type start, Type end);
int  box_eql(Box b1, Box b2);
int  is_in_border(NumBox pos, Border bor, int interface);
int  can_move(NumBox pawnCell);
NumBox *get_moves(int *moves_count, NumBox pawn);
NumBox *get_cells_by_color(Coul color);
Border opposite_border(Border bor);

/** View **/

void draw_edging(POINT center, int number);
void draw_gameboard(int interface);
void position_pawns(NumBox pos[6], Border bor, Coul color, int interface);
void position_AI_pawns(NumBox pos[6], Border bor, int interface);
void draw_unicorn(POINT origin, COULEUR color);
void draw_paladin(POINT origin, COULEUR color);
void erase_pawn(POINT origin);
void draw_pawn(Box pawn, POINT origin);
void display_interface_choice();
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
int  is_on_board(POINT click);
int is_between_points(POINT p1, POINT c1, POINT c2);


/** Controller **/

void init_game(int *interface, Gamemode *mode, Border *bor);
POINT  numbox_to_point(NumBox n, int interface);
POINT  numbox_to_point_ig1(NumBox n);
POINT  numbox_to_point_ig2(NumBox n);
NumBox point_to_numbox(POINT p, int interface);
NumBox point_to_numbox_ig1(POINT p);
NumBox point_to_numbox_ig2(POINT p);
int  get_interface_choice(POINT click);
Gamemode  get_gamemode_choice(POINT click);
int  replay(POINT click);
int  is_on_player_side(POINT click, int interface, Coul color);
NumBox *pick_pawn_and_move(NumBox *start, NumBox *end, POINT *click1, POINT *click2, int *moves_count, int interface);
void AI_game(int interface, NumBox *start, NumBox *end, Coul color, Type *type1, Type *type2, int *lastEdging);
int is_cell_valid(POINT click, int lastEdging, int interface);
int get_border_choice(POINT click);
void players_place_pawns(Border bor, int interface, Gamemode mode);
COULEUR get_color_by_player(Coul color);
Border player_choose_border();
int player_choose_interface();
Gamemode player_choose_gamemode();

/** Helpers **/

void print_numboxes(NumBox *n, int len);
void print_numbox(NumBox n);
int  eql(NumBox n1, NumBox n2);
void remove_numbox(NumBox *ns, int *len, NumBox n);
void append(NumBox *ns, int *len, NumBox n);
void remove_numboxes(NumBox *n1, int *len1, NumBox *n2, int len2);
void copy(NumBox *n1, int *offset, NumBox *n2, int len2);
int  contains(NumBox *ns, int len, NumBox n);
void place_pawns(NumBox pawns[6], Coul color);

/* Main */

int main()
{
    NumBox n1, n2, *moves;
    POINT click1, click2;
    int interface, moves_count, inGame = 1, lastEdging = 0, turns = 0;
    Coul color;
    Type type1, type2;
    Gamemode mode;
    COULEUR colors[2] = { BLACK_PLAYER_COLOR, WHITE_PLAYER_COLOR };
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

        //if (!is_any_pawn_playable(WHITE, &lastEdging)) color = BLACK;

        // Turn loop
        do
        {
            if (color == BLACK && is_any_pawn_playable(WHITE, &lastEdging) && turns != 0) color = WHITE;
            else if (color == WHITE && is_any_pawn_playable(BLACK, &lastEdging) && turns != 0) color = BLACK;

            display_informations(color, lastEdging);

            /*if (color == WHITE && mode == PVC)
            {
                random_move(color, &n1, &n2);
                type1 = gameboard[n1.y][n1.x].type;
                type2 = gameboard[n2.y][n2.x].type;
                erase_pawn(numbox_to_point(n1, interface));
                move_pawn(n1, n2);
                lastEdging = gameboard[n2.y][n2.x].edging;
                draw_pawn(gameboard[n2.y][n2.x], numbox_to_point(n2, interface));
                AI_game(interface, &n1, &n2, color, &type1, &type2, &lastEdging);
            }*/

            if (color == WHITE && mode == PVC) AI_game(interface, &n1, &n2, color, &type1, &type2, &lastEdging);
            else{

                do
                {
                    click1 = wait_clic();
                    n1 = point_to_numbox(click1, interface);
                } while (!is_cell_valid(click1, lastEdging, interface) || !is_on_player_side(click1, interface, color));

                highlight_cell(n1, colors[color], interface);

                moves = get_moves(&moves_count, n1);
                highlight_cells(moves, moves_count, HIGHLIGHT_COLOR, interface);

                do
                {
                    click2 = wait_clic();

                    if(is_on_player_side(click2, interface, color) && is_cell_valid(click2, lastEdging, interface))
                    {
                        erase_highlighting(moves, moves_count, interface);
                        erase_highlight(n1, interface);
                        n1 = point_to_numbox(click2, interface);
                        highlight_cell(n1, colors[color], interface);

                        moves = get_moves(&moves_count, n1);
                        highlight_cells(moves, moves_count, HIGHLIGHT_COLOR, interface);
                    }
                    else n2 = point_to_numbox(click2, interface);
                } while (!contains(moves, moves_count, n2));

                type1 = gameboard[n1.y][n1.x].type;
                type2 = gameboard[n2.y][n2.x].type;

                erase_pawn(numbox_to_point(n1, interface));
                erase_highlighting(moves, moves_count, interface);
                erase_highlight(n1, interface);
                move_pawn(n1, n2);
                lastEdging = gameboard[n2.y][n2.x].edging;
                draw_pawn(gameboard[n2.y][n2.x], numbox_to_point(n2, interface));
            }

            affiche_all();
            turns++;
        } while (is_unicorn_alive(type1, type2));

        display_endgame_menu(color);
        click1 = wait_clic();
        inGame = replay(click1);
        fill_screen(BACKGROUND_COLOR);
    } while (inGame);

    return 0;
}

/* Function definitions */

/* Model */

void init_gameboard()
{
    int edgings[6][6] = {{1,2,2,3,1,2},
                         {3,1,3,1,3,2},
                         {2,3,1,2,1,3},
                         {2,1,3,2,3,1},
                         {1,3,1,3,1,2},
                         {3,2,2,1,3,2}};
    int i, j;
    for (i = 0; i != 6; i++)
    {
        for (j = 0; j != 6; j++)
        {
            gameboard[i][j] = (Box) { edgings[i][j], EMPTY, BLACK };
        }
    }
}

void init_gamepawns_1()
{
    int i;

    NumBox whites[6] = {{4,1}, {4,3}, {4,4}, {5,1}, {5,2}, {5,3}};

    NumBox paladins[10] = {{0,2}, {0,4}, {1,0}, {1,2}, {1,5}, {4,1}, {4,3}, {4,4}, {5,1}, {5,2}};
    NumBox unicorns[2] = {{1,1}, {5,3}};

    for (i = 0; i != 6; i++) gameboard[whites[i].x][whites[i].y].color = WHITE;
    for (i = 0; i != 10; i++) gameboard[paladins[i].x][paladins[i].y].type = PALADIN;
    for (i = 0; i != 2; i++) gameboard[unicorns[i].x][unicorns[i].y].type = UNICORN;
}

void init_gamepawns_2()
{
    int i;

    NumBox whites[6] = {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}};
    NumBox paladins[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {5,0}, {5,1}, {5,2}, {5,4}, {5,5}};
    NumBox unicorns[2] = {{0,0}, {5,3}};

    for (i = 0; i != 6; i++) gameboard[whites[i].x][whites[i].y].color = WHITE;
    for (i = 0; i != 10; i++) gameboard[paladins[i].x][paladins[i].y].type = PALADIN;
    for (i = 0; i != 2; i++) gameboard[unicorns[i].x][unicorns[i].y].type = UNICORN;
}

int can_override(NumBox start, NumBox end)
{
    return (gameboard[start.y][start.x].type == PALADIN
            && gameboard[end.y][end.x].type == UNICORN
            && gameboard[start.y][start.x].color != gameboard[end.y][end.x].color);
}

int is_cell_occupied(NumBox pos)
{
    return gameboard[pos.y][pos.x].type != EMPTY;
}

void move_pawn(NumBox start, NumBox end)
{
    gameboard[end.y][end.x].type = gameboard[start.y][start.x].type;
    gameboard[end.y][end.x].color = gameboard[start.y][start.x].color;
    gameboard[start.y][start.x].type = EMPTY;
}

int is_edging_valid(int lastEdging, NumBox start)
{
    return (lastEdging == 0 || lastEdging == gameboard[start.y][start.x].edging);
}

// or is_unicorn_captured()
int is_unicorn_alive(Type start, Type end)
{
    return !(start == PALADIN && end == UNICORN);
}

int is_any_pawn_playable(Coul color, int* lastEdging)
{
    int i, j;
    NumBox pawnCell;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            pawnCell.x = j;
            pawnCell.y = i;
            if (gameboard[i][j].type != EMPTY && gameboard[i][j].color == color && is_edging_valid(*lastEdging, pawnCell) && can_move(pawnCell)) return 1;
        }
    }

    *lastEdging = 0;
    return 0;
}

void get_neighbours(NumBox *cells, int *offset, NumBox pawn, int moves, NumBox forbidden, NumBox player)
{
    int i;
    NumBox neigh[4];

    neigh[0] = (NumBox) { pawn.x + 1, pawn.y };
    neigh[1] = (NumBox) { pawn.x - 1, pawn.y };
    neigh[2] = (NumBox) { pawn.x, pawn.y - 1 };
    neigh[3] = (NumBox) { pawn.x, pawn.y + 1 };

    for (i = 0; i != 4; i++)
    {
        if (!out_of_range(neigh[i]) && !eql(neigh[i], forbidden))
        {
            if (!is_cell_occupied(neigh[i]) || can_override(player, neigh[i]))
            {
                if (moves == 0) append(cells, offset, neigh[i]);
                else get_neighbours(cells, offset, neigh[i], moves - 1, pawn, player);
            }
        }
    }
}

NumBox *get_moves(int *moves_count, NumBox pawn)
{
    int m = gameboard[pawn.y][pawn.x].edging;
    NumBox forbidden = { -1, -1 };

    *moves_count = 0;
    NumBox *moves = (NumBox *) malloc(sizeof(NumBox) * m * 4);

    get_neighbours(moves, moves_count, pawn, m - 1, forbidden, pawn);
    uniq(moves, moves_count);

    return moves;
}

void random_move(Coul color, NumBox *start, NumBox *end)
{
    int len;
    NumBox *cells, *ends;

    cells = get_cells_by_color(color);
    *start = cells[alea_int(6)];

    ends = get_moves(&len, *start);
    *end = ends[alea_int(len)];
}


int out_of_range(NumBox pos)
{
    return !(pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5);
}

NumBox *get_cells_by_color(Coul color)
{
    int cursor = 0, i, j;
    NumBox *cells = (NumBox *) malloc(sizeof(NumBox) * 6);

    for (i = 0; i != 6; i++)
    {
        for (j = 0; j != 6; j++)
        {
            if (gameboard[i][j].type != EMPTY && gameboard[i][j].color == color)
            {
                cells[cursor++] = (NumBox) { j, i };
            }
        }
    }

    return cells;
}

int is_in_border(NumBox pos, Border bor, int interface)
{
    switch (bor)
    {
    case TOP:
        return interface == 1 ? pos.y == 4 || pos.y == 5 : pos.x == 0 || pos.x == 1;
    case BOTTOM:
        return interface == 1 ? pos.y == 0 || pos.y == 1 : pos.x == 4 || pos.x == 5;
    case LEFT:
        return interface == 1 ? pos.x == 0 || pos.x == 1 : pos.y == 0 || pos.y == 1;
    case RIGHT:
        return interface == 1 ? pos.x == 4 || pos.x == 5 : pos.y == 4 || pos.y == 5;
    }

    return 0;
}

int can_move(NumBox pawnCell)
{
    int moves_count = 0;
    NumBox *moves = get_moves(&moves_count, pawnCell);
    free(moves);
    return moves_count != 0;
}

void place_pawns(NumBox pawns[6], Coul color)
{
    int i;
    gameboard[pawns[0].y][pawns[0].x].type = UNICORN;
    gameboard[pawns[0].y][pawns[0].x].color = color;

    for (i = 1; i != 6; i++)
    {
        gameboard[pawns[i].y][pawns[i].x].type = PALADIN;
        gameboard[pawns[i].y][pawns[i].x].color = color;
    }
}

/* View */

void display_interface_choice()
{
    POINT top, bottom, label;
    int size;

    fill_screen(BACKGROUND_COLOR);
    size = 30;
    label.x = MID_WIDTH - (size * 2) - 15;
    label.y = HEIGHT - size;
    aff_pol("Interface", size, label, red);

    top.x = MID_WIDTH;
    top.y = MID_HEIGHT + (MARGIN * 2);
    bottom.x = MID_WIDTH;
    bottom.y = MARGIN / 2;

    draw_line(top, bottom, white);

    size = 25;
    label.x = ((MID_WIDTH / 2) / 4) + size;
    label.y = MID_HEIGHT + 20;

    aff_pol("Interface 1", size, label, white);

    label.x = MID_WIDTH + ((MID_WIDTH / 2) / 4) + size;

    aff_pol("Interface 2", size, label, white);

    affiche_all();
}

void display_gamemode_choice()
{
    POINT top, bottom, label;
    int size = 30;

    label.x = MID_WIDTH - (size * 3);
    label.y = HEIGHT - size;

    fill_screen(BACKGROUND_COLOR);
    aff_pol("Mode de jeu", size, label, orange);

    top.x = MID_WIDTH;
    top.y = MID_HEIGHT + (MARGIN * 2);
    bottom.x = MID_WIDTH;
    bottom.y = MARGIN / 2;

    draw_line(top, bottom, white);

    size = 25;
    label.x = ((MID_WIDTH / 2) / 2) - (size / 2);
    label.y = MID_HEIGHT + (size * 2);
    aff_pol("Joueur", size, label, white);
    label.y = label.y - size - (size / 4);
    aff_pol("contre", size, label, white);
    label.y = label.y - size - (size / 4);
    aff_pol("joueur", size, label, white);

    label.x = MID_WIDTH + label.x;
    label.y = MID_HEIGHT + (size * 2);
    aff_pol("Joueur", size, label, white);
    label.y = label.y - size - (size / 4);
    aff_pol("contre", size, label, white);
    label.x = label.x + size;
    label.y = label.y - size - (size / 4);
    aff_pol("IA", size, label, white);
    affiche_all();
}

void display_border_choice()
{
    char* positions[4] = {"H", "G", "D", "B"};
    int i, squareSize= 75, textSize = 50;
    POINT squarePoints[4][2] = {{ (POINT) {MID_WIDTH - (squareSize / 2), HEIGHT - MARGIN},
                                 (POINT) {MID_WIDTH  + (squareSize / 2), HEIGHT - MARGIN - squareSize }},
                                { (POINT) {(MID_WIDTH) / 4 + (squareSize / 2), MID_HEIGHT + (squareSize / 2)},
                                 (POINT) {(MID_WIDTH) / 4 + (squareSize / 2) + squareSize, MID_HEIGHT - (squareSize / 2)}},
                                { (POINT) {MID_WIDTH + ((MID_WIDTH) / 2) - (squareSize / 2), MID_HEIGHT + (squareSize / 2)},
                                 (POINT) {MID_WIDTH + ((MID_WIDTH) / 2) + (squareSize / 2), MID_HEIGHT - (squareSize / 2)}},
                                { (POINT) {MID_WIDTH - (squareSize / 2), MARGIN + squareSize},
                                 (POINT) {MID_WIDTH + (squareSize / 2), MARGIN}}
                                };
    POINT textPoints[4] = {{MID_WIDTH - (squareSize / 2) + (textSize / 3), HEIGHT - MARGIN - 5},
                           {((MID_WIDTH) / 4) + (squareSize / 2) + (textSize / 3), MID_HEIGHT + (squareSize / 2) - 5},
                           {MID_WIDTH + ((MID_WIDTH) / 2) - (squareSize / 2) + (textSize / 3), MID_HEIGHT + (squareSize / 2) - 5},
                           {MID_WIDTH - (squareSize / 2) + (textSize / 3), MARGIN + squareSize - 5}
                            };
    fill_screen(BACKGROUND_COLOR);

    for (i = 0; i < 4; i++)
    {
        draw_fill_rectangle(squarePoints[i][0], squarePoints[i][1], black);
        aff_pol(positions[i], textSize, textPoints[i], white);
    }

    affiche_all();
}

int get_border_choice(POINT click/*, POINT squarePoints[4][2]*/)
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
        //print_numbox(pos[0]);
    } while (!is_in_border(pos[0], bor, interface) || !is_on_board(click));

    draw_unicorn(numbox_to_point(pos[0], interface), get_color_by_player(color));
    affiche_all();

    for (i = 1; i != 6; i++)
    {
        do
        {
            click = wait_clic();
            pos[i] = point_to_numbox(click, interface);
            //print_numbox(pos[i]);
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
        affiche_all();
    }
}

Border opposite_border(Border bor)
{
    if (bor == LEFT)   return RIGHT;
    if (bor == RIGHT)  return LEFT;
    if (bor == BOTTOM) return TOP;
    if (bor == TOP)    return BOTTOM;
    return BOTTOM;
}


void draw_unicorn(POINT origin, COULEUR color)
{
    int top_margin = 20;
    int bot_margin = 24;
    int side_margin = 25;

    POINT top = { origin.x + CIRCLE_RADIUS,
                  origin.y + CELL_HEIGHT - top_margin };
    POINT botl = { origin.x + side_margin,
                   origin.y + bot_margin };
    POINT botr = { origin.x + CELL_WIDTH -  side_margin,
                   origin.y + bot_margin };

    draw_fill_triangle(top, botl, botr, color);
}

void draw_paladin(POINT origin, COULEUR color)
{
    int top_margin = 20;
    int bot_margin = 24;
    int side_margin = 25;

    draw_unicorn(origin, color);

    POINT top = { origin.x + CIRCLE_RADIUS,
                  origin.y + CELL_HEIGHT - top_margin };
    POINT botl = { origin.x + side_margin,
                   origin.y + bot_margin + 20 };
    POINT botr = { origin.x + CELL_WIDTH - side_margin,
                   origin.y + bot_margin + 20 };

    draw_fill_triangle(top, botl, botr, BACKGROUND_COLOR);
}

void draw_gameboard(int interface)
{
    int row, col;
    POINT cursor;
    NumBox n;
    fill_screen(BACKGROUND_COLOR);
    POINT p1 = { MARGIN - 3, MARGIN - 3 };
    POINT p2 = { MARGIN + BOARD_WIDTH + 3, MARGIN };

    draw_fill_rectangle(p1, p2, black);

    p2.x = MARGIN; p2.y = MARGIN + BOARD_HEIGHT + 3;
    draw_fill_rectangle(p1, p2, black);


    p1.x = MARGIN + BOARD_WIDTH; p1.y = MARGIN - 3;
    p2.x = MARGIN + BOARD_WIDTH + 3; p2.y = MARGIN + BOARD_WIDTH + 3;
    draw_fill_rectangle(p1, p2, EDGING_COLOR);

    p1.x = MARGIN - 3; p1.y = MARGIN + BOARD_HEIGHT;
    p2.x = MARGIN + BOARD_WIDTH + 3; p2.y = MARGIN + BOARD_HEIGHT + 3;
    draw_fill_rectangle(p1, p2, EDGING_COLOR);

    for (row = 0; row != 6; row++)
    {
        for (col = 0; col != 6; col++)
        {
            n.x = col;
            n.y = row;
            cursor = numbox_to_point(n, interface);

            draw_edging(cursor, gameboard[row][col].edging);
            draw_pawn(gameboard[row][col], cursor);
        }
    }

    affiche_all();
}

void draw_pawn(Box pawn, POINT origin)
{
    COULEUR color;

    if (pawn.color == BLACK) color = BLACK_PLAYER_COLOR;
    else color = WHITE_PLAYER_COLOR;

    if (pawn.type == UNICORN) draw_unicorn(origin, color);
    else if (pawn.type == PALADIN) draw_paladin(origin, color);
}

void draw_edging(POINT bl_corner, int number)
{
    int c, f = 5;
    POINT center;

    center.x = bl_corner.x + CIRCLE_RADIUS;
    center.y = bl_corner.y + CIRCLE_RADIUS;

    for (c = 1; c != number + 1; c++)
    {
        draw_circle(center, CIRCLE_RADIUS - c * f, EDGING_COLOR);
        draw_circle(center, CIRCLE_RADIUS - (c * f) - 1, EDGING_COLOR);
        draw_circle(center, CIRCLE_RADIUS - (c * f) - 2, EDGING_COLOR);
    }
}

void erase_pawn(POINT origin)
{
   draw_unicorn(origin, BACKGROUND_COLOR);
}

int is_on_board(POINT click)
{
    return click.x >= MARGIN && click.x <= (WIDTH - MARGIN) && click.y >= MARGIN && click.y <= (HEIGHT - MARGIN);
}

void erase_highlight(NumBox cell, int interface)
{
    highlight_cell(cell, BACKGROUND_COLOR, interface);
}

void highlight_cell(NumBox cell, COULEUR color, int interface)
{
    int c, f = 5, number;
    POINT p;

    p = numbox_to_point(cell, interface);
    p.x += CIRCLE_RADIUS;
    p.y += CIRCLE_RADIUS;

    number = gameboard[cell.y][cell.x].edging;

    for (c = 1; c != number + 1; c++)
    {
        draw_circle(p, CIRCLE_RADIUS - c * f - 3, color);
        draw_circle(p, CIRCLE_RADIUS - (c * f) - 4, color);
    }
}

void highlight_cells(NumBox *cells, int len, COULEUR color, int interface)
{
    int i, c, f = 5, number;
    POINT p;

    for (i = 0; i != len; i++)
    {
        p = numbox_to_point(cells[i], interface);
        p.x += CIRCLE_RADIUS;
        p.y += CIRCLE_RADIUS;

        number = gameboard[cells[i].y][cells[i].x].edging;

        for (c = 1; c != number + 1; c++)
        {
            draw_circle(p, CIRCLE_RADIUS - c * f - 3, color);
            draw_circle(p, CIRCLE_RADIUS - (c * f) - 4, color);
        }
    }

    affiche_all();
}

void erase_highlighting(NumBox *cells, int len, int interface)
{
    highlight_cells(cells, len, BACKGROUND_COLOR, interface);
    free(cells);
}


void display_endgame_menu(Coul color)
{
    fill_screen(BACKGROUND_COLOR);

    POINT top, bottom, label;
    int size = 30;
    label.x = MARGIN + (size * 2);
    label.y = HEIGHT - size;

    if (color == 0) aff_pol("Le joueur 1 gagne !", size, label, skyblue);
    else aff_pol("Le joueur 2 gagne !", size, label, skyblue);

    top.x = MID_WIDTH;
    top.y = MID_HEIGHT + (MARGIN * 2);
    bottom.x = MID_WIDTH;
    bottom.y = MARGIN / 2;
    draw_line(top, bottom, white);

    size = 25;
    label.x = (MID_WIDTH / 2) / 2 - (size / 2);
    label.y = MID_HEIGHT + 20;
    aff_pol("Rejouer", size, label, white);

    label.x = MID_WIDTH + (MID_WIDTH / 2) / 2 - (size / 2);
    aff_pol("Quitter", size, label, white);

    affiche_all();
}

void erase_information()
{
    POINT start, end;

    start.x = 0;
    start.y = HEIGHT;
    end.x = WIDTH;
    end.y = MARGIN + BOARD_HEIGHT + 6;
    draw_fill_rectangle(start, end, BACKGROUND_COLOR);

    start.y = 0;
    end.x = WIDTH;
    end.y = MARGIN - 5;
    draw_fill_rectangle(start, end, BACKGROUND_COLOR);
}

void display_informations(Coul playerColor, int lastEdging)
{
    erase_information();
    POINT label;
    char* text;
    COULEUR textColor;
    int size = 35;

    label.x = MID_WIDTH - (size * 2);
    label.y = MARGIN + BOARD_HEIGHT + (size * 2);

    if (playerColor == BLACK)
    {
        text = "Joueur 1";
        textColor = BLACK_PLAYER_COLOR;
    }
    else
    {
        text = "Joueur 2";
        textColor = WHITE_PLAYER_COLOR;
    }

    aff_pol(text, size, label, textColor);
    display_turn_helper(textColor, lastEdging);

    affiche_all();
}

void display_turn_helper(COULEUR textColor, int lastEdging)
{
    POINT label;
    char requiredEdging[5];
    int size = 35, radius = 25, i;

    if (lastEdging == 0)
    {
        strcpy(requiredEdging, "1-3");
        lastEdging = 3;
    }
    else
    {
        sprintf(requiredEdging, "%d", lastEdging);
    }

    label.x = MID_WIDTH;
    label.y = MARGIN - size;
    draw_fill_circle(label, radius / 2, textColor);

    for (i = 0; i != lastEdging; i++)
    {
        draw_circle(label, radius - (i * 4), EDGING_COLOR);
        draw_circle(label, radius - (i * 4) - 1, EDGING_COLOR);
    }

    label.x += 30;
    label.y += radius / 2;
    aff_pol(requiredEdging, 20, label, black);
    label.y += 1;
    aff_pol(requiredEdging, 20, label, black);
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

// TODO: some prob with unicorn
void players_place_pawns(Border bor, int interface, Gamemode mode)
{
    NumBox white_pawns[6], black_pawns[6];

    position_pawns(black_pawns, bor, BLACK, interface);
    place_pawns(black_pawns, BLACK);

    if (mode == PVC)
    {
        position_AI_pawns(white_pawns, opposite_border(bor), interface);
        place_pawns(white_pawns, WHITE);
    }
    else
    {
        position_pawns(white_pawns, opposite_border(bor), WHITE, interface);
        place_pawns(white_pawns, WHITE);
    }
}

Border player_choose_border()
{
    Border bor;
    POINT choice;

    do
    {
        choice = wait_clic();
        bor = get_border_choice(choice/*, borderPoints*/);
    } while (bor == -1);

    return bor;
}


/* Doesn't work */
NumBox *pick_pawn_and_move(NumBox *start, NumBox *end, POINT *click1, POINT *click2, int *moves_count, int interface)
{
    NumBox *moves;

    *click1 = wait_clic();
    *start = point_to_numbox(*click1, interface);
    moves = get_moves(moves_count, *start);
    highlight_cells(moves, *moves_count, HIGHLIGHT_COLOR, interface);

    *click2 = wait_clic();
    *end = point_to_numbox(*click2, interface);

    return moves;
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

int replay(POINT click)
{
    if (click.y >= 0 && click.x < MID_WIDTH) return 1;
    else return 0;
}

void set_game_finished(int* finished)
{
     *finished = 1;
}

void AI_game(int interface, NumBox *start, NumBox *end, Coul color, Type *type1, Type *type2, int *lastEdging)
{
    random_move(color, start, end);
    *type1 = gameboard[start->y][start->x].type;
    *type2 = gameboard[end->y][end->x].type;
    erase_pawn(numbox_to_point(*start, interface));
    move_pawn(*start, *end);
    *lastEdging = gameboard[end->y][end->x].edging;
    draw_pawn(gameboard[end->y][end->x], numbox_to_point(*end, interface));
}

int is_cell_valid(POINT click, int lastEdging, int interface)
{
    NumBox selectedCell = point_to_numbox(click, interface);
    return (lastEdging == 0 || lastEdging == gameboard[selectedCell.y][selectedCell.x].edging) && gameboard[selectedCell.y][selectedCell.x].type != EMPTY;
}

/* Helpers */

void print_numbox(NumBox n)
{
    printf("x : %d ; y : %d\n", n.x, n.y);
}

void print_numboxes(NumBox *n, int len)
{
    int i;

    for (i = 0; i != len; i++)
    {
        print_numbox(n[i]);
    }
}


void remove_numboxes(NumBox *n1, int *len1, NumBox *n2, int len2)
{
    int i;

    for (i = 0; i != len2; i++)
    {
        remove_numbox(n1, len1, n2[i]);
    }
}

void remove_numbox(NumBox *ns, int *len, NumBox n)
{
    int i;

    for (i = 0; i != *len; i++)
    {
        if (eql(ns[i], n))
        {
            ns[i--] = ns[--(*len)];
        }
    }
}

void copy(NumBox *n1, int *offset, NumBox *n2, int len2)
{
    int i;

    for (i = 0; i != len2; i++)
    {
        append(n1, offset, n2[i]);
    }
}

void append(NumBox *ns, int *len, NumBox n)
{
    if (contains(ns, *len, n)) return;
    ns[(*len)++] = n;
}

int eql(NumBox n1, NumBox n2)
{
    return n1.x == n2.x && n1.y == n2.y;
}

int contains(NumBox *ns, int len, NumBox n)
{
    int i;

    for (i = 0; i != len; i++)
        if (eql(ns[i], n))
            return 1;

    return 0;
}


void uniq(NumBox *ns, int *len)
{
    int i, j;

    for (i = 0; i != *len; i++)
    {
        for (j = i + 1; j != *len; j++)
        {
            if (eql(ns[i], ns[j])) ns[j--] = ns[--(*len)];
        }
    }
}


/* Escampe project by HAUSER Joshua & REPAIN Paul */
/* IATIC3 2019 */

/* Librairies */

#include "graphics.h"

/* Constants */

#define DEBUG printf("**%d**\n", __LINE__);

#define EDGING_COLOR 0x22211d
//#define EDGING_COLOR red
#define EDGING_WHITE_COLOR magenta
#define HIGHLIGHT_COLOR green
#define BACKGROUND_COLOR 0xBA963E
//#define BACKGROUND_COLOR 0xd2b776
//#define BACKGROUND_COLOR 0x7E7D73
//#define BACKGROUND_COLOR 0x80809F
//#define BACKGROUND_COLOR 0x789D8C

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

typedef enum Type
{
    EMPTY, UNICORN, PALADIN
} Type;

typedef enum Coul
{
    BLACK, WHITE
} Coul;

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
int  is_cell_occupied(NumBox pos);
void init_gamepawns_1();
void init_gamepawns_2();
void move_pawn(NumBox start, NumBox end);
int is_edging_valid(int lastEdging, NumBox start);
int is_any_pawn_playable(Coul color, int* lastEdging);
int  out_of_range(NumBox pos);
void get_neighbours(NumBox *cells, int *offset, NumBox pawn);
NumBox *get_possible_moves(int *len, NumBox pos);
NumBox *get_cells_by_color(Coul color);
int can_override(NumBox start, NumBox end);
void random_move(Coul color, NumBox *start, NumBox *end);

/** View **/

void draw_edging(POINT center, int number);
void draw_gameboard(int interface);
void draw_unicorn(POINT origin, COULEUR color);
void draw_paladin(POINT origin, COULEUR color);
void erase_pawn(POINT origin);
void draw_pawn(Box pawn, POINT origin);
void display_interface_choice();
void display_gamemode_choice();
int  is_on_board(POINT click);
void display_endgame_menu(Coul color);
void erase_information();
void display_informations(Coul playerColor, int lastEdging);
void highlight_cells(NumBox *cells, int len, COULEUR color, int interface);
void erase_highlighting(NumBox *cells, int len, int interface);

/** Controller **/

POINT  numbox_to_point(NumBox n, int interface);
NumBox point_to_numbox(POINT p, int interface);
POINT  numbox_to_point_ig1(NumBox n);
NumBox point_to_numbox_ig1(POINT p);
POINT  numbox_to_point_ig2(NumBox n);
NumBox point_to_numbox_ig2(POINT p);
int  get_interface_choice(POINT click);
int  get_gamemode_choice(POINT click);
int  replay(POINT click);
void set_game_finished(int* finished);
int  is_on_player_side(POINT click, int interface, Coul color);
NumBox *pick_pawn_and_move(NumBox *start, NumBox *end, POINT *click1, POINT *click2, int *moves_count, int interface);

/** Helpers **/

void print_numboxes(NumBox *n, int len);
void print_numbox(NumBox n);
int  eql(NumBox n1, NumBox n2);
void remove_numbox(NumBox *ns, int *len, NumBox n);
void append(NumBox *ns, int *len, NumBox n);
void remove_numboxes(NumBox *n1, int *len1, NumBox *n2, int len2);
void copy(NumBox *n1, int *offset, NumBox *n2, int len2);
int  contains(NumBox *ns, int len, NumBox n);

/* Main */

int main()
{
    NumBox n1, n2, *moves;
    POINT choice, click1, click2;
    int interface, moves_count, gamemode, type1, type2, finished = 0, inGame = 1, lastEdging = 0;
    Coul color;

    init_gameboard();
    init_graphics(WIDTH, HEIGHT);
    affiche_auto_off();

    do
    {
        color = WHITE;
        lastEdging = 0;
        finished = 0;

        display_interface_choice();
        choice = wait_clic();
        interface = get_interface_choice(choice);

        display_gamemode_choice();
        choice = wait_clic();
        gamemode = get_gamemode_choice(choice);

        init_gameboard();
        init_gamepawns_2();
        draw_gameboard(interface);

        do
        {
            display_informations(color, lastEdging);

            do
            {
                click1 = wait_clic();
                n1 = point_to_numbox(click1, interface);
            } while (!is_cell_occupied(n1) || !is_on_player_side(click1, interface, color) || !is_edging_valid(lastEdging, n1));

            type1 = gameboard[n1.y][n1.x].type;
            moves = get_possible_moves(&moves_count, n1);
            highlight_cells(moves, moves_count, HIGHLIGHT_COLOR, interface);

            do
            {
                click2 = wait_clic();
                n2 = point_to_numbox(click2, interface);
            } while (is_on_player_side(click2, interface, color));

            type2 = gameboard[n2.y][n2.x].type;

            erase_pawn(numbox_to_point(n1, interface));
            erase_highlighting(moves, moves_count, interface);
            move_pawn(n1, n2);
            lastEdging = gameboard[n2.y][n2.x].edging;
            draw_pawn(gameboard[n2.y][n2.x], numbox_to_point(n2, interface));

            if (type1 == PALADIN && type2 == UNICORN )
            {
                set_game_finished(&finished);
            }
            else
            {
                if (color == BLACK && is_any_pawn_playable(WHITE, &lastEdging)) color = WHITE;
                else if (is_any_pawn_playable(BLACK, &lastEdging)) color = BLACK;
            }

            affiche_all();
        } while (!finished);

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

int is_any_pawn_playable(Coul color, int* lastEdging)
{
    int i, j;
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            if (gameboard[i][j].color == color && gameboard[i][j].type != EMPTY && gameboard[i][j].edging == *lastEdging) return 1;
        }
    }

    *lastEdging = 0;
    return 0;
}

void get_neighbours(NumBox *cells, int *offset, NumBox pawn)
{
    int i;
    NumBox n[4];

    n[0] = (NumBox) { pawn.x + 1, pawn.y };
    n[1] = (NumBox) { pawn.x - 1, pawn.y };
    n[2] = (NumBox) { pawn.x, pawn.y - 1 };
    n[3] = (NumBox) { pawn.x, pawn.y + 1 };

    for (i = 0; i != 4; i++)
    {
        if (!out_of_range(n[i]))
        {
            if (!is_cell_occupied(n[i]) || can_override(pawn, n[i]))
            {
                cells[(*offset)++] = n[i];
            }
        }
    }
}

void random_move(Coul color, NumBox *start, NumBox *end)
{
    int len;
    NumBox *cells, *ends;

    cells = get_cells_by_color(color);
    *start = cells[alea_int(6)];

    ends = get_possible_moves(&len, *start);
    *end = ends[alea_int(len)];
}

NumBox *get_possible_moves(int *len, NumBox pos)
{
    int moves = gameboard[pos.y][pos.x].edging, i, j;
    int cells_count = 1, neigh_count = 0;
    NumBox neighbours[14], *cells;

    cells = (NumBox *) malloc(sizeof(NumBox) * 14);
    cells[0] = pos;

    for (i = 0; i != moves; i++)
    {
        for (j = 0; j != cells_count; j++)
        {
            get_neighbours(neighbours, &neigh_count, cells[j]);
            remove_numbox(neighbours, &neigh_count, cells[j]);
        }

        cells_count = 0;
        copy(cells, &cells_count, neighbours, neigh_count);
        neigh_count = 0;
    }

    *len = cells_count;

    return cells;
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

void draw_unicorn(POINT origin, COULEUR color)
{
    int top_margin = 18;
    int bot_margin = 20;
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
    int top_margin = 18;
    int bot_margin = 20;
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

    if (pawn.color == BLACK) color = blue;
    else color = white;

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

        //draw_fill_circle(p, CIRCLE_RADIUS / 3, color);
        /*
        draw_circle(p, CIRCLE_RADIUS, color);
        draw_circle(p, CIRCLE_RADIUS - 1, color);
        draw_circle(p, CIRCLE_RADIUS - 2, color);
        draw_circle(p, CIRCLE_RADIUS - 3, color);*/
        for (c = 1; c != number + 1; c++)
        {
            draw_circle(p, CIRCLE_RADIUS - c * f - 3, color);
            draw_circle(p, CIRCLE_RADIUS - (c * f) - 4, color);
            draw_circle(p, CIRCLE_RADIUS - (c * f) - 5, color);
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
    label.x = MARGIN + size;
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
        textColor = blue;
    }
    else
    {
        text = "Joueur 2";
        textColor = white;
    }

    aff_pol(text, size, label, textColor);

    if (lastEdging != 0)
    {
        int radius = 25, i;
        char requiredEdging[1];
        sprintf(requiredEdging, "%d", lastEdging);

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
    }
    affiche_all();
}

/* Controller */

/* Doesn't work */
NumBox *pick_pawn_and_move(NumBox *start, NumBox *end, POINT *click1, POINT *click2, int *moves_count, int interface)
{
    NumBox *moves;

    *click1 = wait_clic();
    *start = point_to_numbox(*click1, interface);
    moves = get_possible_moves(moves_count, *start);
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

int get_interface_choice(POINT click)
{
    if (click.x >= 0 && click.x < MID_WIDTH) return 1;
    else return 2;
}

int get_gamemode_choice(POINT click)
{
    if (click.x >= 0 && click.x < MID_WIDTH) return 1;
    else return 2;
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


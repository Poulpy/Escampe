/* Librairies */

#include "graphics.h"

/* Constants */

#define DEBUG 0

#define EDGING_COLOR red
#define BACKGROUND_COLOR black

#define CELL_WIDTH 65
#define CELL_HEIGHT 65
#define CIRCLE_RADIUS 32

#define MARGIN 30

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

/* Model */

void init_gameboard();
int  is_cell_occupied(NumBox pos);
void init_gamepaws_1();
void init_gamepaws_2();
void move_pawn(NumBox start, NumBox end);
int  possible_moves(NumBox **poss_moves, NumBox pos, int moves);
int  out_of_range(NumBox pos);

/* View */

void draw_edging(POINT center, int number);
void draw_gameboard(int interface);
void draw_unicorn(POINT origin, COULEUR color);
void draw_paladin(POINT origin, COULEUR color);
void erase_pawn(POINT origin);
void draw_pawn(Box pawn, POINT origin);
void display_menu();
int  get_interface_choice(POINT click);
int  is_on_board(POINT click);
void highlight_possible_moves(NumBox pos, int moves, int interface);
void erase_highlighting(NumBox pos, int moves, int interface);
/* Controller */

POINT  numbox_to_point(NumBox n, int interface);
NumBox point_to_numbox(POINT p, int interface);
POINT  numbox_to_point_ig1(NumBox n);
NumBox point_to_numbox_ig1(POINT p);
POINT  numbox_to_point_ig2(NumBox n);
NumBox point_to_numbox_ig2(POINT p);

/* Main */

int main()
{
    NumBox n1, n2;
    POINT choice, click1, click2, p1, p2;
    int interface;


    init_graphics(WIDTH, HEIGHT);
    affiche_auto_off();
    display_menu();

    choice = wait_clic();
    interface = get_interface_choice(choice);
    fill_screen(black);

    init_gameboard();
    init_gamepaws_2();
    draw_gameboard(interface);

    while (1)
    {
        do
        {
            click1 = wait_clic();
            n1 = point_to_numbox(click1, interface);
            highlight_possible_moves(n1, gameboard[n1.y][n1.x].edging, interface);

            click2 = wait_clic();
            n2 = point_to_numbox(click2, interface);


            p1 = numbox_to_point(n1, interface);
            p2 = numbox_to_point(n2, interface);

        } while (!is_cell_occupied(n1) || !is_on_board(click1) || !is_on_board(click2));

        erase_highlighting(n1, gameboard[n1.y][n1.x].edging, interface);
        erase_pawn(p1);
        move_pawn(n1, n2);
        draw_pawn(gameboard[n2.y][n2.x], p2);

        affiche_all();
    }

    wait_escape();

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
            gameboard[i][j].edging = edgings[i][j];
            gameboard[i][j].type = EMPTY;
            gameboard[i][j].color = BLACK;
        }
    }
}

void init_gamepaws_1()
{
    int i;

    NumBox whites[6] = {{4,1}, {4,3}, {4,4}, {5,1}, {5,2}, {5,3}};
    NumBox paladins[10] = {{0,2}, {0,4}, {1,0}, {1,2}, {1,5}, {4,1}, {4,3}, {4,4}, {5,1}, {5,2}};
    NumBox unicorns[2] = {{1,1}, {5,3}};

    for (i = 0; i != 6; i++) gameboard[whites[i].x][whites[i].y].color = WHITE;
    for (i = 0; i != 10; i++) gameboard[paladins[i].x][paladins[i].y].type = PALADIN;
    for (i = 0; i != 2; i++) gameboard[unicorns[i].x][unicorns[i].y].type = UNICORN;
}

void init_gamepaws_2()
{
    int i;

    NumBox whites[6] = {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}};
    NumBox paladins[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {5,0}, {5,1}, {5,2}, {5,4}, {5,5}};
    NumBox unicorns[2] = {{0,0}, {5,3}};

    for (i = 0; i != 6; i++) gameboard[whites[i].x][whites[i].y].color = WHITE;
    for (i = 0; i != 10; i++) gameboard[paladins[i].x][paladins[i].y].type = PALADIN;
    for (i = 0; i != 2; i++) gameboard[unicorns[i].x][unicorns[i].y].type = UNICORN;
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

int possible_moves(NumBox **poss_moves, NumBox pos, int moves)
{
    int i, j = 0, sort = 0, k;

    *poss_moves = (NumBox *) malloc(sizeof(NumBox) * 4 * moves);
    NumBox *buffer_moves = (NumBox *) malloc(sizeof(NumBox) * 4);
    NumBox buffer = { 0, 0 };
/*
    (*poss_moves)[j++] = (NumBox) { pos.x, pos.y + moves };
    (*poss_moves)[j++] = (NumBox) { pos.x, pos.y - moves };
    (*poss_moves)[j++] = (NumBox) { pos.x + moves, pos.y };
    (*poss_moves)[j++] = (NumBox) { pos.x - moves, pos.y };
#if DEBUG
    printf("1) : x : %d y : %d\n", (*poss_moves)[0].x, (*poss_moves)[0].y);
    printf("2) : x : %d y : %d\n", (*poss_moves)[1].x, (*poss_moves)[1].y);
    printf("3) : x : %d y : %d\n", (*poss_moves)[2].x, (*poss_moves)[2].y);
    printf("2) : x : %d y : %d\n", (*poss_moves)[3].x, (*poss_moves)[3].y);
#endif
*/
    for (i = 0; i < moves + 1; i++)
    {
        buffer_moves[0].x = pos.x + i;
        buffer_moves[0].y = pos.y - (moves - i);
        buffer_moves[1].x = pos.x + i;
        buffer_moves[1].y = pos.y + (moves - i);
        buffer_moves[2].x = pos.x - i;
        buffer_moves[2].y = pos.y - (moves - i);
        buffer_moves[3].x = pos.x - i;
        buffer_moves[3].y = pos.y + (moves - i);

        for (k = 0; k != 4; k++)
        {
            if (!out_of_range(buffer_moves[k]))
            {
                if (!is_cell_occupied(buffer_moves[k]))
                {
                    (*poss_moves)[j++] = (NumBox) { buffer_moves[k].x, buffer_moves[k].y };
                }
            }
        }
    }
/*
    for (i = 0; i < j; i++)
    {
        printf("i %d j %d\n", i, j);
        if (out_of_range((*poss_moves)[i])) { puts("out of range"); sort = 1; }
        else if (is_cell_occupied((*poss_moves)[i])) { puts("cell occupied"); sort = 1; }
        else sort = 0;

        if (sort == 1)
        {
            printf("Correct : x : %d y : %d\n", (*poss_moves)[j].x, (*poss_moves)[j].y);
            printf("To move : x : %d y : %d\n", (*poss_moves)[i].x, (*poss_moves)[i].y);
            (*poss_moves)[i].x = (*poss_moves)[j].x;
            (*poss_moves)[i].y = (*poss_moves)[j].y;
            j--;
            printf("Length : %d\n", j);
        }
    }
*/
    return j;
}

int out_of_range(NumBox pos)
{
    return !(pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5);
}


/* View */

void highlight_possible_moves(NumBox pos, int moves, int interface)
{
    NumBox *poss_moves = NULL;
    int i, length = possible_moves(&poss_moves, pos, moves);
    POINT p;

    for (i = 0; i != length; i++)
    {
        p = numbox_to_point(poss_moves[i], interface);
        p.x += CIRCLE_RADIUS;
        p.y += CIRCLE_RADIUS;

        draw_fill_circle(p, 10, orange);
    }

    affiche_all();
}

void erase_highlighting(NumBox pos, int moves, int interface)
{
    NumBox *poss_moves = NULL;
    int i, length = possible_moves(&poss_moves, pos, moves);
    POINT p;

    for (i = 0; i != length; i++)
    {
        p = numbox_to_point(poss_moves[i], interface);
        p.x += CIRCLE_RADIUS;
        p.y += CIRCLE_RADIUS;

        draw_fill_circle(p, 10, BACKGROUND_COLOR);
    }

    affiche_all();
}

void display_menu()
{
    POINT top, bottom, label;

    top.x = MID_WIDTH;
    top.y = HEIGHT - MARGIN;
    bottom.x = MID_WIDTH;
    bottom.y = MARGIN;
    label.x = 50;
    label.y = MID_HEIGHT + 20;

    draw_line(top, bottom, white);

    aff_pol("Interface 1", 20, label, white);

    label.x = MID_WIDTH + 50;

    aff_pol("Interface 2", 20, label, white);

    affiche_all();
}

int get_interface_choice(POINT click)
{
    if (click.x >= 0 && click.x < MID_WIDTH) return 1;
    else return 2;
}

void draw_unicorn(POINT origin, COULEUR color)
{
    int top_margin = 10;
    int bot_margin = 15;
    int side_margin = 20;

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
    int top_margin = 10;
    int bot_margin = 15;
    int side_margin = 20;

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
    int c;
    POINT center;

    center.x = bl_corner.x + CIRCLE_RADIUS;
    center.y = bl_corner.y + CIRCLE_RADIUS;

    for (c = 0; c != number; c++)
    {
        draw_circle(center, CIRCLE_RADIUS - c * 4, EDGING_COLOR);
    }
}

void erase_pawn(POINT origin)
{
   draw_unicorn(origin, black);
}

int is_on_board(POINT click){
    return (click.x >= MARGIN && click.x <= (WIDTH - MARGIN) && click.y >= MARGIN  && click.y <= (HEIGHT - MARGIN));
}

/* Controller */

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

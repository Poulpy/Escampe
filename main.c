/* Librairies */

#include "graphics.h"

/* Constants */

#define EDGING_COLOR red

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

typedef enum TYPE
{
    VIDE, LICORNE, PALADIN
} TYPE;

typedef enum COUL
{
    NOIR, BLANC
} COUL;

/* Structures */

typedef struct NUMBOX
{
    int x;
    int y;
} NUMBOX;

typedef struct BOX
{
    int lisere;/* 1, 2, 3 */
    TYPE typeP;
    COUL coulP;
} BOX;

/* Global variables */

BOX plateau[6][6];

/* Functions prototypePs */

/* Model */

void init_plateau();
int  is_cell_occupied(NUMBOX pos);
void init_piece1_debug();
void init_piece2_debug();
void move_piece(NUMBOX start, NUMBOX end);

/* View */

void draw_lisere(POINT center, int number);
void draw_plateau(int interface);
void affiche_licorne(POINT origin, COULEUR coulP);
void affiche_paladin(POINT origin, COULEUR coulP);
void erase_piece(POINT origin);
void affiche_piece(BOX piece, POINT origin);
void display_menu();
int  get_interface_choice(POINT click);
int is_on_board(POINT click);

/* Controller */

POINT numBox_to_point(NUMBOX n, int interface);
NUMBOX point_to_numBox(POINT p, int interface);
POINT  numBox_to_pointBG_ig1(NUMBOX n);
NUMBOX point_ig1_to_numBox(POINT p);
POINT  numBox_to_pointBG_ig2(NUMBOX n);
NUMBOX point_ig2_to_numBox(POINT p);

/* Main */

int main()
{
    NUMBOX n1, n2;
    POINT choice, click1, click2, p1, p2;
    int interface;


    init_graphics(WIDTH, HEIGHT);
    affiche_auto_off();
    display_menu();

    choice = wait_clic();
    interface = get_interface_choice(choice);
    fill_screen(black);

    init_plateau();
    init_piece2_debug();
    draw_plateau(interface);

    while (1)
    {
        do
        {
            click1 = wait_clic();
            click2 = wait_clic();

            n1 = point_to_numBox(click1, interface);
            n2 = point_to_numBox(click2, interface);


            p1 = numBox_to_point(n1, interface);
            p2 = numBox_to_point(n2, interface);

        } while (!is_cell_occupied(n1) || !is_on_board(click1) || !is_on_board(click2));

        erase_piece(p1);
        move_piece(n1, n2);
        affiche_piece(plateau[n2.y][n2.x], p2);

        affiche_all();
    }

    wait_escape();

    return 0;
}

/* Function definitions */

/* Model */

void init_plateau()
{
    int liseres[6][6] = {{1,2,2,3,1,2},
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
            plateau[i][j].lisere = liseres[i][j];
            plateau[i][j].typeP = VIDE;
            plateau[i][j].coulP = NOIR;
        }
    }
}

void init_piece1_debug()
{
    int i;

    NUMBOX whites[6] = {{4,1}, {4,3}, {4,4}, {5,1}, {5,2}, {5,3}};
    NUMBOX paladins[10] = {{0,2}, {0,4}, {1,0}, {1,2}, {1,5}, {4,1}, {4,3}, {4,4}, {5,1}, {5,2}};
    NUMBOX unicorns[2] = {{1,1}, {5,3}};

    for (i = 0; i != 6; i++) plateau[whites[i].x][whites[i].y].coulP = BLANC;
    for (i = 0; i != 10; i++) plateau[paladins[i].x][paladins[i].y].typeP = PALADIN;
    for (i = 0; i != 2; i++) plateau[unicorns[i].x][unicorns[i].y].typeP = LICORNE;
}

void init_piece2_debug()
{
    int i;

    NUMBOX whites[6] = {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}};
    NUMBOX paladins[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {5,0}, {5,1}, {5,2}, {5,4}, {5,5}};
    NUMBOX unicorns[2] = {{0,0}, {5,3}};

    for (i = 0; i != 6; i++) plateau[whites[i].x][whites[i].y].coulP = BLANC;
    for (i = 0; i != 10; i++) plateau[paladins[i].x][paladins[i].y].typeP = PALADIN;
    for (i = 0; i != 2; i++) plateau[unicorns[i].x][unicorns[i].y].typeP = LICORNE;
}


int is_cell_occupied(NUMBOX pos)
{
    return plateau[pos.y][pos.x].typeP != VIDE;
}

void move_piece(NUMBOX start, NUMBOX end)
{
    plateau[end.y][end.x].typeP = plateau[start.y][start.x].typeP;
    plateau[end.y][end.x].coulP = plateau[start.y][start.x].coulP;
    plateau[start.y][start.x].typeP = VIDE;
}

/* View */

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

void affiche_licorne(POINT origin, COULEUR coulP)
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

    draw_fill_triangle(top, botl, botr, coulP);
}

void affiche_paladin(POINT origin, COULEUR coulP)
{
    int top_margin = 10;
    int bot_margin = 15;
    int side_margin = 20;

    affiche_licorne(origin, coulP);

    POINT top = { origin.x + CIRCLE_RADIUS,
                  origin.y + CELL_HEIGHT - top_margin };
    POINT botl = { origin.x + side_margin,
                   origin.y + bot_margin + 20 };
    POINT botr = { origin.x + CELL_WIDTH - side_margin,
                   origin.y + bot_margin + 20 };

    draw_fill_triangle(top, botl, botr, black);
}

void draw_plateau(int interface)
{
    int row, col;
    POINT cursor;
    NUMBOX n;

    for (row = 0; row != 6; row++)
    {
        for (col = 0; col != 6; col++)
        {
            n.x = col;
            n.y = row;
            cursor = numBox_to_point(n, interface);

            draw_lisere(cursor, plateau[row][col].lisere);
            affiche_piece(plateau[row][col], cursor);
        }
    }

    affiche_all();
}

void affiche_piece(BOX piece, POINT origin)
{
    COULEUR coulP;

    if (piece.coulP == NOIR) coulP = blue;
    else coulP = white;

    if (piece.typeP == LICORNE) affiche_licorne(origin, coulP);
    else if (piece.typeP == PALADIN) affiche_paladin(origin, coulP);
}

void draw_lisere(POINT bl_corner, int number)
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

void erase_piece(POINT origin)
{
   affiche_licorne(origin, black);
}

int is_on_board(POINT click){
    return (click.x >= MARGIN && click.x <= (WIDTH - MARGIN) && click.y >= MARGIN  && click.y <= (HEIGHT - MARGIN));
}

/* Controller */

POINT numBox_to_point(NUMBOX n, int interface)
{
    if (interface == 1) return numBox_to_pointBG_ig1(n);
    else return numBox_to_pointBG_ig2(n);
}

NUMBOX point_to_numBox(POINT p, int interface)
{
    if (interface == 1) return point_ig1_to_numBox(p);
    else return point_ig2_to_numBox(p);
}

POINT numBox_to_pointBG_ig1(NUMBOX n)
{
    POINT p;

    p.x = MARGIN + n.x * CELL_WIDTH;
    p.y = MARGIN + n.y * CELL_HEIGHT;

    return p;
}

NUMBOX point_ig1_to_numBox(POINT p)
{
    NUMBOX n;

    n.x = (p.x - MARGIN) / CELL_WIDTH;
    n.y = (p.y - MARGIN) / CELL_HEIGHT;

    return n;
}

POINT numBox_to_pointBG_ig2(NUMBOX n)
{
    POINT p;

    p.x = MARGIN + (n.y * CELL_WIDTH);
    p.y = MARGIN + BOARD_HEIGHT - CELL_HEIGHT - (n.x * CELL_HEIGHT);

    return p;
}

NUMBOX point_ig2_to_numBox(POINT p)
{
    NUMBOX n;

    n.y = (p.x - MARGIN) / CELL_WIDTH;
    n.x = (MARGIN + BOARD_HEIGHT - p.y) / CELL_HEIGHT;

    return n;
}

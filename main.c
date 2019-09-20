/* Librairies */

#include "graphics.h"

/* Constants */

#define WIDTH BOARD_WIDTH + MARGIN * 2
#define HEIGHT BOARD_HEIGHT + MARGIN * 2

#define MID_WIDTH 3 * CELL_WIDTH + MARGIN
#define MID_HEIGHT 3 * CELL_HEIGHT + MARGIN

#define MARGIN 30
#define BOARD_HEIGHT 6 * CELL_HEIGHT
#define BOARD_WIDTH 6 * CELL_WIDTH


#define CELL_WIDTH 65
#define CELL_HEIGHT 65
#define CIRCLE_RADIUS 32

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

/* View */

void draw_edging(POINT center, int number, COULEUR color);
void draw_gameboard(int interface);
void draw_unicorn(POINT origin, COULEUR color);
void draw_paladin(POINT origin, COULEUR color);
void erase_pawn(POINT origin);
void draw_pawn(Box pawn, POINT origin);
void display_menu();
int  get_interface_choice(POINT click);

/* Controller */

POINT  numbox_to_point_ig1(NumBox n);
NumBox point_to_numbox_ig1(POINT p);
POINT  numbox_to_point_ig2(NumBox n);
NumBox point_to_numbox_ig2(POINT p);
POINT  get_botleft_corner(POINT p, int interface);


/* Main */

int main()
{
    NumBox pawn, move;
    POINT cell, new_cell, p, p2;
    int interface;

    init_gameboard();

    init_graphics(WIDTH, HEIGHT);
    affiche_auto_off();
    display_menu(); 
    affiche_all();

    POINT click = wait_clic();
    interface = get_interface_choice(click);
    fill_screen(black);

    init_gamepaws_1();
    draw_gameboard(interface);
    affiche_all();

    while (1)
    {
        do
        {
            cell = wait_clic();
            new_cell = wait_clic();

            if (interface == 1)
            {
                pawn = point_to_numbox_ig1(cell);
                move = point_to_numbox_ig1(new_cell);
            }
            else
            {
                pawn = point_to_numbox_ig2(cell);
                move = point_to_numbox_ig2(new_cell);
            }

            p = get_botleft_corner(cell, interface);
            p2 = get_botleft_corner(new_cell, interface);

            //printf("move : %d %d\n", move.x, move.y);
            //printf("p : %d %d\n", p.x, p.y);
        } while (is_cell_occupied(move));

        erase_pawn(p);
        move_pawn(pawn, move);
        draw_pawn(gameboard[move.y][move.x], p2);

        // IMPORTANT for the loop to display the paws
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
    int edgings[6][6] = {{1,2,2,3,1,2},
                         {3,1,3,1,3,2},
                         {2,3,1,2,1,3},
                         {2,1,3,2,3,1},
                         {1,3,1,3,1,2},
                         {3,2,2,1,3,2}};

    Coul colors[6][6] = {{BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {BLACK, WHITE, BLACK, WHITE, WHITE, BLACK},
                         {BLACK, WHITE, WHITE, WHITE, BLACK, BLACK}};

    Type types[6][6] = {{EMPTY, EMPTY, PALADIN, EMPTY, PALADIN, EMPTY },
                        {PALADIN, UNICORN, PALADIN, EMPTY, EMPTY, PALADIN },
                        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
                        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
                        {EMPTY, PALADIN, EMPTY, PALADIN, PALADIN, EMPTY },
                        {EMPTY, PALADIN, PALADIN, UNICORN, EMPTY, EMPTY }};
    int i, j;
    for (i = 0; i != 6; i++)
    {
        for (j = 0; j != 6; j++)
        {
            gameboard[i][j].edging = edgings[i][j];
            gameboard[i][j].type = types[i][j];
            gameboard[i][j].color = colors[i][j];
        }
    }
}

void init_gamepaws_2()
{
    int edgings[6][6] = {{1,2,2,3,1,2},
                         {3,1,3,1,3,2},
                         {2,3,1,2,1,3},
                         {2,1,3,2,3,1},
                         {1,3,1,3,1,2},
                         {3,2,2,1,3,2}};

    Coul colors[6][6] = {{WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK},
                         {WHITE, BLACK, BLACK, BLACK, BLACK, BLACK}};

    Type types[6][6] = {{UNICORN, EMPTY, EMPTY, EMPTY, EMPTY, PALADIN },
                        {PALADIN, EMPTY, EMPTY, EMPTY, EMPTY, PALADIN },
                        {PALADIN, EMPTY, EMPTY, EMPTY, EMPTY, PALADIN },
                        {PALADIN, EMPTY, EMPTY, EMPTY, EMPTY, UNICORN },
                        {PALADIN, EMPTY, EMPTY, EMPTY, EMPTY, PALADIN },
                        {PALADIN, EMPTY, EMPTY, EMPTY, EMPTY, PALADIN }};
    int i, j;
    for (i = 0; i != 6; i++)
    {
        for (j = 0; j != 6; j++)
        {
            gameboard[i][j].edging = edgings[i][j];
            gameboard[i][j].type = types[i][j];
            gameboard[i][j].color = colors[i][j];
        }
    }

}


int is_cell_occupied(NumBox pos)
{
    return gameboard[pos.y][pos.x].type != EMPTY;
}

void move_pawn(NumBox start, NumBox end)
{
    Box pawn;

    pawn.type = gameboard[start.y][start.x].type;
    pawn.color = gameboard[start.y][start.x].color;
    
    gameboard[end.y][end.x].type = pawn.type;
    gameboard[end.y][end.x].color = pawn.color;
    gameboard[start.y][start.x].type = EMPTY;    
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

    //printf("WIDTH : %d \ntop : %d %d\n", WIDTH, top.x, top.y);
    
    draw_line(top, bottom, white);

    aff_pol("Interface 1", 20, label, white);
    
    label.x = MID_WIDTH + 50;
    
    aff_pol("Interface 2", 20, label, white);  
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

    draw_fill_triangle(top, botl, botr, black);
}

void draw_gameboard(int interface)
{
    int row, col;
    POINT cursor;

    if (interface == 1) cursor = (POINT) { MARGIN, MARGIN };
    else cursor = (POINT) { MARGIN, HEIGHT - MARGIN - CELL_HEIGHT };

    for (row = 0; row != 6; row++)
    {
        if (interface == 1) cursor.x = MARGIN;
        else cursor.y = HEIGHT - CELL_HEIGHT - MARGIN;

        for (col = 0; col != 6; col++)
        {
            draw_edging(cursor, gameboard[row][col].edging, red);
            draw_pawn(gameboard[row][col], cursor);

            if (interface == 1) cursor.x += CELL_WIDTH;
            else cursor.y -= CELL_HEIGHT;
        }

        if (interface == 1) cursor.y += CELL_HEIGHT;
        else cursor.x += CELL_WIDTH;
    }
}

void draw_pawn(Box pawn, POINT origin)
{
    COULEUR color;

    if (pawn.color == BLACK) color = blue;
    else color = white;

    if (pawn.type == UNICORN) draw_unicorn(origin, color);
    else if (pawn.type == PALADIN) draw_paladin(origin, color);
}

void draw_edging(POINT bl_corner, int number, COULEUR color)
{
    int c;
    POINT center;

    center.x = bl_corner.x + CIRCLE_RADIUS;
    center.y = bl_corner.y + CIRCLE_RADIUS;

    for (c = 0; c != number; c++)
    {
        draw_circle(center, CIRCLE_RADIUS - c * 4, color);
    }
}

void erase_pawn(POINT origin)
{  
   draw_unicorn(origin, black); 
}

/* Controller */

// TODO: manage for the 2 UIs; the x-axis on bottom and the x-axis on top
// with #if UI2 etc.
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

    p.x = MARGIN + (n.y * CELL_WIDTH);//BOARD_HEIGHT + n.x * CELL_HEIGHT;
    p.y = MARGIN + BOARD_HEIGHT - CELL_HEIGHT - (n.x * CELL_HEIGHT);//BOARD_HEIGHT - (n.y + 1) * CELL_HEIGHT;

    return p;
}

NumBox point_to_numbox_ig2(POINT p)
{
    NumBox n;

    n.y = (p.x - MARGIN) / CELL_WIDTH;
    n.x = (MARGIN + BOARD_HEIGHT - p.y) / CELL_HEIGHT;

    return n;
}

POINT get_botleft_corner(POINT p, int interface)
{
    NumBox n;
    POINT p2;

    if (interface == 1) n = point_to_numbox_ig1(p);
    else n = point_to_numbox_ig2(p);

    if (interface == 1) p2 = numbox_to_point_ig1(n);
    else p2 = numbox_to_point_ig2(n);

    return p2;
}


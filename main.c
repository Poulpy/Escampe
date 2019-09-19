/* Librairies */

#include "graphics.h"

/* Constants */

#define WIDTH 600
#define HEIGHT 500

#define MARGIN 10
#define BOARD_HEIGHT HEIGHT - 2 * MARGIN
#define BOARD_WIDTH BOARD_HEIGHT


// TODO: Can't do correct division T.T
#define CELL_WIDTH (BOARD_WIDTH / 6.0)
#define CELL_HEIGHT (BOARD_HEIGHT / 6.0)
#define CIRCLE_RADIUS CELL_WIDTH / 2.0

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

/* TODO: Division doesn't work, so for now we use globals */
int cell_width = 63, cell_height = 63, circle_radius = 30;

Box gameboard[6][6];

/* Functions prototypes */

/* Model */

void init_gameboard();
int is_cell_occupied(NumBox pos);
int is_in_border(NumBox pos, Border bor);
void possible_moves(NumBox pos, NumBox moves[]);
void init_gamepaws_1();
void init_gamepaws_2();
void move_pawn(NumBox start, NumBox end);

/* View */

void highlighting_possible_moves(NumBox pos);
void draw_edging(POINT center, int number, COULEUR color);
void position_paws(NumBox pos[], Border bor);
void cell_click(NumBox pos);
void draw_gameboard(int interface);
void draw_unicorn(POINT origin, COULEUR color);
void draw_paladin(POINT origin, COULEUR color);
void erase_pawn(POINT origin);
void draw_pawn(Box pawn, POINT origin);
void display_menu();
int  get_interface_choice(POINT click);

/* Controller */

POINT numbox_to_point_ig1(NumBox n);
NumBox point_to_numbox_ig1(POINT p);
POINT numbox_to_point_ig2(NumBox n);
NumBox point_to_numbox_ig2(POINT p);
POINT get_botleft_corner(POINT p, int interface);


/* Main */

int main()
{
    Box b;
    NumBox pawn, move;
    POINT click1, click2, p, p2;

    init_gameboard();

    init_graphics(WIDTH, HEIGHT);
    affiche_auto_off();
    display_menu(); 
    affiche_all();
    POINT click = wait_clic();
    int interface = get_interface_choice(click);
    //printf("%d\n", interface);
    fill_screen(black);

    init_gamepaws_1();
    draw_gameboard(interface);
    affiche_all();

    while (1)
    {
        do
        {
            click1 = wait_clic();
            click2 = wait_clic();
            if (interface == 1)
            {
                pawn = point_to_numbox_ig1(click1);
                move = point_to_numbox_ig1(click2);
            }
            else
            {
                pawn = point_to_numbox_ig2(click1);
                move = point_to_numbox_ig2(click2);
            }

            printf("move : %d %d\n", move.x, move.y);
            p = get_botleft_corner(click1, interface);
            p2 = get_botleft_corner(click2, interface);

            printf("p : %d %d\n", p.x, p.y);
    
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

int is_in_border(NumBox pos, Border bor)
{
    switch (bor)
    {
    case TOP:
        return pos.x == 0 || pos.x == 1;
    case BOTTOM:
        return pos.x == 4 || pos.x == 5;
    case LEFT:
        return pos.y == 0 || pos.y == 1;
    case RIGHT:
        return pos.y == 4 || pos.y == 5;
    }

    return 0;
}

void move_pawn(NumBox start, NumBox end){
    Box b;
    b.type = gameboard[start.y][start.x].type;
    b.color = gameboard[start.y][start.x].color;
    
    gameboard[end.y][end.x].type = b.type;
    gameboard[end.y][end.x].color = b.color;
    gameboard[start.y][start.x].type = EMPTY;    
}

/* View */

void display_menu(){
    POINT top, bottom;

    top.x = WIDTH / 2;
    top.y = HEIGHT - MARGIN;
    bottom.x = WIDTH / 2;
    bottom.y = MARGIN;
    
    draw_line(top, bottom, white);

    POINT yoda;
    yoda.x = 100;
    yoda.y = (HEIGHT / 2) + 20;

    aff_pol("Interface 1", 20, yoda, white);
    
    yoda.x = (WIDTH / 2) + 100;
    
    aff_pol("Interface 2", 20, yoda, white);  
}

int get_interface_choice(POINT click){
    if(click.x >= 0 && click.x < (HEIGHT / 2)) return 1;
    else return 2;
}

/* The user chooses where to put his paws on his border */
void position_paws(NumBox paws[], Border bor)
{
/*
    int i;
    for (i = 0; i != 6; i++)
    {
        do
        {
            cell_click(paws[i]);
        } while (!is_in_border(paws[i], bor));
    }*/
}

// TODO: rename margin values
void draw_unicorn(POINT origin, COULEUR color)
{
    int margin = 10;
    int marginy = 15;
    int marginx = 20;
    
    POINT top = { origin.x + circle_radius,
                  origin.y + cell_height - margin };
    POINT botl = { origin.x + marginx,
                   origin.y + marginy };
    POINT botr = { origin.x + cell_width -  marginx,
                   origin.y + marginy };

    draw_fill_triangle(top, botl, botr, color);
}

void draw_paladin(POINT origin, COULEUR color){
    draw_unicorn(origin, color);
    
    int margin = 10;
    int marginy = 15;
    int marginx = 20;

    POINT top = { origin.x + circle_radius,
                  origin.y + cell_height - margin };
    POINT botl = { origin.x + marginx,
                   origin.y + marginy + 20 };
    POINT botr = { origin.x + cell_width - marginx,
                   origin.y + marginy + 20 };

    draw_fill_triangle(top, botl, botr, black);
}

void draw_gameboard(int interface)
{
    int row, col, y = cell_height;
    POINT p;

    if (interface == 1)
    {
        p = (POINT) { MARGIN, MARGIN };
    }
    else
    {
        p = (POINT){ MARGIN, HEIGHT - MARGIN - cell_height };
        //y = -cell_height;
        printf("y : %d\n", y);
        printf("p : %d %d\n", p.x, p.y);
    }

    for (row = 0; row != 6; row++)
    {
        if (interface == 1) p.x = MARGIN;
        else p.y = HEIGHT - cell_height - MARGIN;
        for (col = 0; col != 6; col++)
        {
            draw_edging(p, gameboard[row][col].edging, red);
            draw_pawn(gameboard[row][col], p);

            if (interface == 1) p.x += cell_width;
            else p.y -= cell_height;
            
        }
        if (interface == 1) p.y += cell_height;
        else p.x += cell_width;
        //p.y += y;
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

    center.x = bl_corner.x + circle_radius;
    center.y = bl_corner.y + circle_radius;

    for (c = 0; c != number; c++)
    {
        draw_circle(center, circle_radius - c * 3, color);
    }
}

void erase_pawn(POINT origin){  
   draw_unicorn(origin, black); 
}
/* Controller */

// TODO: manage for the 2 UIs; the x-axis on bottom and the x-axis on top
// with #if UI2 etc.
POINT numbox_to_point_ig1(NumBox n)
{
    POINT p;

    p.x = MARGIN + n.x * cell_width;
    p.y = MARGIN + n.y * cell_height;

    return p;
}

NumBox point_to_numbox_ig1(POINT p)
{
    NumBox n;

    n.x = (p.x - MARGIN) / cell_width;
    n.y = (p.y - MARGIN) / cell_height;

    return n;
}

POINT numbox_to_point_ig2(NumBox n)
{
    POINT p;

    p.x = MARGIN + (n.y * cell_width);//BOARD_HEIGHT + n.x * cell_height;
    p.y = MARGIN + BOARD_HEIGHT - cell_height - (n.x * cell_height);//BOARD_HEIGHT - (n.y + 1) * cell_height;

    return p;
}

NumBox point_to_numbox_ig2(POINT p)
{
    NumBox n;

    n.y = (p.x - MARGIN) / cell_width;
    n.x = (MARGIN + BOARD_HEIGHT - p.y) / cell_height;

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

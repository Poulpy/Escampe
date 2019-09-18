/* Librairies */

#include "graphics.h"

/* Constants */

#define WIDTH 600
#define HEIGHT 400

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
int cell_width = 62, cell_height = 62, circle_radius = 30;

Box gameboard[6][6];

/* Functions prototypes */

int is_cell_ocuppied(NumBox pos);
void highlighting_possible_moves(NumBox pos);
int is_in_border(NumBox pos, Border bor);
void draw_gameboard();
void draw_unicorn(NumBox pos, COULEUR color);
void cell_click(NumBox pos);
void position_paws(NumBox pos[], Border bor);
void init_gamepaws_1();
void init_gamepaws_2();

/* Main */

int main()
{
    int i;
    NumBox unicorn = {0, 0};
    NumBox unicorn2 = {3, 4};
    //int pos[] = {1,1};
    init_graphics(WIDTH, HEIGHT);


    draw_gameboard();
    /*
    cell_click(unicorn);
    draw_unicorn(unicorn, blanc);
    cell_click(unicorn2);
    draw_unicorn(unicorn2, bleu);
    */
    /*
    position_paws(white_pawns, TOP);
    for (i = 0; i != 6; i++)
    {
        draw_unicorn(white_pawns[i], white);
    }
    */
    cell_click(unicorn);
    highlighting_possible_moves(unicorn);

    wait_escape();

    return 0;
}

/* Functions definitions */

/* Model */

void init_gameboard()
{
    gameboard = Box {{{1, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {2, EMPTY, BLACK}},
                {{3, EMPTY, BLACK},// 1
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {2, EMPTY, BLACK}},
                {{2, EMPTY, BLACK},// 2
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK}},
                {{2, EMPTY, BLACK},// 3
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK}},
                {{1, EMPTY, BLACK},// 4
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {2, EMPTY, BLACK}},
                {{3, EMPTY, BLACK},// 5
                {2, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {2, EMPTY, BLACK}}};
}

void init_gamepaws_1()
{
    gameboard = {{{1, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {2, PALADIN, BLACK},
                {3, EMPTY, BLACK},
                {1, PALADIN, BLACK},
                {2, EMPTY, BLACK}},
                {{3, PALADIN, BLACK},// 1
                {1, UNICORN, BLACK},
                {3, PALADIN, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {2, PALADIN, BLACK}},
                {{2, EMPTY, BLACK},// 2
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK}},
                {{2, EMPTY, BLACK},// 3
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK}},
                {{1, EMPTY, BLACK},// 4
                {3, PALADIN, WHITE},
                {1, EMPTY, BLACK},
                {3, PALADIN, WHITE},
                {1, PALADIN, WHITE},
                {2, EMPTY, BLACK}},
                {{3, EMPTY, BLACK},// 5
                {2, PALADIN, WHITE},
                {2, PALADIN, WHITE},
                {1, UNICORN, WHITE},
                {3, EMPTY, BLACK},
                {2, EMPTY, BLACK}}};
}

void init_gamepaws_2()
{
    gameboard = {{{1, UNICORN, WHITE},
                {2, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {2, PALADIN, BLACK}},
                {{3, PALADIN, WHITE},// 1
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {2, PALADIN, BLACK}},
                {{2, PALADIN, WHITE},// 2
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, PALADIN, BLACK}},
                {{2, PALADIN, WHITE},// 3
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, UNICORN, BLACK}},
                {{1, PALADIN, WHITE},// 4
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {2, PALADIN, BLACK}},
                {{3, PALADIN, WHITE},// 5
                {2, EMPTY, BLACK},
                {2, EMPTY, BLACK},
                {1, EMPTY, BLACK},
                {3, EMPTY, BLACK},
                {2, PALADIN, BLACK}}};
}

void possible_moves(NumBox pos, NumBox moves[])
{
    moves[0] = { pos.x - 3, pos.y - 1 }
    moves[1] = { pos.x - 2, pos.y - 2 }
    moves[2] = { pos.x - 1, pos.y - 1 }
    moves[3] = { pos.x, pos.y - 3 }
    moves[4] = { pos.x + 1, pos.y - 2 }
    moves[5] = { pos.x + 2, pos.y - 1 }
    moves[6] = { pos.x + 3, pos.y }
    moves[7] = { pos.x + 2, pos.y + 1 }
    moves[8] = { pos.x + 1, pos.y + 2 }
    moves[9] = { pos.x, pos.y + 3 }
    moves[10] = { pos.x - 1, pos.y + 2 }
    moves[11] = { pos.x - 2, pos.y + 1 }
}

int is_cell_ocuppied(NumBox pos)
{
    return !(pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5 && gameboard[pos.y][pos.x].type != EMPTY);
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


/* View */


void highlighting_possible_moves(NumBox pos)
{
    int moves = gameboard[pos.y][pos.x].edging;
    printf("moves : %d\n", moves);
}


/* The user chooses where to put his paws on his border */
void position_paws(NumBox paws[], Border bor)
{
    int i;
    for (i = 0; i != 6; i++)
    {
        do
        {
            cell_click(paws[i]);
        } while (!is_in_border(paws[i], bor));
    }
}

// TODO: manage for the 2 UIs; the x-axis on bottom and the x-axis on top
void cell_click(NumBox pos)
{
    POINT clic = wait_clic();
    pos.x = (clic.x - MARGIN) / cell_width;
    pos.y = (BOARD_HEIGHT - clic.y) / cell_height;
    //printf("pos : %d, %d\n", pos[0], pos[1]);
}

// TODO: rename margin values
void draw_unicorn(NumBox pos, COULEUR color)
{
    int margin = 2;
    int marginy = 10;
    int marginx = 20;
    POINT top = { MARGIN + circle_radius + pos.x * cell_width,
                  BOARD_HEIGHT - pos.y * cell_height - margin };
    POINT botl = { MARGIN + pos.x * cell_width + marginx,
                   BOARD_HEIGHT - (pos.y + 1) * cell_height + marginy };
    POINT botr = { MARGIN + (pos.x + 1) * cell_width - marginx,
                   BOARD_HEIGHT - (pos.y + 1) * cell_height + marginy };

    draw_fill_triangle(top, botl, botr, color);
}

void draw_gameboard()
{
    int row, col, c;
    POINT p = { MARGIN + circle_radius, BOARD_HEIGHT - circle_radius };

    for (row = 0; row != 6; row++)
    {
        p.x = MARGIN + circle_radius;
        for (col = 0; col != 6; col++)
        {
            for (c = 0; c != gameboard[row][col].edging; c++)
            {
                draw_circle(p, circle_radius - c * 3, red);
            }
            p.x += cell_width;
        }
        p.y -= cell_height;
    }
}


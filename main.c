#include "graphics.h"

#define WIDTH 600
#define HEIGHT 400

#define MARGIN 10
#define BOARD_HEIGHT HEIGHT - 2 * MARGIN
#define BOARD_WIDTH BOARD_HEIGHT

// Can't do correct division T.T
#define CELL_WIDTH (BOARD_WIDTH / 6.0)
#define CELL_HEIGHT (BOARD_HEIGHT / 6.0)

typedef enum Border
{
    TOP, BOTTOM, LEFT, RIGHT
} Border;

typedef struct Box
{
    int edging;/* 1, 2, 3 */
    Type type;
    Coul color;
} Box;

typedef enum Type
{
    EMPTY, UNICORN, PALADIN
} Type;

typedef enum Coul
{
    BLACK, WHITE
} Coul;

typedef struct NumBox
{
    int x;
    int y;
} NumBox;

int cell_width = 62, cell_height = 62, circle_radius = 30;
/*int gameboard[6][6] = {{1,2,2,3,1,2},
                           {3,1,3,1,3,2},
                           {2,3,1,2,1,3},
                           {2,1,3,2,3,1},
                           {1,3,1,3,1,2},
                           {3,2,2,1,3,2}};
*/
Box gameboard[6][6];

int is_cell_ocuppied(NumBox pos);
void highlighting_possible_moves(NumBox pos);
int is_in_border(NumBox pos, Border bor);
void draw_gameboard();
void draw_unicorn(NumBox pos, COULEUR color);
void cell_click(NumBox pos);
void position_paws(NumBox pos[], Border bor);


int main()
{
    int i;
    int unicorn[] = {0, 0};
    int unicorn2[] = {3, 4};
    int white_pawns[6][2];
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

void init_gameboard()
{
    gameboard = {{{1, EMPTY, BLACK},
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





void highlighting_possible_moves(int pos[])
{
    int moves = gameboard[pos[1]][pos[0]];
    printf("moves : %d\n", moves);
}

void possible_moves(NumBox pos, NumBox moves[])
{
    moves[0] = NumBox { pos.x - 3, pos.y - 1 }
    moves[1] = NumBox { pos.x - 2, pos.y - 2 }
    moves[2] = NumBox { pos.x - 1, pos.y - 1 }
    moves[3] = NumBox { pos.x, pos.y - 3 }
    moves[4] = NumBox { pos.x + 1, pos.y - 2 }
    moves[5] = NumBox { pos.x + 2, pos.y - 1 }
    moves[6] = NumBox { pos.x + 3, pos.y }
    moves[7] = NumBox { pos.x + 2, pos.y + 1 }
    moves[8] = NumBox { pos.x + 1, pos.y + 2 }
    moves[9] = NumBox { pos.x, pos.y + 3 }
    moves[10] = NumBox { pos.x - 1, pos.y + 2 }
    moves[11] = NumBox { pos.x - 2, pos.y + 1 }
}

int is_cell_ocuppied(NumBox pos)
{
    return !(pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5 && gameboard[pos.y][pos.x] != EMPTY);
}

void position_paws(NumBox pos[], Border bor)
{
    int i;
    for (i = 0; i != 6; i++)
    {
        do
        {
            cell_click(pos[i]);
        } while (!is_in_border(pos[i], bor));
    }
}

int is_in_border(NumBox pos, Border bor)
{
    switch (bor)
    {
    case TOP:
        return pos[1] == 0 || pos[1] == 1;
    case BOTTOM:
        return pos[1] == 4 || pos[1] == 5;
    case LEFT:
        return pos[0] == 0 || pos[0] == 1;
    case RIGHT:
        return pos[0] == 4 || pos[0] == 5;
    }

    return 0;
}

void cell_click(NumBox pos)
{
    POINT clic = wait_clic();
    pos[0] = (clic.x - MARGIN) / cell_width;
    pos[1] = (BOARD_HEIGHT - clic.y) / cell_height;
    //printf("pos : %d, %d\n", pos[0], pos[1]);
}

void draw_unicorn(NumBox pos, COULEUR color)
{
    int margin = 2;
    int marginy = 10;
    int marginx = 20;
    POINT top = { MARGIN + circle_radius + pos[0] * cell_width,
                  BOARD_HEIGHT - pos[1] * cell_height - margin };
    POINT botl = { MARGIN + pos[0] * cell_width + marginx,
                   BOARD_HEIGHT - (pos[1] + 1) * cell_height + marginy };
    POINT botr = { MARGIN + (pos[0] + 1) * cell_width - marginx,
                   BOARD_HEIGHT - (pos[1] + 1) * cell_height + marginy };

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


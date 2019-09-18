#include "graphics.h"

#define WIDTH 600
#define HEIGHT 400

#define MARGIN 10
#define BOARD_HEIGHT HEIGHT - 2 * MARGIN
#define BOARD_WIDTH BOARD_HEIGHT

// Can't do correct division T.T
#define CELL_WIDTH (BOARD_WIDTH / 6.0)
#define CELL_HEIGHT (BOARD_HEIGHT / 6.0)

int cell_width = 62, cell_height = 62, circle_radius = 30;
void draw_gameboard(int gameboard[6][6]);
void draw_unicorn(int pos[], COULEUR color);


int main()
{
    int gameboard[6][6] = {{1,2,2,3,1,2},
                           {3,1,3,1,3,2},
                           {2,3,1,2,1,3},
                           {2,1,3,2,3,1},
                           {1,3,1,3,1,2},
                           {3,2,2,1,3,2}};
    int unicorn[] = {0, 0};
    int unicorn2[] = {3, 4};
    init_graphics(WIDTH, HEIGHT);


    draw_gameboard(gameboard);
    draw_unicorn(unicorn, blanc);
    draw_unicorn(unicorn2, bleu);

    wait_escape();

    return 0;
}

void draw_unicorn(int pos[], COULEUR color)
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

void draw_gameboard(int gameboard[6][6])
{
    int row, col, c;
    POINT p = { MARGIN + circle_radius, BOARD_HEIGHT - circle_radius };

    for (row = 0; row != 6; row++)
    {
        p.x = MARGIN + circle_radius;
        for (col = 0; col != 6; col++)
        {
            for (c = 0; c != gameboard[row][col]; c++)
            {
                draw_circle(p, circle_radius - c * 3, red);
            }
            p.x += cell_width;
        }
        p.y -= cell_height;
    }
}


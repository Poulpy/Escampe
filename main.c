#include "graphics.h"

#define WIDTH 600
#define HEIGHT 400

#define MARGIN 10
#define BOARD_HEIGHT HEIGHT - 2 * MARGIN
#define BOARD_WIDTH BOARD_HEIGHT

// Can't do correct division T.T
#define CELL_WIDTH (BOARD_WIDTH / 6.0)
#define CELL_HEIGHT (BOARD_HEIGHT / 6.0)

void draw_gameboard(int gameboard[6][6]);


int main()
{
    int gameboard[6][6] = {{1,2,2,3,1,2},
                           {3,1,3,1,3,2},
                           {2,3,1,2,1,3},
                           {2,1,3,2,3,1},
                           {1,3,1,3,1,2},
                           {3,2,2,1,3,2}};
    init_graphics(WIDTH, HEIGHT);

    printf("bh : %d\n", BOARD_HEIGHT);

    draw_gameboard(gameboard);

    wait_escape();

    return 0;
}

void draw_gameboard(int gameboard[6][6])
{
    int row, col, circle_radius = 31, c, cell_width = 63;
    POINT p = { MARGIN + cell_width, BOARD_HEIGHT - cell_width };

    for (row = 0; row != 6; row++)
    {
        p.x = MARGIN + cell_width;
        for (col = 0; col != 6; col++)
        {
            for (c = 0; c != gameboard[row][col]; c++)
            {
                draw_circle(p, circle_radius - c * 3, red);
            }
            p.x += cell_width;
        }
        p.y -= cell_width;
    }
}


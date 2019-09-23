/* Librairies */

#include "graphics.h"

/* Constants */

#define DEBUG printf("**%d**\n", __LINE__);

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
int  get_possible_moves_v2(NumBox pos);
int  get_neighboursv2(NumBox **free, int size, NumBox pawn);

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
void erase_highlighting(NumBox pos, int moves, int interface);
void highlight_cells(NumBox *cells, int len, COULEUR color, int interface);

/* Controller */

POINT  numbox_to_point(NumBox n, int interface);
NumBox point_to_numbox(POINT p, int interface);
POINT  numbox_to_point_ig1(NumBox n);
NumBox point_to_numbox_ig1(POINT p);
POINT  numbox_to_point_ig2(NumBox n);
NumBox point_to_numbox_ig2(POINT p);

/* Tests */

void print_numboxes(NumBox *n, int len);
void print_numbox(NumBox n);
int test_possible_moves();
int test_possible_moves2(NumBox n);
int eql(NumBox n1, NumBox n2);
void remove_numbox(NumBox *ns, int *len, NumBox n);
int test_possible_moves3(NumBox n);

/* Main */

int main()
{
    NumBox n1, n2;//, *free;
    POINT choice, click1, click2, p1, p2;
    int interface;//, length = 0, i;

    //
    //*free = (NumBox *) malloc(sizeof(NumBox) * 4);
    //

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
            //highlight_possible_moves(n1, gameboard[n1.y][n1.x].edging, interface);
            test_possible_moves3(n1);

            //
            /*length = get_possible_moves(&free, n1);
            for (i = 0;i != length; i++)
                printf("%d %d\n", free[i].x, free[i].y);
            */
            //

            click2 = wait_clic();
            n2 = point_to_numbox(click2, interface);


            p1 = numbox_to_point(n1, interface);
            p2 = numbox_to_point(n2, interface);

        } while (!is_cell_occupied(n1) || !is_on_board(click1) || !is_on_board(click2));

        //erase_highlighting(n1, gameboard[n1.y][n1.x].edging, interface);
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

//Appends the neighbours to the array
int get_neighboursv2(NumBox **free, int size, NumBox pawn)
{
    int j = size, i;
    NumBox n[4];

    n[0] = (NumBox) { pawn.x + 1, pawn.y };
    n[1] = (NumBox) { pawn.x - 1, pawn.y };
    n[2] = (NumBox) { pawn.x, pawn.y - 1 };
    n[3] = (NumBox) { pawn.x, pawn.y + 1 };

    for (i = 0; i != 4; i++)
    {
        if (!out_of_range(n[i]))
        {
            if (!is_cell_occupied(n[i]))
            {
                print_numbox(n[i]);
                *free = (NumBox*) realloc((NumBox *) *free, sizeof(NumBox) * ++j);
                (*free)[j - 1] = n[i];
            }
        }
    }

    printf("Len : %d\n", j);
    print_numboxes(*free, j);

    return j;
}

int get_possible_moves_v2(NumBox pos)
{
    int cells = 1, size = 0, moves = gameboard[pos.y][pos.x].edging;
    int i, j;
    NumBox *empt_cells, *neighbours;

    empt_cells = (NumBox *) calloc(1, sizeof(NumBox));
    neighbours = (NumBox *) calloc(1, sizeof(NumBox));
    empt_cells[0] = pos;

    // size : len of neighbours
    // cells : len of empt_cells
    for (i = 0; i != moves; i++)
    {
        for (j = 0; j != cells; j++)
        {
            size = get_neighboursv2(&neighbours, size, (empt_cells)[j]);// invalid read
            /*printf("Size before remove : %d\n", size);
            remove_numbox(&neighbours, &size, (empt_cells)[j]);
            printf("Size after remove : %d\n", size);*/
        }
        if (i + 1 == moves) break;
        free(empt_cells);
        empt_cells = NULL;
        empt_cells = neighbours;
        cells = size;
        size = 0;
        free(neighbours);
        neighbours = NULL;
        neighbours = (NumBox *) malloc(sizeof(NumBox));
    }
    highlight_cells(neighbours, size, orange, 1);
    free(empt_cells);
    free(neighbours);

    return size;
}

void highlight_cells(NumBox *cells, int len, COULEUR color, int interface)
{
    int i;
    POINT p;

    for (i = 0; i != len; i++)
    {
        p = numbox_to_point(cells[i], interface);
        p.x += CIRCLE_RADIUS;
        p.y += CIRCLE_RADIUS;

        draw_fill_circle(p, 10, color);
    }

    affiche_all();
}


int out_of_range(NumBox pos)
{
    return !(pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5);
}


/* View */
void erase_highlighting(NumBox pos, int moves, int interface)
{
    /*
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

    affiche_all();*/
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


void print_numbox(NumBox n)
{
    printf("x : %d ; y : %d\n", n.x, n.y);
}

void print_numboxes(NumBox *n, int len)
{
    int i;
    puts(">");
    for (i = 0; i != len; i++)
        print_numbox(n[i]);
    puts("<");
}
/* Tests */




int test_possible_moves3(NumBox n)
{
    get_possible_moves_v2(n);

    return 0;
}

void remove_numbox(NumBox *ns, int *len, NumBox n)
{
    int i;
    for (i = 0; i != *len; i++)
    {
        if (eql(ns[i], n))
        {
            ns[i].x = ns[*len].x;
            ns[i].y = ns[*len].y;
            *len -= 1;
            return;
        }
    }
}

int eql(NumBox n1, NumBox n2)
{
    return n1.x == n2.x && n1.y == n2.y;
}

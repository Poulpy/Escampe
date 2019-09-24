/* Librairies */

#include "graphics.h"

/* Constants */

#define EDGING_COLOR red

#define CELL_WIDTH 65
#define CELL_HEIGHT 65
#define CIRCLE_RADIUS 32

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

/* Model */

void init_gameboard();
int  is_cell_occupied(NumBox pos);
void init_gamepaws_1();
void init_gamepaws_2();
void move_pawn(NumBox start, NumBox end);
int  out_of_range(NumBox pos);
int is_edging_valid(int lastEdging, NumBox start);
int is_any_pawn_playable(Coul color, int* lastEdging);

/* View */

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

/* Controller */

POINT numbox_to_point(NumBox n, int interface);
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

/* Main */

int main()
{
    NumBox n1, n2;
    POINT choice, click1, click2, p1, p2;
    int interface, gamemode, type1, type2, finished = 0, inGame = 1, lastEdging = 0;
    init_gameboard();
    Coul color;

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
        init_gamepaws_2();
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

            do
            {
                click2 = wait_clic();
                n2 = point_to_numbox(click2, interface);
            } while (is_on_player_side(click2, interface, color));

            type2 = gameboard[n2.y][n2.x].type;

            p1 = numbox_to_point(n1, interface);
            p2 = numbox_to_point(n2, interface);

            erase_pawn(p1);
            move_pawn(n1, n2);
            lastEdging = gameboard[n2.y][n2.x].edging;
            draw_pawn(gameboard[n2.y][n2.x], p2);


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
        fill_screen(black);
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

int out_of_range(NumBox pos)
{
    return !(pos.x >= 0 || pos.x <= 5 || pos.y >= 0 || pos.y <= 5);
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

/* View */

void display_interface_choice()
{
    POINT top, bottom, label;
    int size;

    fill_screen(black);
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

    fill_screen(black);
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
    NumBox n;
    fill_screen(black);

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

int is_on_board(POINT click)
{
    return (click.x >= MARGIN && click.x <= (WIDTH - MARGIN) && click.y >= MARGIN  && click.y <= (HEIGHT - MARGIN));
}

void display_endgame_menu(Coul color)
{
    fill_screen(black);

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
    end.y = MARGIN + BOARD_HEIGHT;
    draw_fill_rectangle(start, end, black);

    start.y = 0;
    end.x = WIDTH;
    end.y = MARGIN - 5;
    draw_fill_rectangle(start, end, black);

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
        char requiredEdging[1];
        sprintf(requiredEdging, "%d", lastEdging);

        label.x = size;
        label.y = MARGIN - size;
        aff_pol("Liseres:", size, label, white);
        label.x = MID_WIDTH;
        aff_pol(requiredEdging, size, label, white);
    }
    affiche_all();
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

#include "view.h"


void display_interface_choice()
{
    POINT top, bottom, label;
    int size;

    fill_screen(BACKGROUND_COLOR);
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

    fill_screen(BACKGROUND_COLOR);
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
void display_border_choice()
{
    char* positions[4] = {"H", "G", "D", "B"};
    int i, squareSize= 75, textSize = 50;
    POINT squarePoints[4][2] = {{ (POINT) {MID_WIDTH - (squareSize / 2), HEIGHT - MARGIN},
                                 (POINT) {MID_WIDTH  + (squareSize / 2), HEIGHT - MARGIN - squareSize }},
                                { (POINT) {(MID_WIDTH) / 4 + (squareSize / 2), MID_HEIGHT + (squareSize / 2)},
                                 (POINT) {(MID_WIDTH) / 4 + (squareSize / 2) + squareSize, MID_HEIGHT - (squareSize / 2)}},
                                { (POINT) {MID_WIDTH + ((MID_WIDTH) / 2) - (squareSize / 2), MID_HEIGHT + (squareSize / 2)},
                                 (POINT) {MID_WIDTH + ((MID_WIDTH) / 2) + (squareSize / 2), MID_HEIGHT - (squareSize / 2)}},
                                { (POINT) {MID_WIDTH - (squareSize / 2), MARGIN + squareSize},
                                 (POINT) {MID_WIDTH + (squareSize / 2), MARGIN}}
                                };
    POINT textPoints[4] = {{MID_WIDTH - (squareSize / 2) + (textSize / 3), HEIGHT - MARGIN - 5},
                           {((MID_WIDTH) / 4) + (squareSize / 2) + (textSize / 3), MID_HEIGHT + (squareSize / 2) - 5},
                           {MID_WIDTH + ((MID_WIDTH) / 2) - (squareSize / 2) + (textSize / 3), MID_HEIGHT + (squareSize / 2) - 5},
                           {MID_WIDTH - (squareSize / 2) + (textSize / 3), MARGIN + squareSize - 5}
                            };
    fill_screen(BACKGROUND_COLOR);

    for (i = 0; i < 4; i++)
    {
        draw_fill_rectangle(squarePoints[i][0], squarePoints[i][1], FIRST_COLOR);
        aff_pol(positions[i], textSize, textPoints[i], THIRD_COLOR);
    }
    draw_tiny_gameboard(0);

    affiche_all();
}

void draw_tiny_gameboard(int interface)
{
    int row, col;
    POINT cursor = { 10, 10 };
    NumBox n;
    fill_screen(BACKGROUND_COLOR);

    for (row = 0; row != 6; row++)
    {
        cursor.x = 0;
        for (col = 0; col != 6; col++)
        {
            n.x = col;
            n.y = row;

            draw_fill_circle(cursor, 3, FOURT_COLOR);
            cursor.x += 10;
        }
        cursor.y += 10;
    }

    affiche_all();
}

void draw_unicorn(POINT origin, COULEUR color)
{
    int top_margin = 20;
    int bot_margin = 24;
    int side_margin = 25;

    POINT top = { origin.x + CIRCLE_RADIUS,
                  origin.y + CELL_HEIGHT - top_margin };
    POINT botl = { origin.x + side_margin,
                   origin.y + bot_margin };
    POINT botr = { origin.x + CELL_WIDTH -  side_margin,
                   origin.y + bot_margin };

    top.x -= 2;
    draw_fill_triangle(top, botl, botr, color);
    top.x += 4;
    draw_fill_triangle(top, botl, botr, color);

    top.x -= 2;
    top.y--;
    draw_fill_circle(top, 2, color);
    draw_fill_ellipse(botl, botr, 2, color);
}


void draw_paladin(POINT origin, COULEUR color)
{
    int top_margin = 30;
    int bot_margin = 24;
    int side_margin = 25;

    POINT top = { origin.x + CIRCLE_RADIUS,
                  origin.y + CELL_HEIGHT - top_margin };
    POINT botl = { origin.x + side_margin,
                   origin.y + bot_margin };
    POINT botr = { origin.x + CELL_WIDTH -  side_margin,
                   origin.y + bot_margin };

    draw_fill_triangle(top, botl, botr, color);
    top.x -= 5;
    draw_fill_triangle(top, botl, botr, color);
    top.x += 10;
    draw_fill_triangle(top, botl, botr, color);

    draw_fill_ellipse(botl, botr, 2, color);
    botl.x = top.x - 9;
    botr.x = top.x - 1;
    botl.y = top.y;
    botr.y = top.y;
    draw_fill_ellipse(botl, botr, 3, color - 0x070707);
}

void draw_gameboard(int interface)
{
    int row, col;
    POINT cursor;
    NumBox n;
    fill_screen(BACKGROUND_COLOR);

    for (row = 0; row != 6; row++)
    {
        for (col = 0; col != 6; col++)
        {
            n.x = col;
            n.y = row;
            cursor = numbox_to_point(n, interface);

            draw_edging(cursor, gameboard[row][col].edging);
            draw_pawn(n, interface);
        }
    }

    affiche_all();
}

void draw_pawn(NumBox pawn, int interface)
{
    COULEUR color;
    Box cell = gameboard[pawn.y][pawn.x];
    POINT origin = numbox_to_point(pawn, interface);

    color = get_color_by_player(cell.color);

    if (cell.type == UNICORN) draw_unicorn(origin, color);
    else if (cell.type == PALADIN) draw_paladin(origin, color);
}

void draw_edging(POINT bl_corner, int number)
{
    int c, f = 5;
    COULEUR colors[] = { FIRST_COLOR, SECON_COLOR, THIRD_COLOR };
    POINT center;

    center.x = bl_corner.x + CIRCLE_RADIUS;
    center.y = bl_corner.y + CIRCLE_RADIUS;

    for (c = 1; c != number + 1; c++)
    {
        draw_fill_circle(center, CIRCLE_RADIUS - c * f, colors[c - 1]);
    }
}

void erase_pawn(NumBox pawn, int interface)
{
    draw_edging(numbox_to_point(pawn, interface), get_edging(pawn));
}



void erase_highlight(NumBox cell, int interface)
{
    highlight_cell(cell, BACKGROUND_COLOR, interface);
}

void highlight_cell(NumBox cell, COULEUR color, int interface)
{
    POINT p;

    p = numbox_to_point(cell, interface);
    p.x += CIRCLE_RADIUS;
    p.y += CIRCLE_RADIUS;

    draw_circle(p, CIRCLE_RADIUS - 3, color);
    draw_circle(p, CIRCLE_RADIUS - 4, color);
}

void highlight_cells(NumBox *cells, int len, COULEUR color, int interface)
{
    int i;

    for (i = 0; i != len; i++)
    {
        highlight_cell(cells[i], color, interface);
    }

    affiche_all();
}

void erase_highlighting(NumBox *cells, int len, int interface)
{
    int i;
    for (i = 0; i != len; i++)
        erase_highlight(cells[i], interface);
    free(cells);
}


void display_endgame_menu(Coul color)
{
    fill_screen(BACKGROUND_COLOR);

    POINT top, bottom, label;
    int size = 30;
    label.x = MARGIN + (size * 2);
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
    end.y = MARGIN + BOARD_HEIGHT + 6;
    draw_fill_rectangle(start, end, BACKGROUND_COLOR);

    start.y = 0;
    end.x = WIDTH;
    end.y = MARGIN - 5;
    draw_fill_rectangle(start, end, BACKGROUND_COLOR);
}

void display_informations(Coul playerColor, int lastEdging)
{
    POINT label;
    char* text;
    COULEUR textColor;
    int size = 35;

    erase_information();
    label.x = MID_WIDTH - (size * 2);
    label.y = MARGIN + BOARD_HEIGHT + (size * 2);

    if (playerColor == BLACK)
    {
        text = "Joueur 1";
        textColor = BLACK_PLAYER_COLOR;
    }
    else
    {
        text = "Joueur 2";
        textColor = WHITE_PLAYER_COLOR;
    }

    aff_pol(text, size, label, textColor);
    display_turn_helper(textColor, lastEdging);

    affiche_all();
}

void display_turn_helper(COULEUR textColor, int lastEdging)
{
    char requiredEdging[5];
    int textSize = 35, i, radii[] = { 100, 90, 80 };
    POINT label, p = { 0, HEIGHT };
    COULEUR colors[] = { FIRST_COLOR, SECON_COLOR, THIRD_COLOR };

    if (lastEdging == 0)
    {
        strcpy(requiredEdging, "1-3");
        lastEdging = 3;
    }
    else
    {
        sprintf(requiredEdging, "%d", lastEdging);
    }

    for (i = 0; i != lastEdging; i++)
    {
        draw_fill_circle(p, radii[i], colors[i]);
    }
    label.x = 10;
    label.y = HEIGHT - 15;
    aff_pol(requiredEdging, textSize, label, BACKGROUND_COLOR);
}

void erase_highlightings(NumBox *moves, NumBox pawn, int moves_count, int interface)
{
    erase_highlight(pawn, interface);
    erase_highlighting(moves, moves_count, interface);
}

 void draw_move(NumBox start, NumBox end, int interface)
{
    erase_pawn(start, interface);
    draw_pawn(end, interface);
}

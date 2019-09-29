#include "view.h"

void display_interface_choice()
{
    POINT p1, p2;
    int size;

    fill_screen(BACKGROUND_COLOR);

    size = 80;
    p1.x = MID_WIDTH - (size * 2.4);
    p1.y = HEIGHT - size;
    aff_pol("Escampe", size, p1, THIRD_COLOR);

    size = 25;
    p1.x = MARGIN / 2;
    p1.y = MID_HEIGHT + 20;
    p2.y = p1.y - 80;
    p2.x = p1.x + 250;

    display_button("Interface 1", p1, p2, FIRST_COLOR, white, 30);

    p1.x = MID_WIDTH;
    p2.y = p1.y - 80;
    p2.x = p1.x + 250;

    display_button("Interface 2", p1, p2, FIRST_COLOR, white, 30);

    affiche_all();
}

void display_button(char *text, POINT bl_corner, POINT ur_corner, COULEUR back,
                    COULEUR front, int textSize)
{
    draw_fill_rectangle(bl_corner, ur_corner, back);
    bl_corner.y += (ur_corner.y - bl_corner.y) * 0.3;
    bl_corner.x += (ur_corner.x - bl_corner.x) * 0.1;
    aff_pol(text, textSize, bl_corner, front);
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
    char* positions[4] = { "H", "G", "D", "B" };
    int i, textSize = 50, padding = 24, buttonWidth = 70;
    POINT textPoints[4] = { { MID_WIDTH - padding, MARGIN + BOARD_HEIGHT + buttonWidth },
                            { MARGIN - buttonWidth * 0.75, MID_HEIGHT + textSize },
                            { MARGIN + BOARD_WIDTH + buttonWidth * 0.25, MID_HEIGHT + textSize },
                            { MID_WIDTH - padding, MARGIN } };
    POINT squarePoints[4][2] = { { { MARGIN, BOARD_HEIGHT + MARGIN },
                                   { MARGIN + BOARD_WIDTH, BOARD_HEIGHT + MARGIN + buttonWidth } },
                                 { { MARGIN - buttonWidth, MARGIN },
                                   { MARGIN, BOARD_HEIGHT + MARGIN } },
                                 { { MARGIN + BOARD_WIDTH, MARGIN },
                                   { MARGIN + BOARD_WIDTH + buttonWidth, MARGIN + BOARD_HEIGHT } },
                                 { { MARGIN, MARGIN - buttonWidth },
                                   { MARGIN + BOARD_WIDTH, MARGIN } } };

    POINT top = { MID_WIDTH - 20 * 8, HEIGHT };
    POINT bottom = { MID_WIDTH - 20 * 8, MARGIN - buttonWidth };
    aff_pol("Choisissez votre bordure", 20, top, white);
    aff_pol("Puis placez vos pions dessus", 20, bottom, white);

    for (i = 0; i < 4; i++)
    {
        draw_fill_rectangle(squarePoints[i][0], squarePoints[i][1], FIRST_COLOR);
        aff_pol(positions[i], textSize, textPoints[i], THIRD_COLOR);
    }

    affiche_all();
}

// Erase everything in the window except the gameboard in the center
void erase_window_except_gameboard()
{
    POINT p = { 0, 0 };
    POINT p2 = { WIDTH, MARGIN };
    POINT p3 = { MARGIN, HEIGHT };
    POINT p4 = { WIDTH, HEIGHT };
    draw_fill_rectangle(p, p2, BACKGROUND_COLOR);
    draw_fill_rectangle(p, p3, BACKGROUND_COLOR);
    p2.x -= MARGIN;
    p2.y -= MARGIN;
    draw_fill_rectangle(p2, p4, BACKGROUND_COLOR);
    p3.y -= MARGIN;
    draw_fill_rectangle(p3, p4, BACKGROUND_COLOR);
}

// Draw a unicorn at the POINT origin
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

// Draw a paladin from the Point origin
// The little shade on the top of the paladin is made
// with the substraction - 0x070707
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

// Draw the gameboard, along with the edgings and the pawns
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

// Draw a pawn on a cell of the gameboard
void draw_pawn(NumBox pawn, int interface)
{
    COULEUR color;
    Box cell = gameboard[pawn.y][pawn.x];
    POINT origin = numbox_to_point(pawn, interface);

    color = get_color_by_player(cell.color);

    if (cell.type == UNICORN) draw_unicorn(origin, color);
    else if (cell.type == PALADIN) draw_paladin(origin, color);
}

// Draw edgings on a cell of the gameboard
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

// Erase a pawn by drawing over an edging; because the color
// of the cell (behind the pawn), depends on the number of
// edgings of the cell
void erase_pawn(NumBox pawn, int interface)
{
    draw_edging(numbox_to_point(pawn, interface), get_edging(pawn));
}

// Another version
/*
void erase_pawn(NumBox pawn, int interface)
{
    int edgings = get_edging(pawn);
    POINT p = numbox_to_point(pawn, interface);
    if (edgings == 1)
        draw_unicorn(p, FIRST_COLOR);
    if (edgings == 2)
        draw_unicorn(p, SECON_COLOR);
    if (edgings == 3)
        draw_unicorn(p, THIRD_COLOR);
}
*/

// Show which player won
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
}

// Show the player's turn (Player 1, Player 2)
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

// Show the number of edgings to play
void display_turn_helper(COULEUR textColor, int lastEdging)
{
    char requiredEdging[5];
    int textSize = 35, radii[] = { 100, 90, 80 }, i;
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
    label.x = 15;
    label.y = HEIGHT - 10;
    aff_pol(requiredEdging, textSize, label, BACKGROUND_COLOR);
}

// Draw a circle surrounding a cell of the gameboard
void highlight_cell(NumBox cell, COULEUR color, int interface, int display)
{
    POINT p;

    p = numbox_to_point(cell, interface);
    p.x += CIRCLE_RADIUS;
    p.y += CIRCLE_RADIUS;

    draw_circle(p, CIRCLE_RADIUS - 3, color);
    draw_circle(p, CIRCLE_RADIUS - 4, color);

    if (display) affiche_all();
}

// Draw circles around cells of the gameboard
// If display is set to TRUE, affiche_all() is called to display the result
void highlight_cells(NumBox *cells, int len, COULEUR color, int interface, int display)
{
    int i;

    for (i = 0; i != len; i++)
    {
        highlight_cell(cells[i], color, interface, 0);
    }

    if (display) affiche_all();
}

// Erase an highlight by highlighting over with the background color
void erase_highlight(NumBox cell, int interface, int display)
{
    highlight_cell(cell, BACKGROUND_COLOR, interface, display);
}

// Erase highlights over multiple cells
void erase_highlights(NumBox *cells, int len, int interface, int display)
{
    int i;
    for (i = 0; i != len; i++)
        erase_highlight(cells[i], interface, 0);

    free(cells);

    if (display) affiche_all();
}

// Erase where the pawn was and draw the pawn to its destination
void draw_move(NumBox start, NumBox end, int interface)
{
    erase_pawn(start, interface);
    draw_pawn(end, interface);
    affiche_all();
}


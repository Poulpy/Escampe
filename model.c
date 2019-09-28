#include "model.h"

// Initialize the gameboard with the edgings and the type (EMPTY)
void init_gameboard()
{
    int edgings[6][6] = { { 1, 2, 2, 3, 1, 2 },
                          { 3, 1, 3, 1, 3, 2 },
                          { 2, 3, 1, 2, 1, 3 },
                          { 2, 1, 3, 2, 3, 1 },
                          { 1, 3, 1, 3, 1, 2 },
                          { 3, 2, 2, 1, 3, 2 } };
    int i, j;
    for (i = 0; i != 6; i++)
    {
        for (j = 0; j != 6; j++)
        {
            gameboard[i][j] = (Box) { edgings[i][j], EMPTY, BLACK };
        }
    }
}

// Fill the gameboard with a pre-arranged set of pawns (white & black)
// Could have done it even smaller with only one array, because the
// number of white pawns, paladins and unicorns are limited
void init_gamepawns_1()
{
    int i;

    NumBox whites[6] = { { 4, 1 }, { 4, 3 }, { 4, 4 },
                         { 5, 1 }, { 5, 2 }, { 5, 3 } };
    NumBox paladins[10] = { { 0, 2 }, { 0, 4 }, { 1, 0 }, { 1, 2 }, { 1, 5 },
                            { 4, 1 }, { 4, 3 }, { 4, 4 }, { 5, 1 }, { 5, 2 } };
    NumBox unicorns[2] = { { 1, 1 }, { 5, 3 } };

    for (i = 0; i != 6; i++)
        gameboard[whites[i].x][whites[i].y].color = WHITE;
    for (i = 0; i != 10; i++)
        gameboard[paladins[i].x][paladins[i].y].type = PALADIN;
    for (i = 0; i != 2; i++)
        gameboard[unicorns[i].x][unicorns[i].y].type = UNICORN;
}

void init_gamepawns_2()
{
    int i;

    NumBox whites[6] = { { 0, 0 }, { 0, 1 }, { 0, 2 },
                         { 0, 3 }, { 0, 4 }, { 0, 5 } };
    NumBox paladins[10] = { { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 },
                            { 5, 0 }, { 5, 1 }, { 5, 2 }, { 5, 4 }, { 5, 5 } };
    NumBox unicorns[2] = { { 0, 0 }, { 5, 3 } };

    for (i = 0; i != 6; i++)
        gameboard[whites[i].x][whites[i].y].color = WHITE;
    for (i = 0; i != 10; i++)
        gameboard[paladins[i].x][paladins[i].y].type = PALADIN;
    for (i = 0; i != 2; i++)
        gameboard[unicorns[i].x][unicorns[i].y].type = UNICORN;
}

// Check if the pawn can crush the pawn at the destination; that is,
// check if the pawn is a paladin and the pawn at the destination is
// a unicorn
int can_override(NumBox start, NumBox end)
{
    return (gameboard[start.y][start.x].type == PALADIN &&
            gameboard[end.y][end.x].type == UNICORN &&
            gameboard[start.y][start.x].color != gameboard[end.y][end.x].color);
}

// Check if the cell is occupied any pawn
int is_cell_occupied(NumBox pos)
{
    return gameboard[pos.y][pos.x].type != EMPTY;
}

// Move a pawn in the gameboard
int move_pawn(NumBox start, NumBox end)
{
    Type enemy = gameboard[end.y][end.x].type;

    gameboard[end.y][end.x].type = gameboard[start.y][start.x].type;
    gameboard[end.y][end.x].color = gameboard[start.y][start.x].color;
    gameboard[start.y][start.x].type = EMPTY;

    // check if the pawn beat the unicorn
    return enemy == UNICORN;
}

int is_edging_valid(int lastEdging, NumBox start)
{
    return lastEdging == 0 || lastEdging == gameboard[start.y][start.x].edging;
}


// Get the 4 surrounding neighbours of a cell
void get_neighbours(NumBox neigh[4], NumBox cell)
{
    neigh[0] = (NumBox) { cell.x + 1, cell.y };
    neigh[1] = (NumBox) { cell.x - 1, cell.y };
    neigh[2] = (NumBox) { cell.x, cell.y - 1 };
    neigh[3] = (NumBox) { cell.x, cell.y + 1 };
}

// Searches all reachable cells for a NumBox pawn;
// All moves are appended to an array of NumBox cells of size moves;
// This function shan't turn back, that's the role of the NumBox forbidden;
// This function checks also if the NumBox pawn (the starting point)
// can capture the unicorn (the end point; that's why we have to check
// if there is no more moves to play)
void depth_first_search(NumBox *cells, int *offset, NumBox pawn, int moves, NumBox forbidden, NumBox player)
{
    int i;
    NumBox neigh[4];

    get_neighbours(neigh, pawn);

    for (i = 0; i != 4; i++)
    {
        if (in_range(neigh[i]) && !eql(neigh[i], forbidden))
        {
            if (!is_cell_occupied(neigh[i]) ||
                (can_override(player, neigh[i]) && 1 == moves))
            {
                // if it is the LAST move, we stop and get the cell
                if (1 == moves)
                    append(cells, offset, neigh[i]);
                else
                    depth_first_search(cells, offset, neigh[i], moves - 1, pawn, player);
            }
        }
    }
}

// Call the function depth_first_search
// The size of the array that stores the possible moves of NumBox pawn
// 1 move => 4 cells max
// 2 move => 8 cells max
// 3 move => 16 cells max
NumBox *get_moves(int *moves_count, NumBox pawn)
{
    int max_size, m = gameboard[pawn.y][pawn.x].edging;
    NumBox forbidden = { -1, -1 };

    if (m == 1)      max_size = 4;
    else if (m == 2) max_size = 8;
    else             max_size = 16;

    NumBox *moves = (NumBox *) malloc(sizeof(NumBox) * max_size);

    *moves_count = 0;

    depth_first_search(moves, moves_count, pawn, m, forbidden, pawn);
    uniq(moves, moves_count);
    moves = (NumBox *) realloc(moves, sizeof(NumBox) * *moves_count);

    return moves;
}

// Random move for a pawn;
// Check all possible moves in an array, then
// get a move with a random number
void random_move(Coul color, NumBox *start, NumBox *end)
{
    int len, go_on = 0;
    NumBox *cells, *ends;

    do
    {
        cells = get_cells_by_color(color);
        *start = cells[alea_int(6)];
        ends = get_moves(&len, *start);
        // must do that because the array is DYNAMICALLY allocated
        if (0 == len)
        {
            go_on = 0;
            free(ends);
        }
    } while (go_on);

    *end = ends[alea_int(len)];

    free(ends);
}

int in_range(NumBox pos)
{
    return pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5;
}

// Get all pawns of a player
NumBox *get_cells_by_color(Coul color)
{
    int cursor = 0, i, j;
    NumBox *cells = (NumBox *) malloc(sizeof(NumBox) * 6);

    for (i = 0; i != 6; i++)
    {
        for (j = 0; j != 6; j++)
        {
            if (gameboard[i][j].type != EMPTY && gameboard[i][j].color == color)
            {
                cells[cursor++] = (NumBox) { j, i };
            }
        }
    }

    return cells;
}

// Check if a NumBox pos is on the border bor (TOP, BOTTOM, etc)
int is_in_border(NumBox pos, Border bor, int interface)
{
    switch (bor)
    {
        case TOP:
            return interface == 1 ? pos.y == 4 || pos.y == 5 : pos.x == 0 || pos.x == 1;
        case BOTTOM:
            return interface == 1 ? pos.y == 0 || pos.y == 1 : pos.x == 4 || pos.x == 5;
        case LEFT:
            return interface == 1 ? pos.x == 0 || pos.x == 1 : pos.y == 0 || pos.y == 1;
        case RIGHT:
            return interface == 1 ? pos.x == 4 || pos.x == 5 : pos.y == 4 || pos.y == 5;
    }

    return 0;
}

// Check if a pawn pawnCell can move
int can_move(NumBox pawnCell)
{
    int moves_count = 0;
    NumBox *moves = get_moves(&moves_count, pawnCell);

    free(moves);

    return 0 != moves_count;
}

// Set the pawns in the gameboard
void set_pawns(NumBox pawns[6], Coul color)
{
    int i;

    gameboard[pawns[0].y][pawns[0].x].type = UNICORN;
    gameboard[pawns[0].y][pawns[0].x].color = color;

    for (i = 1; i != 6; i++)
    {
        gameboard[pawns[i].y][pawns[i].x].type = PALADIN;
        gameboard[pawns[i].y][pawns[i].x].color = color;
    }
}

// Add a NumBox n to an array of NumBox ns of size len if the NumBox doesn't already exist
void append(NumBox *ns, int *len, NumBox n)
{
    if (contains(ns, *len, n)) return;
    ns[(*len)++] = n;
}

// Check if two NumBox n1 and n2 are equal
int eql(NumBox n1, NumBox n2)
{
    return n1.x == n2.x && n1.y == n2.y;
}

// Check if an array of NumBox ns of size len contains a NumBox n
int contains(NumBox *ns, int len, NumBox n)
{
    int i;

    for (i = 0; i != len; i++)
        if (eql(ns[i], n))
            return 1;

    return 0;
}

// Remove duplicate values in a array ns of size len
void uniq(NumBox *ns, int *len)
{
    int i, j;

    for (i = 0; i != *len; i++)
    {
        for (j = i + 1; j != *len; j++)
        {
            if (eql(ns[i], ns[j]))
                ns[j--] = ns[--(*len)];
        }
    }
}

// Substract an array of NumBox n1 by another array of NumBox n2
void remove_numboxes(NumBox *n1, int *len1, NumBox *n2, int len2)
{
    int i;

    for (i = 0; i != len2; i++)
    {
        remove_numbox(n1, len1, n2[i]);
    }
}

// Remove a NumBox n from an array of NumBox ns of size len
void remove_numbox(NumBox *ns, int *len, NumBox n)
{
    int i;

    for (i = 0; i != *len; i++)
    {
        if (eql(ns[i], n))
        {
            ns[i--] = ns[--(*len)];
        }
    }
}

// Copy an array n2 of size len2 to an array n1
void copy(NumBox *n1, int *offset, NumBox *n2, int len2)
{
    int i;

    for (i = 0; i != len2; i++)
    {
        append(n1, offset, n2[i]);
    }
}

// Returns the number of edgings from a NumBox n
int get_edging(NumBox n)
{
    return gameboard[n.y][n.x].edging;
}

// Returns the opposite border of bor
Border get_opposite_border(Border bor)
{
    if (bor == LEFT)   return RIGHT;
    if (bor == RIGHT)  return LEFT;
    if (bor == BOTTOM) return TOP;
    if (bor == TOP)    return BOTTOM;
    return BOTTOM;
}

int can_other_player_move(Coul player, int lastEdging)
{
    int i, j;
    Coul otherPlayer = get_other_player(player);
    NumBox pawnCell;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            pawnCell.x = j;
            pawnCell.y = i;
            if (gameboard[i][j].type != EMPTY &&
                gameboard[i][j].color == otherPlayer &&
                is_edging_valid(lastEdging, pawnCell) &&
                can_move(pawnCell))
                return 1;
        }
    }

    return 0;
}

// Returns the color of the other player
Coul get_other_player(Coul currentPlayer)
{
    return currentPlayer == BLACK ? WHITE : BLACK;
}

// Check if it is the computer/AI turn
int is_AI_turn(Coul currentPlayer, Gamemode mode)
{
    return currentPlayer == AI_COLOR && mode == PVC;
}

// Print a NumBox (debug)
void print_numbox(NumBox n)
{
    printf("x : %d ; y : %d\n", n.x, n.y);
}

// Print an array of NumBox (debug)
void print_numboxes(NumBox *n, int len)
{
    int i;

    for (i = 0; i != len; i++)
    {
        print_numbox(n[i]);
    }
}


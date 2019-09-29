#include "model.h"

// Initialize the plateau with the liseres and the typeP (VIDE)
void init_plateau()
{
    int liseres[6][6] = { { 1, 2, 2, 3, 1, 2 },
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
            plateau[i][j] = (BOX) { liseres[i][j], VIDE, NOIR };
        }
    }
}

// Fill the plateau with a pre-arranged set of pieces (white & black)
// COULd have done it even smaller with only one array, because the
// number of white pieces, paladins and unicorns is limited;
// Black is the default coulP, so no need to assign this coulP, only white
void init_piece1_debug()
{
    int i;

    NUMBOX whites[6] = { { 4, 1 }, { 4, 3 }, { 4, 4 },
                         { 5, 1 }, { 5, 2 }, { 5, 3 } };
    NUMBOX paladins[10] = { { 0, 2 }, { 0, 4 }, { 1, 0 }, { 1, 2 }, { 1, 5 },
                            { 4, 1 }, { 4, 3 }, { 4, 4 }, { 5, 1 }, { 5, 2 } };
    NUMBOX unicorns[2] = { { 1, 1 }, { 5, 3 } };

    for (i = 0; i != 6; i++)
        plateau[whites[i].x][whites[i].y].coulP = BLANC;
    for (i = 0; i != 10; i++)
        plateau[paladins[i].x][paladins[i].y].typeP = PALADIN;
    for (i = 0; i != 2; i++)
        plateau[unicorns[i].x][unicorns[i].y].typeP = LICORNE;
}

// 2nd pre-arranged set of pieces
void init_piece2_debug()
{
    int i;

    NUMBOX whites[6] = { { 0, 0 }, { 0, 1 }, { 0, 2 },
                         { 0, 3 }, { 0, 4 }, { 0, 5 } };
    NUMBOX paladins[10] = { { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 },
                            { 5, 0 }, { 5, 1 }, { 5, 2 }, { 5, 4 }, { 5, 5 } };
    NUMBOX unicorns[2] = { { 0, 0 }, { 5, 3 } };

    for (i = 0; i != 6; i++)
        plateau[whites[i].x][whites[i].y].coulP = BLANC;
    for (i = 0; i != 10; i++)
        plateau[paladins[i].x][paladins[i].y].typeP = PALADIN;
    for (i = 0; i != 2; i++)
        plateau[unicorns[i].x][unicorns[i].y].typeP = LICORNE;
}

// Check if the piece can crush the piece at the destination; that is,
// check if the piece is a paladin and the piece at the destination is
// a unicorn
int can_override(NUMBOX start, NUMBOX end)
{
    return (plateau[start.y][start.x].typeP == PALADIN &&
            plateau[end.y][end.x].typeP == LICORNE &&
            plateau[start.y][start.x].coulP != plateau[end.y][end.x].coulP);
}

// Check if the cell is occupied by any piece
int is_cell_occupied(NUMBOX pos)
{
    return plateau[pos.y][pos.x].typeP != VIDE;
}

// Move a piece in the plateau
// Returns also if the piece beat a unicorn
int move_piece(NUMBOX start, NUMBOX end)
{
    TYPE enemy = plateau[end.y][end.x].typeP;

    plateau[end.y][end.x].typeP = plateau[start.y][start.x].typeP;
    plateau[end.y][end.x].coulP = plateau[start.y][start.x].coulP;
    plateau[start.y][start.x].typeP = VIDE;

    // check if the piece beat the unicorn
    return enemy == LICORNE;
}

// Check if a NUMBOX start has the same number of liseres as the lastEdging
int is_lisere_valid(int lastEdging, NUMBOX start)
{
    return lastEdging == 0 || lastEdging == plateau[start.y][start.x].lisere;
}


// Get the 4 surrounding neighbours of a cell
void get_neighbours(NUMBOX neigh[4], NUMBOX cell)
{
    neigh[0] = (NUMBOX) { cell.x + 1, cell.y };
    neigh[1] = (NUMBOX) { cell.x - 1, cell.y };
    neigh[2] = (NUMBOX) { cell.x, cell.y - 1 };
    neigh[3] = (NUMBOX) { cell.x, cell.y + 1 };
}

// Searches all reachable cells for a NUMBOX piece;
// All moves are appended to an array of NUMBOX cells of size moves;
// This function shan't turn back, that's the role of the NUMBOX forbidden;
// This function checks also if the NUMBOX piece (the starting point)
// can capture the unicorn (the end point; that's why we have to check
// if there is no more moves to play)
void depth_first_search(NUMBOX *cells, int *offset, NUMBOX piece, int moves,
                        NUMBOX forbidden, NUMBOX player)
{
    int i;
    NUMBOX neigh[4];

    get_neighbours(neigh, piece);

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
                    depth_first_search(cells, offset, neigh[i], moves - 1, piece, player);
            }
        }
    }
}

// Call the function depth_first_search
// The size of the array that stores the possible moves of NUMBOX piece
// is dependent of the number of moves allowed
// 1 move => 4 cells max
// 2 move => 8 cells max
// 3 move => 16 cells max
// The array storing the moves is reallocated at the end
NUMBOX *get_moves(int *moves_count, NUMBOX piece)
{
    int max_size, liseres = plateau[piece.y][piece.x].lisere;
    NUMBOX forbidden = { -1, -1 };

    if (liseres == 1)      max_size = 4;
    else if (liseres == 2) max_size = 8;
    else                   max_size = 16;

    NUMBOX *moves = (NUMBOX *) malloc(sizeof(NUMBOX) * max_size);
    *moves_count = 0;

    depth_first_search(moves, moves_count, piece, liseres, forbidden, piece);
    uniq(moves, moves_count);
    moves = (NUMBOX *) realloc(moves, sizeof(NUMBOX) * *moves_count);

    return moves;
}

// Random move for a piece;
// Check all possible moves in an array, then
// get a move with a random number
void random_move(COUL coulP, NUMBOX *start, NUMBOX *end)
{
    int len, go_on = 1;
    NUMBOX *cells, *ends;

    cells = get_cells_by_coulP(coulP);

    do
    {
        *start = cells[alea_int(6)];
        ends = get_moves(&len, *start);
        // must do that in the loop because the array is DYNAMICALLY allocated
        if (0 == len) free(ends);
        else go_on = 0;
    } while (go_on);

    *end = ends[alea_int(len)];

    free(ends);
    free(cells);
}

// Check if the NUMBOX pos is in plateau, 0 <= x & y <= 5
int in_range(NUMBOX pos)
{
    return pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5;
}

// Get all pieces of a player (represented by a coulP COUL)
NUMBOX *get_cells_by_coulP(COUL coulP)
{
    int cursor = 0, i, j;
    NUMBOX *cells = (NUMBOX *) malloc(sizeof(NUMBOX) * 6);

    for (i = 0; i != 6; i++)
    {
        for (j = 0; j != 6; j++)
        {
            if (plateau[i][j].typeP != VIDE && plateau[i][j].coulP == coulP)
            {
                cells[cursor++] = (NUMBOX) { j, i };
            }
        }
    }

    return cells;
}

// Check if a NUMBOX pos is on the border bor (TOP, BOTTOM, etc)
// A border is the first 2 rows
int is_in_border(NUMBOX pos, Border bor, int interface)
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

// Check if a piece pieceCell can move
int can_move(NUMBOX pieceCell)
{
    int moves_count = 0;
    NUMBOX *moves = get_moves(&moves_count, pieceCell);

    free(moves);

    return 0 != moves_count;
}

// Set the pieces of a player in the plateau
void set_pieces(NUMBOX pieces[6], COUL coulP)
{
    int i;

    plateau[pieces[0].y][pieces[0].x].typeP = LICORNE;
    plateau[pieces[0].y][pieces[0].x].coulP = coulP;

    for (i = 1; i != 6; i++)
    {
        plateau[pieces[i].y][pieces[i].x].typeP = PALADIN;
        plateau[pieces[i].y][pieces[i].x].coulP = coulP;
    }
}

// Add a NUMBOX n to an array of NUMBOX ns of size len if the NUMBOX doesn't
// already exist
void append(NUMBOX *ns, int *len, NUMBOX n)
{
    if (contains(ns, *len, n)) return;
    ns[(*len)++] = n;
}

// Check if two NUMBOX n1 and n2 are equal
int eql(NUMBOX n1, NUMBOX n2)
{
    return n1.x == n2.x && n1.y == n2.y;
}

// Check if an array of NUMBOX ns of size len contains a NUMBOX n
int contains(NUMBOX *ns, int len, NUMBOX n)
{
    int i;

    for (i = 0; i != len; i++)
        if (eql(ns[i], n))
            return 1;

    return 0;
}

// Remove duplicate values in a array ns of size len
void uniq(NUMBOX *ns, int *len)
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

// Substract an array of NUMBOX n1 by another array of NUMBOX n2
void remove_numBoxes(NUMBOX *n1, int *len1, NUMBOX *n2, int len2)
{
    int i;

    for (i = 0; i != len2; i++)
    {
        remove_numBox(n1, len1, n2[i]);
    }
}

// Remove a NUMBOX n from an array of NUMBOX ns of size len
// It removes ALL NUMBOX that are equal to n, not just one
void remove_numBox(NUMBOX *ns, int *len, NUMBOX n)
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
// The append starts at offset (not neccessarily the length of n1)
void copy(NUMBOX *n1, int *offset, NUMBOX *n2, int len2)
{
    int i;

    for (i = 0; i != len2; i++)
    {
        append(n1, offset, n2[i]);
    }
}

// Returns the number of liseres of a NUMBOX n
int get_lisere(NUMBOX n)
{
    return plateau[n.y][n.x].lisere;
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

// Check if the other player can move any of his pieces
int can_other_player_move(COUL player, int lastEdging)
{
    int i;
    COUL otherPlayer = get_other_player(player);
    NUMBOX *pieces = get_cells_by_coulP(otherPlayer);

    for (i = 0; i != 6; i++)
    {
        if (is_lisere_valid(lastEdging, pieces[i]) && can_move(pieces[i]))
            return 1;
    }

    free(pieces);

    return 0;
}

// Returns the coulP of the other player
COUL get_other_player(COUL currentPlayer)
{
    return currentPlayer == NOIR ? BLANC : NOIR;
}

// Check if it is the computer/AI turn
int is_AI_turn(COUL currentPlayer, Gamemode mode)
{
    return currentPlayer == AI_COLOR && mode == PVC;
}

// Print a NUMBOX in the terminal (debug)
void print_numBox(NUMBOX n)
{
    printf("x : %d ; y : %d\n", n.x, n.y);
}

// Print an array of NUMBOX in the terminal (debug)
void print_numBoxes(NUMBOX *n, int len)
{
    int i;

    for (i = 0; i != len; i++)
    {
        print_numBox(n[i]);
    }
}


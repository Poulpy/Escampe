#include "model.h"

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
            gameboard[i][j] = (Box) { edgings[i][j], EMPTY, BLACK };
        }
    }
}

void init_gamepawns_1()
{
    int i;

    NumBox whites[6] = {{4,1}, {4,3}, {4,4}, {5,1}, {5,2}, {5,3}};

    NumBox paladins[10] = {{0,2}, {0,4}, {1,0}, {1,2}, {1,5}, {4,1}, {4,3}, {4,4}, {5,1}, {5,2}};
    NumBox unicorns[2] = {{1,1}, {5,3}};

    for (i = 0; i != 6; i++) gameboard[whites[i].x][whites[i].y].color = WHITE;
    for (i = 0; i != 10; i++) gameboard[paladins[i].x][paladins[i].y].type = PALADIN;
    for (i = 0; i != 2; i++) gameboard[unicorns[i].x][unicorns[i].y].type = UNICORN;
}

void init_gamepawns_2()
{
    int i;

    NumBox whites[6] = {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}};
    NumBox paladins[10] = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {5,0}, {5,1}, {5,2}, {5,4}, {5,5}};
    NumBox unicorns[2] = {{0,0}, {5,3}};

    for (i = 0; i != 6; i++) gameboard[whites[i].x][whites[i].y].color = WHITE;
    for (i = 0; i != 10; i++) gameboard[paladins[i].x][paladins[i].y].type = PALADIN;
    for (i = 0; i != 2; i++) gameboard[unicorns[i].x][unicorns[i].y].type = UNICORN;
}

int can_override(NumBox start, NumBox end)
{
    return (gameboard[start.y][start.x].type == PALADIN
            && gameboard[end.y][end.x].type == UNICORN
            && gameboard[start.y][start.x].color != gameboard[end.y][end.x].color);
}

int is_cell_occupied(NumBox pos)
{
    return gameboard[pos.y][pos.x].type != EMPTY;
}

int move_pawn(NumBox start, NumBox end)
{
    Type enemy = gameboard[end.y][end.x].type;
    printf("START: %d %d\n", start.y, start.x);
    printf("END: %d %d\n", end.y, end.x);
    gameboard[end.y][end.x].type = gameboard[start.y][start.x].type;
    gameboard[end.y][end.x].color = gameboard[start.y][start.x].color;
    gameboard[start.y][start.x].type = EMPTY;

    return enemy == UNICORN;
}

int is_edging_valid(int lastEdging, NumBox start)
{
    return (lastEdging == 0 || lastEdging == gameboard[start.y][start.x].edging);
}

int is_unicorn_captured(Type start, Type end)
{
    return start == PALADIN && end == UNICORN;
}

int can_any_pawn_move(Coul color, int* lastEdging)
{
    int i, j;
    NumBox pawnCell;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            pawnCell.x = j;
            pawnCell.y = i;
            if (gameboard[i][j].type != EMPTY && gameboard[i][j].color == color && is_edging_valid(*lastEdging, pawnCell) && can_move(pawnCell)) return 1;
        }
    }

    *lastEdging = 0;
    return 0;
}

void depth_first_search(NumBox *cells, int *offset, NumBox pawn, int moves, NumBox forbidden, NumBox player)
{
    int i;
    NumBox neigh[4];

    neigh[0] = (NumBox) { pawn.x + 1, pawn.y };
    neigh[1] = (NumBox) { pawn.x - 1, pawn.y };
    neigh[2] = (NumBox) { pawn.x, pawn.y - 1 };
    neigh[3] = (NumBox) { pawn.x, pawn.y + 1 };

    for (i = 0; i != 4; i++)
    {
        if (in_range(neigh[i]) && !eql(neigh[i], forbidden))
        {
            if (!is_cell_occupied(neigh[i]) || (can_override(player, neigh[i]) && moves == 1))
            {
                if (moves == 1) append(cells, offset, neigh[i]);
                else depth_first_search(cells, offset, neigh[i], moves - 1, pawn, player);
            }
        }
    }
}

NumBox *get_moves(int *moves_count, NumBox pawn)
{
    int m = gameboard[pawn.y][pawn.x].edging;
    NumBox forbidden = { -1, -1 };

    *moves_count = 0;
    NumBox *moves = (NumBox *) malloc(sizeof(NumBox) * m * 5);

    depth_first_search(moves, moves_count, pawn, m, forbidden, pawn);
    uniq(moves, moves_count);

    return moves;
}

void random_move(Coul color, NumBox *start, NumBox *end)
{
    int len;
    NumBox *cells, *ends;

    cells = get_cells_by_color(color);
    *start = cells[alea_int(6)];

    ends = get_moves(&len, *start);
    *end = ends[alea_int(len)];

    //sleep(2);
}

int in_range(NumBox pos)
{
    return pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5;
}


int out_of_range(NumBox pos)
{
    return !(pos.x >= 0 && pos.x <= 5 && pos.y >= 0 && pos.y <= 5);
}

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

int can_move(NumBox pawnCell)
{
    int moves_count = 0;
    NumBox *moves = get_moves(&moves_count, pawnCell);
    free(moves);
    return moves_count != 0;
}

void place_pawns(NumBox pawns[6], Coul color)
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

void append(NumBox *ns, int *len, NumBox n)
{
    if (contains(ns, *len, n)) return;
    ns[(*len)++] = n;
}

int eql(NumBox n1, NumBox n2)
{
    return n1.x == n2.x && n1.y == n2.y;
}

int contains(NumBox *ns, int len, NumBox n)
{
    int i;

    for (i = 0; i != len; i++)
        if (eql(ns[i], n))
            return 1;

    return 0;
}

void uniq(NumBox *ns, int *len)
{
    int i, j;

    for (i = 0; i != *len; i++)
    {
        for (j = i + 1; j != *len; j++)
        {
            if (eql(ns[i], ns[j])) ns[j--] = ns[--(*len)];
        }
    }
}
void remove_numboxes(NumBox *n1, int *len1, NumBox *n2, int len2)
{
    int i;

    for (i = 0; i != len2; i++)
    {
        remove_numbox(n1, len1, n2[i]);
    }
}

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

void copy(NumBox *n1, int *offset, NumBox *n2, int len2)
{
    int i;

    for (i = 0; i != len2; i++)
    {
        append(n1, offset, n2[i]);
    }
}

void print_numbox(NumBox n)
{
    printf("x : %d ; y : %d\n", n.x, n.y);
}

void print_numboxes(NumBox *n, int len)
{
    int i;

    for (i = 0; i != len; i++)
    {
        print_numbox(n[i]);
    }
}

int get_edging(NumBox n)
{
    return gameboard[n.y][n.x].edging;
}

Border opposite_border(Border bor)
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
            if (gameboard[i][j].type != EMPTY && gameboard[i][j].color == otherPlayer && is_edging_valid(lastEdging, pawnCell) && can_move(pawnCell)) return 1;
        }
    }

    return 0;
}


Coul get_other_player(Coul currentPlayer)
{
    return currentPlayer == BLACK ? WHITE : BLACK;
}

int is_AI_turn(Coul currentPlayer, Gamemode mode)
{
    return currentPlayer == WHITE && mode == PVC;
}

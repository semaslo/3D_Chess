#ifndef MOVEKING_H_INCLUDED
#define MOVEKING_H_INCLUDED
//Contains the logic to check a proposed move of the king is valid
int castle0 = 3;
int castle1 = 3;

int MoveKing(int x, int y, int endx, int endy, bool color)
{
    if ((abs(endx - x) == 1 || endx -x == 0) && (abs(endy - y) == 1 || endy - y == 0) && CheckCheck(color, 6, x, y, endx, endy) == 50)
    {
        Board[endx][endy].piecetype = 6;
        Board[endx][endy].color = color;
        Board[x][y].piecetype = 0;
    }
    //Castling
    else if ((endx - x == 2 || endx - x == -2) && endy == y)
    {
        if (color == 0)
        {
            if (endx == 1 && (castle0 == 1 || castle0 == 3))
            {
                if (Board[2][0].piecetype == 0 && CheckCheck(color, 6, x, y, 2, 0) == 50 && CheckCheck(color, 6, x, y, 1, 0) == 50)
                {
                    castle0 = 0;
                    Board[endx][endy].piecetype = 6;
                    Board[x][y].piecetype = 0;
                    Board[2][0].piecetype = 2;
                    Board[0][0].piecetype = 0;
                    Board[endx][endy].color = 0;
                    Board[2][0].color = 0;
                }
                else
                {
                    return 1;
                }
            }
            else if (endx == 5 && (castle0 == 2 || castle0 == 3))
            {
                if (Board[4][0].piecetype == 0 && CheckCheck(color, 6, x, y, 4,0) == 50 && CheckCheck(color, 6, x, y, 5, 0) == 50)
                {
                    castle0 = 0;
                    Board[endx][endy].piecetype = 6;
                    Board[x][y].piecetype = 0;
                    Board[endx][endy].color = 0;
                    Board[4][0].piecetype = 2;
                    Board[4][0].color = 0;
                    Board[7][0].piecetype = 1;
                }
                else
                {
                    return 1;
                }
            }
        }
        else if (color == 1)
        {
            if (endx == 1 && (castle1 == 1 || castle1 == 3))
            {
                if (Board[2][7].piecetype == 0 && CheckCheck(color, 6, x, y, 2, endy) == 50 && CheckCheck(color, 6, x, y, 1, endy) == 50)
                {
                    castle1 = 0;
                    Board[endx][endy].piecetype = 6;
                    Board[x][y].piecetype = 0;
                    Board[2][7].piecetype = 2;
                    Board[0][7].piecetype = 0;
                    Board[endx][endy].color = 1;
                    Board[2][7].color = 1;
                }
                else
                {
                    return 1;
                }
            }
            else if (endx == 5 && (castle1 == 2 || castle1 == 3))
            {
                if (Board[4][7].piecetype == 0 && CheckCheck(color, 6, x, y, 4,7) == 50 && CheckCheck(color, 6, x, y, 5, 7) == 50)
                {
                    castle1 = 0;
                    Board[endx][endy].piecetype = 6;
                    Board[x][y].piecetype = 0;
                    Board[endx][endy].color = 1;
                    Board[4][7].piecetype = 2;
                    Board[4][7].color = 1;
                    Board[7][7].piecetype = 0;
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        return 1;
    }
    if (color == 0)
    {
        castle0 = 0;
        kingx0 = endx;
        kingy0 = endy;
    }
    else if (color == 1)
    {
        castle1 = 0;
        kingx1 = endx;
        kingy1 = endy;
    }
    return 0;
}

#endif // MOVEKING_H_INCLUDED

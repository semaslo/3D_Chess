#ifndef MOVEBISHOP_H_INCLUDED
#define MOVEBISHOP_H_INCLUDED
//Contains the logical statements to check if a move on a bishop is valid

int MoveBishop(int x, int y, int endx, int endy, bool color)
{
    if(abs(endx-x) == abs(endy-y))
    {
        int deltax, deltay;
        if (endx > x)
            deltax = 1;
        else
            deltax = -1;

        if (endy > y)
            deltay = 1;
        else
            deltay = -1;

        int i = x + deltax;
        int j = y + deltay;
        while (i != endx && j != endy)
        {
            if(Board[i][j].piecetype != 0)
            {
                return 1;
            }
            i += deltax;
            j += deltay;
        }

    }
    else
    {
        return 1;
    }
    if (CheckCheck(color, 4, x, y, endx, endy) != 50)
        return 1;

    Board[x][y].piecetype = 0;
    Board[endx][endy].piecetype = 4;
    Board[endx][endy].color = color;
    return 0;
}

#endif // MOVEBISHOP_H_INCLUDED

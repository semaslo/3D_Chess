#ifndef MOVEQUEEN_H_INCLUDED
#define MOVEQUEEN_H_INCLUDED
//Contains the logic checks to make sure a queen's move is valid

int MoveQueen(int x, int y, int endx, int endy, bool color)
{
    //Same as for Bishop
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
    //Same as for Rook
    else if (x == endx)
    {
        //Checks to see if there are pieces between starting and ending squares
        if (endy - y > 0)
        {
            for(int n = y+1; n < endy; n++)
            {
                if(Board[x][n].piecetype != 0)
                {
                    return 1;
                }
            }
        }
        else
        {
            for(int n = y-1; n > endy; n--)
            {
                if(Board[x][n].piecetype != 0)
                {
                    return 1;
                }
            }
        }
    }
    else if (y == endy)
    {
        if (endx - x > 0)
        {
            for(int n = x+1; n < endx; n++)
            {
                if(Board[n][y].piecetype != 0)
                {
                    return 1;
                }
            }
        }
        else
        {
            for (int n = x-1; n > endx; n--)
            {
                if (Board[n][y].piecetype != 0)
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
    if (CheckCheck(color, 5, x, y, endx, endy) != 50)
        return 1;

    Board[x][y].piecetype = 0;
    Board[endx][endy].piecetype = 5;
    Board[endx][endy].color = color;
    return 0;
}

#endif // MOVEQUEEN_H_INCLUDED

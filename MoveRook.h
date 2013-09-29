#ifndef MOVEROOK_H_INCLUDED
#define MOVEROOK_H_INCLUDED
//Contains the logic statements to make sure a rook's move is valid

int MoveRook(int x, int y, int endx, int endy, bool color)
{
    //No need to check if endy-y != 0 because this has already been disproven
    if (x == endx)
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
    //If it fails these two conditions, the rook cannot move there.
    else
    {
        return 1;
    }

    if (CheckCheck(color, 2, x, y, endx, endy) != 50)
        return 1;
    //If everthing is correct, move piece; if anything is incorrect, the function will return 1

    Board[x][y].piecetype = 0;
    Board[endx][endy].piecetype = 2;
    Board[endx][endy].color = color;
    //changing castle0 and castle1 which tell if a king can castle 0 = cannot castle, 1 = can castle of the left only, 2 can castle on the right only
    if (color == 0)
    {
        if (x == 0 && y == 0)
        {
            if (castle0 == 3)
                castle0 = 2;
            else if (castle0 == 1)
                castle0 = 0;
        }
        else if (x == 7 && y == 0)
        {
            if (castle0 == 3)
                castle0 = 1;
            else if (castle0 == 2)
                castle0 = 0;
        }
    }
    else if (color == 1)
    {
        if (x == 0 && y == 7)
        {
            if (castle1 == 3)
                castle1 = 2;
            else if (castle1 == 1)
                castle1 = 0;
        }
        else if (x == 7 && y == 7)
        {
            if (castle1 == 3)
                castle1= 1;
            else if (castle1 == 2)
                castle1 = 0;
        }
    }

    return 0;
}

#endif // MOVEROOK_H_INCLUDED

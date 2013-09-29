#ifndef CHECK_H_INCLUDED
#define CHECK_H_INCLUDED
//Checks if the king is in Check at the current status of Board

int kingx0 = 3;
int kingy0 = 0;
int kingy1 = 7;
int kingx1 = 3;

Coordinate Check(int kingx, int kingy, int color)
{
    //Checking 4 cardinal directions from king; stops if it hits a piece; If piece is of the same color as the king, moves to the next direction
    Coordinate toreturn;
    //Try replacing this with a while loop - while (Board[i][kingy].piecetype != 0 && i < 8) ... - this would get rid of the multiple break statements
    for(int i = kingx + 1; i < 8; i++)
    {
        if (Board[i][kingy].piecetype != 0)
        {
            if (Board[i][kingy].color == color)
                break;
            else
            {
                if (Board[i][kingy].piecetype == 2 || Board[i][kingy].piecetype == 5)
                {
                    toreturn.x = i;
                    toreturn.y = kingy;
                    return toreturn;
                }
                else if (Board[i][kingy].piecetype == 6 && (i - kingx) == 1)
                {
                    toreturn.x = i;
                    toreturn.y = kingy;
                    return toreturn;
                }
            }
            break;
        }
    }
    for (int i = kingx - 1; i >= 0; i--)
    {
        if (Board[i][kingy].piecetype != 0)
        {
            if (Board[i][kingy].color == color)
                break;
            else
            {
                if (Board[i][kingy].piecetype == 2 || Board[i][kingy].piecetype == 5)
                {
                    toreturn.x = i;
                    toreturn.y = kingy;
                    return toreturn;
                }
                else if (Board[i][kingy].piecetype == 6 && (kingx - i) == 1)
                {
                    toreturn.x = i;
                    toreturn.y = kingy;
                    return toreturn;
                }
            }
            break;
        }
    }
    for (int i = kingy + 1; i < 8; i++)
    {
        if (Board[kingx][i].piecetype != 0)
        {
            if (Board[kingx][i].color == color)
                break;
            else
            {
                if (Board[kingx][i].piecetype == 2 || Board[kingx][i].piecetype == 5)
                {
                    toreturn.x = kingx;
                    toreturn.y = i;
                    return toreturn;
                }
                else if (Board[kingx][i].piecetype == 6 && (i - kingy) == 1)
                {
                    toreturn.x = kingx;
                    toreturn.y = i;
                    return toreturn;
                }
            }
            break;
        }
    }
    for (int i = kingy - 1; i >= 00; i--)
    {
        if (Board[kingx][i].piecetype != 0)
        {
            if (Board[kingx][i].color == color)
                break;
            else
            {
                if (Board[kingx][i].piecetype == 2 || Board[kingx][i].piecetype == 5)
                {
                    toreturn.x = kingx;
                    toreturn.y = i;
                    return toreturn;
                }
                else if (Board[kingx][i].piecetype == 6 && (kingy - i) == 1)
                {
                    toreturn.x = kingx;
                    toreturn.y = i;
                    return toreturn;
                }
            }
            break;
        }
    }
    //Checking the 4 diagonals from king
    //Upper right diagonal
    //for some reason it gave me an error message when I declared i and j within the for loop parentheses and I needed to put and between the two arguments in the middle section
    int i, j;
    for ( i = (kingx + 1), j = (kingy + 1); j < 8 && i < 8; i++, j++)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == color)
                break;
            else
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                {
                    toreturn.x = i;
                    toreturn.y = j;
                    return toreturn;
                }
                else if (j - kingy == 1)
                {
                    //If the color is black, then white pawns one diagonal away can take out the king
                    if (color == 0 && Board[i][j].piecetype == 1)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                    else if (Board[i][j].piecetype == 6)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                }
            }
            break;
        }
    }
    //Lower right diagonal
    for (i = (kingx + 1), j = (kingy - 1); j >= 0 && i < 8; i++, j--)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == color)
                break;
            else
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                {
                    toreturn.x = i;
                    toreturn.y = j;
                    return toreturn;
                }
                else if (kingy - j == 1)
                {
                    //If the color is white, then white pawns one diagonal away can take out the king
                    if (color == 1 && Board[i][j].piecetype == 1)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                    else if (Board[i][j].piecetype == 6)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                }
            }
            break;
        }
    }
    //Lower left diagonal
    for (i = kingx - 1, j = kingy - 1; j >= 0 && i >= 0; i--, j--)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == color)
                break;
            else
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                {
                    toreturn.x = i;
                    toreturn.y = j;
                    return toreturn;
                }
                else if (kingy - j == 1)
                {
                    //If the color is white, then white pawns one diagonal away can take out the king
                    if (color == 1 && Board[i][j].piecetype == 1)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                    else if (Board[i][j].piecetype == 6)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                }
            }
            break;
        }
    }
    //Upper left diagonal
    for (i = kingx - 1, j = kingy + 1; j < 8 && i >= 0; i--, j++)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == color)
                break;
            else
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                {
                    toreturn.x = i;
                    toreturn.y = j;
                    return toreturn;
                }
                else if (kingy - j == 1)
                {
                    //If the color is black, then white pawns one diagonal away can take out the king
                    if (color == 0 && Board[i][j].piecetype == 1)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                    else if (Board[i][j].piecetype == 6)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                }
            }
            break;
        }
    }

    //Checking the spots where a knight could be and hit the king
    if(kingx+1 < 8 && kingy+2 < 8)
    {
        if (Board[kingx + 1][kingy + 2].piecetype == 3 && Board[kingx + 1][kingy + 2].color != color)
        {
            toreturn.x = kingx + 1;
            toreturn.y = kingy + 2;
            return toreturn;
        }
    }
    if(kingx+1 < 8 && kingy -2 >= 0)
    {
        if (Board[kingx + 1][kingy - 2].piecetype == 3 && Board[kingx + 1][kingy - 2].color != color)
        {
            toreturn.x = kingx + 1;
            toreturn.y = kingy - 2;
            return toreturn;
        }
    }
    if(kingx- 1 >=0 && kingy - 2 >= 0)
    {
        if (Board[kingx - 1][kingy - 2].piecetype == 3 && Board[kingx - 1][kingy - 2].color != color)
        {
            toreturn.x = kingx - 1;
            toreturn.y = kingy - 2;
            return toreturn;
        }
    }
    if(kingx - 1 >= 0 && kingy + 2 < 8)
    {
        if (Board[kingx - 1][kingy + 2].piecetype == 3 && Board[kingx - 1][kingy + 2].color != color)
        {
            toreturn.x = kingx - 1;
            toreturn.y = kingy + 2;
            return toreturn;
        }
    }
    if(kingx - 2 >= 0 && kingy + 1 < 8)
    {
        if (Board[kingx - 2][kingy + 1].piecetype == 3 && Board[kingx - 2][kingy + 1].color != color)
        {
            toreturn.x = kingx - 2;
            toreturn.y = kingy + 1;
            return toreturn;
        }
    }
    if(kingx + 2 < 8 && kingy + 1 < 8)
    {
        if (Board[kingx + 2][kingy + 1].piecetype == 3 && Board[kingx + 2][kingy + 1].color != color)
        {
            toreturn.x = kingx + 2;
            toreturn.y = kingy + 1;
            return toreturn;
        }
    }
    if(kingx + 2 < 8 && kingy - 1 >= 0)
    {
        if (Board[kingx + 2][kingy - 1].piecetype == 3 && Board[kingx + 2][kingy - 1].color != color)
        {
            toreturn.x = kingx + 2;
            toreturn.y = kingy - 1;
            return toreturn;
        }
    }
    if(kingx - 2 >= 0 && kingy - 1 >= 0)
    {
        if (Board[kingx - 2][kingy - 1].piecetype == 3 && Board[kingx - 2][kingy - 1].color != color)
        {
            toreturn.x = kingx - 2;
            toreturn.y = kingy - 1;
            return toreturn;
        }
    }

    //If the function doesn't return by the time it gets here, the king is not in check
    toreturn.x = 50;
    toreturn.y = 50;
    return toreturn;
}

#endif // CHECK_H_INCLUDED

#ifndef BLOCKERABILITY_H_INCLUDED
#define BLOCKERABILITY_H_INCLUDED
//Checks to see if a piece can move inbetween the king and the attacking piece; a modified version of Check
//returns 0 if no piece can block, 1 if a piece can block
bool Blockerability(int squarex, int squarey, bool kingcolor)
{
    //Checking 4 cardinal directions from square
    for(int i = squarex + 1; i < 8; i++)
    {
        if (Board[i][squarey].piecetype != 0)
        {
            if (Board[i][squarey].color == kingcolor)
            {
                if (Board[i][squarey].piecetype == 2 || Board[i][squarey].piecetype == 5)
                    return 1;
                break;
            }
            break;
        }
    }
    for (int i = squarex - 1; i >= 0; i--)
    {
        if (Board[i][squarey].piecetype != 0)
        {
            if (Board[i][squarey].color == kingcolor)
            {
                if (Board[i][squarey].piecetype == 2 || Board[i][squarey].piecetype == 5)
                    return 1;
                break;
            }
            break;
        }
    }
    for (int i = squarey + 1; i < 8; i++)
    {
        if (Board[squarex][i].piecetype != 0)
        {
            if (Board[squarex][i].piecetype == kingcolor)
            {
                if (Board[squarex][i].piecetype == 2 || Board[squarex][i].piecetype == 5)
                    return 1;
                //white pawns would be able to move
                else if (Board[squarex][i].piecetype == 1 && (i - squarey) == 1 && kingcolor == 1)
                    return 1;
                //White pawns may also be able to move two from the starting row
                else if (Board[squarex][i].piecetype == 1 && i - squarey == 2 && kingcolor == 1)
                    return 1;
                break;
            }
            break;
        }
    }
    for (int i = squarey - 1; i >= 00; i--)
    {
        if (Board[squarex][i].piecetype != 0)
        {
            if (Board[squarex][i].color == kingcolor)
            {
                if (Board[squarex][i].piecetype == 2 || Board[squarex][i].piecetype == 5)
                    return 1;
                //Black pawns would be able to move
                else if (Board[squarex][i].piecetype == 1 && (squarey - i) == 1 && kingcolor == 0)
                    return 1;
                else if (Board[squarex][i].piecetype == 1 && (squarey - i) == 2 && kingcolor == 0)
                    return 1;
                break;
            }
            break;
        }
    }
    //Checking the 4 diagonals from king
    //Upper right diagonal
    //for some reason it gave me an error message when I declared i and j within the for loop parentheses and I needed to put and between the two arguments in the middle section
    int i, j;
    for ( i = (squarex + 1), j = (squarey + 1); j < 8 && i < 8; i++, j++)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == kingcolor)
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                    return 1;
                break;
            }
            break;
        }
    }
    //Lower right diagonal
    for (i = (squarex + 1), j = (squarey - 1); j >= 0 && i < 8; i++, j--)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == kingcolor)
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                    return 1;
                break;
            }
            break;
        }
    }
    //Lower left diagonal
    for (i = squarex - 1, j = squarey - 1; j >= 0 && i >= 0; i--, j--)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == kingcolor)
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                    return 1;
                break;
            }
            break;
        }
    }
    //Upper left diagonal
    for (i = squarex - 1, j = squarey + 1; j < 8 && i >= 0; i--, j++)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == kingcolor)
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                    return 1;
                break;
            }
            break;
        }
    }

    //Checking the spots where a knight could be and move to the square
    if(squarex+1 < 8)
    {
        if (squarey + 2 < 8)
        {
            if (Board[squarex + 1][squarey + 2].piecetype == 3 && Board[squarex + 1][squarey + 2].color == kingcolor)
                return 1;
        }
        if (squarey - 2 >= 0)
        {
            if (Board[squarex + 1][squarey - 2].piecetype == 3 && Board[squarex + 1][squarey - 2].color == kingcolor)
            return 1;
        }
    }
    if(squarex - 1 >=0)
    {
        if (squarey - 2 >= 0)
        {
            if (Board[squarex - 1][squarey - 2].piecetype == 3 && Board[squarex - 1][squarey - 2].color == kingcolor)
                return 1;
        }
        if (squarey + 2 < 8)
        {
            if (Board[squarex - 1][squarey + 2].piecetype == 3 && Board[squarex - 1][squarey + 2].color == kingcolor)
                return 1;
        }
    }
    if(squarex - 2 >= 0)
    {
        if (squarey + 1 < 8)
        {
            if (Board[squarex - 2][squarey + 1].piecetype == 3 && Board[squarex - 2][squarey + 1].color == kingcolor)
                return 1;
        }
        if (squarey - 1 >= 0)
        {
            if (Board[squarex - 2][squarey - 1].piecetype == 3 && Board[squarex - 2][squarey - 1].color == kingcolor)
                return 1;
        }
    }
    if(squarex + 2 < 8)
    {
        if (squarey + 1)
        {
            if (Board[squarex + 2][squarey + 1].piecetype == 3 && Board[squarex + 2][squarey + 1].color == kingcolor)
                return 1;
        }
        if (squarey - 1 >= 0)
        {
            if (Board[squarex + 2][squarey - 1].piecetype == 3 && Board[squarex + 2][squarey - 1].color == kingcolor)
                return 1;
        }
    }

    //If the function doesn't return by the time it gets here, a piece cannot more to the square
    return 0;
}


#endif // BLOCKERABILITY_H_INCLUDED

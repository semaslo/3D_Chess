#ifndef CHECKCHECK_H_INCLUDED
#define CHECKCHECK_H_INCLUDED
//Checks to if the king would be in check if a piece was moved

int CheckCheck(bool color, int piecetype, int x, int y, int endx, int endy)
{
    //Changes kingx_ if the pioece moving is a king
    //int tempkx, int tempky;
    if (piecetype == 6)
    {
        if (color == 0)
        {
            kingx0 = endx;
            kingy0 = endy;
        }
        else
        {
            kingx1 = endx;
            kingy1 = endy;
        }
    }
    //Change Board temporarily so that it agrees with move
    int temp = Board[endx][endy].piecetype;
    int tempcolor = Board[endx][endy].color;
    Board[endx][endy].piecetype = piecetype;
    Board[endx][endy].color = color;
    Board[x][y].piecetype = 0;
    int n;
    if (color == 0)
        n = Check(kingx0, kingy0, color).x;
    else if (color == 1)
        n = Check(kingx1, kingy1, color).x;
    //Undo temporary changes
    Board[endx][endy].piecetype = temp;
    Board[endx][endy].color = tempcolor;
    Board[x][y].piecetype = piecetype;
    // Undoing changes to kingx0
    if (piecetype == 6)
    {
        if (color == 0)
        {
            kingx0 = x;
            kingy0 = y;
        }
        else
        {
            kingx1 = x;
            kingy1 = y;
        }
    }
    return n;
}

#endif // CHECKCHECK_H_INCLUDED

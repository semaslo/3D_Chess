#ifndef MOVEKNIGHT_H_INCLUDED
#define MOVEKNIGHT_H_INCLUDED
//Contains the logical statements to check to see if a proposed move of a knight is valid

int MoveKnight(int x, int y, int endx, int endy, bool color)
{
    //check to see if move is valid
    if(abs(endx - x) > 2 || abs(endy - y) > 2 || endx == x || endy == y || abs(endx-x) == abs(endy-y))
    {
        return 1;
    }
    else if (CheckCheck(color, 3, x, y, endx, endy) == 50)
    {
        Board[x][y].piecetype = 0;
        Board[endx][endy].piecetype = 3;
        Board[endx][endy].color = color;
    }
    else
        return 1;
    return 0;
}


#endif // MOVEKNIGHT_H_INCLUDED

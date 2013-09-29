#ifndef MOVEPAWN_H_INCLUDED
#define MOVEPAWN_H_INCLUDED
//Includes the logic checks to make sure that the proposed move of a pawn is valid
//It does not move the pawn itself
using namespace std;

int enpassant = 100;
int enpassanttime = 100;

int MovePawn(int x, int y, int endx, int endy, bool color)
{
    //splitting it because numbers for white (red) will be negative
    if (color == 0)
    {
        if(y == 1 && endy-y == 2 && Board[x][y+1].piecetype == 0 && Board[x][endy].piecetype == 0 && endx == x)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                enpassant = endx;
                enpassanttime = 0;
                return 0;
            }
            else
                return 1;
        }
        else if (endy-y == 1 && Board[endx][endy].piecetype == 0 && endx == x)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                return 0;
            }
            else
                return 1;
        }
        //Checking to see if it can take another piece out en passant
        else if (enpassanttime == 1 && enpassant == endx && endy == 5)
        {
            cout << enpassant << endl;
            if (abs(endx-x) == 1 && endy-y == 1)
            {
                if (CheckCheck(color, 1, x, y, endx, endy) == 50)
                {
                    Board[endx][y].piecetype = 0;
                    Board[endx][endy].piecetype = 1;
                    Board[endx][endy].color = color;
                    Board[x][y].piecetype = 0;
                    return 0;
                }
                else
                    return 1;
            }
            else
            {
                return 1;
            }
        }
        //Checking to see it is moving diagonally 1 and hitting another piece
        else if (abs(endx-x) == 1 && endy-y == 1 && Board[endx][endy].piecetype != 0 && Board[endx][endy].color != color)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                return 0;
            }
            else
                return 1;
        }

        else
        {
            return 1;
        }
    }
    else if (color == 1)
    {
        if (y == 6 && y-endy == 2 && Board[x][y-1].piecetype == 0 && Board[x][endy].piecetype == 0 && endx == x)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                enpassant = endx;
                enpassanttime = 0;
                return 0;
            }
            else
                return 1;
        }
        else if (endy - y == -1 && Board[endx][endy].piecetype == 0 && endx == x)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                return 0;
            }
            else
                return 1;
        }
        else if (abs(endx-x) == 1 && y-endy == 1 && Board[endx][endy].piecetype != 0 && Board[endx][endy].color != color)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                return 0;
            }
            else
                return 1;
        }
        //pawn capture en passant
        else if (enpassanttime == 1 && abs(endx-x) == 1 && endy - y == -1)
        {
            cout << enpassant << endl;
            if (enpassant == endx && endy == 2)
            {
                if (CheckCheck(color, 1, x, y, endx, endy) == 50)
                {
                    Board[endx][y].piecetype = 0;
                    Board[endx][endy].piecetype = 1;
                    Board[endx][endy].color = color;
                    Board[x][y].piecetype = 0;
                    return 0;
                }
                else
                    return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }


    }
    //Pawn Exchange
    if ((color == 1 && endy == 0) || ((color == 0 && endy == 7)))
    {
        cout << "Which piece would you like to exchange your pawn for?"  << endl;
        cout << "2: rook" << endl << "3: knight" << endl << "4: bishop" << endl << "5: queen" << endl;
        int choice;
        cin >> choice;
        Board[endx][endy].piecetype = choice;
    }

    return 0;
}

#endif // MOVEPAWN_H_INCLUDED

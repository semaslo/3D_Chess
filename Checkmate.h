#ifndef CHECKMATE_H_INCLUDED
#define CHECKMATE_H_INCLUDED

//Checks to see if the king is in Checkmate; return 0 if not in checkmate, 1 if in checkmate
bool Checkmate(Coordinate attacker, bool kingcolor)
{
    //Determining what the other color is
    bool attackercolor;
    if (kingcolor == 0)
        attackercolor = 1;
    else
        attackercolor = 0;

    //Determining where the king is
    Coordinate kingcoords;
    if (kingcolor == 0)
    {
        kingcoords.x = kingx0;
        kingcoords.y = kingy0;
    }
    else
    {
        kingcoords.x = kingy1;
        kingcoords.y = kingy1;
    }

    //Checking to see if the attacking piece can be taken out
    //Also checks to make sure that piece that can take out the attacker is not putting the king into check
    Coordinate takerouter;
    takerouter = Check(attacker.x, attacker.y, attackercolor);

    if (takerouter.x != 50)
    {
        if (CheckCheck(kingcolor, Board[takerouter.x][takerouter.y].piecetype, takerouter.x, takerouter.y, attacker.x, attacker.y) == 50)
            return 0;
    }

    //Checking to see if a piece can block the king from the attacking piece
    //Must make sure that the attacking piece is not a knight
    if (Board[attacker.x][attacker.y].piecetype != 3)
    {
        //Determines what direction the attacking piece is from the king for the for-loop
        int a, b;
        if (attacker.x > kingcoords.x)
            a = 1;
        else if (attacker.x == kingcoords.x)
            a = 0;
        else
            a = -1;

        if (attacker.y > kingcoords.y)
            b = 1;
        else if (attacker.y == kingcoords.y)
            b = 0;
        else
            b = -1;
        cout << endl << a << " " << b << endl;
        cout << attacker.x << " " << attacker.y << endl;
        for (int i = kingcoords.x + a, j = kingcoords.y + b; i != attacker.x && j != attacker.y; i += a, j += b)
        {
            cout << i << " " << j << endl;
            int a = Blockerability(i, j, kingcolor);
            cout << "Blockerability " << a << endl;
            if (a == 1)
                return 0;
            /*if (Blockerability(i, j, kingcolor) == 1)
                return 0;*/
        }
    }

    //Checking to see if the king can move to escape
    if (kingcoords.x + 1 < 8)
    {
        if (Board[kingcoords.x + 1][kingcoords.y].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x + 1, kingcoords.y) == 50)
            return 0;

        if (Board[kingcoords.x + 1][kingcoords.y + 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x + 1, kingcoords.y+1) == 50)
        if (kingcoords.y + 1 < 8)
        {
            if (Board[kingcoords.x + 1][kingcoords.y + 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x + 1, kingcoords.y+1) == 50)
                return 0;

            if (Board[kingcoords.x][kingcoords.y + 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x, kingcoords.y+1) == 50)
                return 0;
        }
        if (kingcoords.y  - 1 >= 0)
        {
            if (Board[kingcoords.x + 1][kingcoords.y - 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x + 1, kingcoords.y-1) == 50)
                return 0;
            if (Board[kingcoords.x][kingcoords.y - 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x, kingcoords.y-1) == 50)
                return 0;
        }
    }
    if (kingcoords.x - 1 >= 0)
    {
        if (Board[kingcoords.x - 1][kingcoords.y].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x - 1, kingcoords.y) == 50)
            return 0;
        if (kingcoords.y + 1 < 8)
        {
            if (Board[kingcoords.x - 1][kingcoords.y + 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x - 1, kingcoords.y+1) == 50)
                return 0;
        }
        if (kingcoords.y - 1 >= 0)
        {
            if (Board[kingcoords.x - 1][kingcoords.y - 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x - 1, kingcoords.y-1) == 50)
                return 0;
        }
    }
    cout << "Checkmate!" << endl;
    return 1;
}



#endif // CHECKMATE_H_INCLUDED

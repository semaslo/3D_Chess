#ifndef SETUPBOARD_H_INCLUDED
#define SETUPBOARD_H_INCLUDED
//Sets up the board by assigning the correct values to the beginning positions
//Also declares this universal variable Piece Board[8][8] that stores where all the pieces are

Piece Board[8][8];

int SetUpBoard()
{
    //black pieces
    //black pawns
    for(int i=0; i < 8; i++)
    {
        Board[i][1].piecetype=1; //1 for pawn
        //change this side to black while we're at it
        Board[i][1].color = 0;
        Board[i][0].color = 0;
    }
    Board[0][0].piecetype = 2; //2 for rook
    Board[7][0].piecetype = 2;
    Board[1][0].piecetype = 3; //3 for knight
    Board[6][0].piecetype = 3;
    Board[5][0].piecetype = 4; //4 for bishop
    Board[2][0].piecetype = 4;
    Board[4][0].piecetype = 5; //5 for queen
    Board[3][0].piecetype = 6; //6 for king

    //white pieces
    //white pawns
    for(int i = 0; i < 8; i++)
    {
        Board[i][6].piecetype = 1;
        Board[i][7].color = 1;
        Board[i][6].color = 1;
    }
    Board[0][7].piecetype = 2;
    Board[7][7].piecetype = 2;
    Board[1][7].piecetype = 3;
    Board[6][7].piecetype = 3;
    Board[5][7].piecetype = 4;
    Board[2][7].piecetype = 4;
    Board[4][7].piecetype = 5;
    Board[3][7].piecetype = 6;

    //sets all other squares to 0 (empty)
    for(int i=0; i <8; i++)
    {
        for(int j=2; j<6; j++)
        {
            Board[i][j].piecetype = 0;
        }
    }
    return 0;
}


#endif // SETUPBOARD_H_INCLUDED

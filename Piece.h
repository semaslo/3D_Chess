#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

//A class for a chess piece
class Piece
{
public:
    //For piecetype: pawn in 1, rook is 2, knight is 3, bishop is 4, queen is 5 and king is 6
    int piecetype;
    bool color;  //0 is black, 1 is white

};

#endif // PIECE_H_INCLUDED

//
// Created by 정주신 on 24. 5. 12.
//

#ifndef MULTIPLECHESS_CHESSBOARD_H
#define MULTIPLECHESS_CHESSBOARD_H

#include "ChessPiece.h"

class ChessBoard {

private:
    ChessPiece chessPieces[32];

    int king_init_x = 5;
    int queen_init_x = 4;
    int rook_init_x[2] = {1,8};
    int bishop_init_x[2] = {3,6};
    int knight_init_x[2] = {2,5};
    int pawn_init_x[8] = {1,2,3,4,5,6,7,8};
    int white_pawn_init_y = 2;
    int black_pawn_init_y= 7;
    int white_special_init_y = 1;
    int black_special_init_y = 8;

public:
    ChessPiece &getChessPiece(int x, int y);
    ChessPiece &getChessPiece(int index);
    void init();
    ChessBoard();
    void reset();
    bool hasObstacleToMove(ChessPiece chessPiece, int x, int y);
    ChessPiece &getKing(Color color);
    ChessPiece &getByLocation(Location location);
    std::vector<Location> getPossibleLocations(ChessPiece chessPiece);
    bool isInPawnFirstLine(ChessPiece chess_piece);
    std::vector<Location> pawnEnemyLocations(ChessPiece chess_piece);
    bool isInLocation(Location location);
    void moveChessPice(ChessPiece chess_piece, Location location);
    bool canGoHere(ChessPiece chess_piece, Location location);
    void setChessPiece(int chessPiece_index, int x, int y);
};
#endif //MULTIPLECHESS_CHESSBOARD_H

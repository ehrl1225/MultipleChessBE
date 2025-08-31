//
// Created by 정주신 on 24. 5. 12.
//

#ifndef MULTIPLECHESS_CHESSPIECE_H
#define MULTIPLECHESS_CHESSPIECE_H

#include "Location.h"

// 구조체만 쓰임

enum Color{
    White,
    Black,
    NonColor,
};

enum ChessClass{
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
};


class ChessPiece {

private:
    Location location;
    bool isDead = false;
    Color color;
    ChessClass chessClass;
    bool promoted = false;
    ChessClass promotedClass = ChessClass::Pawn;

public:

    void move(int x, int y);
    void move(Location location);
    bool isHere(int x, int y);
    bool isHere(Location location);
    bool canMoveTo(int x, int y);
    bool isPromoted();
    void promoteTo(ChessClass chessClass_);
    bool isDiagonalFrom(int x, int y);
    bool isStraitFrom(int x, int y);
    int distanceFrom(int x, int y);
    bool hasDifferentColor(ChessPiece chess_piece);
    ChessPiece(ChessClass chessClass_, Color color_, int x, int y);
    ChessPiece();
    ChessClass &getChessClass();
    std::vector<Location> moveableLocations();
    Color getColor();
    Location &getLocation();
    bool isInitialized();
    std::vector<Location> routeLocations(int x, int y);


};

#endif //MULTIPLECHESS_CHESSPIECE_H

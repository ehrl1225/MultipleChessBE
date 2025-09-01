//
// Created by 정주신 on 25. 9. 1..
//

#ifndef MULTIPLECHESS_CHESSPIECE_H
#define MULTIPLECHESS_CHESSPIECE_H
#include <vector>

#include "Location.h"


class ChessPiece {
public:
    virtual ~ChessPiece() = default;

private:
    virtual std::vector<Location> possibleLocations() = 0;
};


#endif //MULTIPLECHESS_CHESSPIECE_H
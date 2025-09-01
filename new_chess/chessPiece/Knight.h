//
// Created by 정주신 on 25. 9. 1..
//

#ifndef MULTIPLECHESS_KNIGHT_H
#define MULTIPLECHESS_KNIGHT_H
#include <vector>

#include "../ChessPiece.h"


class Knight:ChessPiece {

public:
    std::vector<Location> possibleLocations() override;
};


#endif //MULTIPLECHESS_KNIGHT_H
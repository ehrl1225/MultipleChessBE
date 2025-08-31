//
// Created by 정주신 on 24. 5. 7.
//


#include "ChessPiece.h"

#include "ChessBoard.h"


void ChessPiece::move(int x, int y) {
    location.move(x,y);
}

void ChessPiece::move(Location location) {
    location.move(location);
}


bool ChessPiece::isHere(int x, int y) {
    return location.equals(x, y);
}

bool ChessPiece::isHere(Location location) {
    return this->location.equals(location);
}

bool ChessPiece::canMoveTo(int x, int y) {
    switch (this->chessClass){
        case ChessClass::King:
            if (distanceFrom(x, y) == 1){
                return true;
            }
        case ChessClass::Queen:
            if (isStraitFrom(x, y) || isDiagonalFrom(x, y)){
                return true;
            }
        case ChessClass::Rook:
            if (isStraitFrom(x, y)){
                return true;
            }
        case ChessClass::Bishop:
            if (isDiagonalFrom(x, y)){
                return true;
            }
        case ChessClass::Knight:

        case ChessClass::Pawn:

        default:
            return false;

    }
}

std::vector<Location> ChessPiece::moveableLocations() {
    std::vector<Location> locations;
    switch (this->chessClass) {
        case ChessClass::King:
        default:
            int a = 10;
    }
}

bool ChessPiece::isPromoted() {
    return this->promoted;
}

void ChessPiece::promoteTo(ChessClass chessClass_) {
    if (this->chessClass == ChessClass::Pawn){
        promoted = true;
        promotedClass = chessClass;
    }
}

bool ChessPiece::isDiagonalFrom(int x, int y) {
    return location.isDiagonalFrom(x, y);
}

int ChessPiece::distanceFrom(int x, int y) {
    return location.distanceFrom(x, y);
}

bool ChessPiece::isStraitFrom(int x, int y) {
    return location.isStraitFrom(x, y);
}

ChessPiece::ChessPiece(ChessClass chessClass_, Color color_, int x, int y) {
    location = Location(x,y);
    color = color_;
    chessClass = chessClass_;
    promoted = false;
    isDead = false;
}

ChessPiece::ChessPiece() {
    location = Location(0,0);
    color = Color::White;
    chessClass = ChessClass::Pawn;
    promoted = false;
    isDead = false;
}

ChessClass &ChessPiece::getChessClass() {
    return this->chessClass;
}

Color ChessPiece::getColor() {
    return color;
}

Location &ChessPiece::getLocation() {
    return location;
}

bool ChessPiece::isInitialized() {
    if (location.get_x() == 0 && location.get_y() == 0) {
        return false;
    }else {
        return true;
    }
}

bool ChessPiece::hasDifferentColor(ChessPiece chess_piece) {
    if (this->getColor() != chess_piece.getColor()) {
        return true;
    }
    return false;

}

std::vector<Location> ChessPiece::routeLocations(int x, int y) {
    std::vector<Location> locations = std::vector<Location>();
    Location currentLocation = getLocation();
    if (isDiagonalFrom(x,y)) {
        if (x >= currentLocation.get_x()) {
            if (y>= currentLocation.get_y()) {
                for (int dr = 1;dr<=(y-currentLocation.get_y());dr++){
                    locations.emplace_back(currentLocation.get_x()+dr, currentLocation.get_y()+dr);
                }
            }else {
                for (int dr = 1; dr<=(currentLocation.get_y()-y); dr++) {
                    locations.emplace_back(currentLocation.get_x()+dr, currentLocation.get_y()-dr);
                }
            }
        }else {
            if (y>= currentLocation.get_y()) {
                for (int dr = 1; dr<=(y-currentLocation.get_y());dr++) {
                    locations.emplace_back(currentLocation.get_x()-dr, currentLocation.get_y()+dr);
                }
            }else {
                for (int dr = 1; dr<=(currentLocation.get_y()-y);dr++) {
                    locations.emplace_back(currentLocation.get_x()-dr, currentLocation.get_y()-dr);
                }
            }
        }
    }else if (isStraitFrom(x,y)) {
        if (currentLocation.get_x() == x) {
            if (y>= currentLocation.get_y()) {
                for (int dy = 1; dy<=y-currentLocation.get_y();dy++) {
                    locations.push_back(Location(x,currentLocation.get_y()+dy));
                }
            }else {
                for (int dy = 1; dy<=currentLocation.get_y()-y;dy++) {
                    locations.push_back(Location(x,y+dy));
                }
            }
        }else if (currentLocation.get_y() == y) {
            if (x>= currentLocation.get_x()) {
                for (int dx = 1; dx<=x-currentLocation.get_x();dx++) {
                    locations.push_back(Location(currentLocation.get_x()+dx,y));
                }
            }else {
                for (int dx = 1; dx<=currentLocation.get_x()-x;dx++) {
                    locations.push_back(Location(x+dx,y));
                }
            }
        }

    }else if (distanceFrom(x,y)==2) {
        locations.emplace_back(x,y);
    }else {

    }
    return locations;
}




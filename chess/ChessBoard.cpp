//
// Created by 정주신 on 24. 5. 7.
//

// 여기의 대부분은 안쓰입니다.
#include "ChessBoard.h"

ChessPiece &ChessBoard::getChessPiece(int x, int y) {
    ChessPiece chess_piece;
    for (ChessPiece chessPiece : chessPieces){
        if (chessPiece.isHere(x,y)){
            chess_piece = chessPiece;
            break;
        }
    }
    return chess_piece;
}

void ChessBoard::init() {
    int y = white_pawn_init_y;
    int index = 0;
    Color color = Color::White;
    // index 0..7
    for (int x : pawn_init_x){
        ChessPiece chessPiece = ChessPiece(ChessClass::Pawn, color, x, y);
        chessPieces[index++] = chessPiece;
    }
    // index 8..9
    y = white_special_init_y;
    for (int x: rook_init_x){
        ChessPiece chessPiece = ChessPiece(ChessClass::Rook, color, x, y);
        chessPieces[index++] = chessPiece;
    }
    // index 10..11
    for (int x:knight_init_x){
        ChessPiece chessPiece = ChessPiece(ChessClass::Knight, color, x, y);
        chessPieces[index++] = chessPiece;
    }
    //index 12..13
    for (int x:bishop_init_x){
        ChessPiece chessPiece = ChessPiece(ChessClass::Bishop, color, x, y);
        chessPieces[index++] = chessPiece;
    }
    //index 14
    {
        int x = king_init_x;
        ChessPiece chessPiece = ChessPiece(ChessClass::King, color, x, y);
        chessPieces[index++] = chessPiece;
    }
    //index 15
    {
        int x = queen_init_x;
        ChessPiece chessPiece = ChessPiece(ChessClass::Queen, color, x, y);
        chessPieces[index++] = chessPiece;
    }



    color = Color::Black;
    y = black_pawn_init_y;
    //index 16..23
    for (int x:pawn_init_x){
        ChessPiece chessPiece = ChessPiece(ChessClass::Pawn, color, x, y);
        chessPieces[index++] = chessPiece;
    }

    y = black_special_init_y;
    //index 24..25
    for (int x:rook_init_x){
        ChessPiece chessPiece = ChessPiece(ChessClass::Rook, color, x,y);
        chessPieces[index++] = chessPiece;
    }
    //index 26..27
    for (int x:knight_init_x){
        ChessPiece chessPiece = ChessPiece(ChessClass::Knight, color, x, y);
        chessPieces[index++] = chessPiece;
    }
    //index 28..29
    for (int x:bishop_init_x){
        ChessPiece chessPiece = ChessPiece(ChessClass::Bishop, color, x, y);
        chessPieces[index++] = chessPiece;
    }
    //index 30
    {
        int x = king_init_x;
        ChessPiece chessPiece = ChessPiece(ChessClass::King, color, x, y);
        chessPieces[index++] = chessPiece;
    }
    //index 31
    {
        int x = queen_init_x;
        ChessPiece chessPiece = ChessPiece(ChessClass::Queen, color, x, y);
        chessPieces[index++] = chessPiece;
    }
}

void ChessBoard::reset() {
    //white set
    int y = white_pawn_init_y;
    int index = 0;
    for (int x : pawn_init_x){
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }

    y = white_special_init_y;
    for (int x: rook_init_x){
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }

    for (int x:knight_init_x){
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x, y);
    }

    for (int x:bishop_init_x){
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }
    {
        int x = king_init_x;
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }
    {
        int x = queen_init_x;
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }

    //black set
    y = black_pawn_init_y;

    for (int x:pawn_init_x){
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x, y);
    }

    y = black_special_init_y;

    for (int x:rook_init_x){
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x, y);
    }

    for (int x:knight_init_x){
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }

    for (int x:bishop_init_x){
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }
    {
        int x = king_init_x;
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }
    {
        int x = queen_init_x;
        ChessPiece chessPiece = chessPieces[index++];
        chessPiece.move(x,y);
    }
}

ChessBoard::ChessBoard() = default;

ChessPiece &ChessBoard::getKing(Color color) {
    ChessPiece* chessPiece;
    if (color == Color::White){
        chessPiece = &chessPieces[14];
    }else if (color == Color::Black){
        chessPiece = &chessPieces[30];
    }else {
    }
    return *chessPiece;
}

bool ChessBoard::hasObstacleToMove(ChessPiece chessPiece, int x, int y) {
    if (chessPiece.canMoveTo(x, y)){
        Color color = chessPiece.getColor();
        std::vector<Location> routeLocations = chessPiece.routeLocations(x,y);
        for (Location location : routeLocations) {
            ChessPiece chp = getChessPiece(x,y);

        }
    }
}

std::vector<Location> ChessBoard::getPossibleLocations(ChessPiece chessPiece) {
    std::vector<Location> possibleLocations;
    Color color = chessPiece.getColor();
    switch (chessPiece.getChessClass()) {
        case ChessClass::King:
            possibleLocations = chessPiece.getLocation().oneDistanceLocations();
            break;
        case ChessClass::Queen: {
            possibleLocations = chessPiece.getLocation().straitLocations();
            std::vector<Location> loc = chessPiece.getLocation().diagonalLocations();
            possibleLocations.insert(possibleLocations.end(), loc.begin(), loc.end());
            break;
        }
        case ChessClass::Bishop:
            possibleLocations = chessPiece.getLocation().diagonalLocations();
            break;
        case ChessClass::Knight:
            possibleLocations = chessPiece.getLocation().knightLocations();
            break;
        case ChessClass::Rook:
            possibleLocations = chessPiece.getLocation().straitLocations();
            break;
        case ChessClass::Pawn:{
            if (color == Color::White){
                possibleLocations = pawnEnemyLocations(chessPiece);
                Location location = chessPiece.getLocation().copy();
                if (location.get_y()<8) {
                    location.moveRelative(0, 1);
                    possibleLocations.push_back(location);
                }
                if (isInPawnFirstLine(chessPiece)) {
                    location = chessPiece.getLocation().copy();
                    location.moveRelative(0, 2);
                    possibleLocations.push_back(location);
                }
            }else if (color == Color::Black){
                possibleLocations = pawnEnemyLocations(chessPiece);
                Location location = chessPiece.getLocation().copy();
                if (location.get_y()>1) {
                    location.moveRelative(0, -1);
                    possibleLocations.push_back(location);
                }
                if (isInPawnFirstLine(chessPiece)) {
                    location = chessPiece.getLocation().copy();
                    location.moveRelative(0, -2);
                    possibleLocations.push_back(location);
                }
            }else{

            }
            break;
        }
        default:
            break;
    }
    return possibleLocations;
}


ChessPiece &ChessBoard::getByLocation(Location location) {
    for (ChessPiece &chessPiece :chessPieces){
        if (chessPiece.isHere(location)){
            return chessPiece;
        }
    }
    ChessPiece chess_piece;
    return chess_piece;
}

bool ChessBoard::isInPawnFirstLine(ChessPiece chess_piece) {
    Location location = chess_piece.getLocation();
    if (chess_piece.getColor() == Color::White) {
        if (location.get_y() == white_pawn_init_y) {
            return true;
        }
    }else if (chess_piece.getColor() == Color::Black) {
        if (location.get_y() == black_pawn_init_y) {
            return true;
        }
    }else {

    }
    return false;
}

std::vector<Location> ChessBoard::pawnEnemyLocations(ChessPiece chess_piece) {
    std::vector<Location> locations = std::vector<Location>();
    if (chess_piece.getColor() == Color::White) {
        Location location = chess_piece.getLocation().copy();
        location.moveRelative(-1,+1);
        ChessPiece chp = getByLocation(location);
        if (chp.isInitialized() && chp.getColor() == Color::Black) {
            locations.push_back(location);
        }
        location = chess_piece.getLocation().copy();
        location.moveRelative(+1,+1);
        chp = getByLocation(location);
        if (chp.isInitialized() && chp.getColor() == Color::Black) {
            locations.push_back(location);
        }
    }else if (chess_piece.getColor() == Color::Black) {
        Location location = chess_piece.getLocation().copy();
        location.moveRelative(-1,-1);
        ChessPiece chp = getByLocation(location);
        if (chp.isInitialized() && chp.getColor() == Color::White) {
            locations.push_back(location);
        }
        location = chess_piece.getLocation().copy();
        location.moveRelative(+1,-1);
        chp = getByLocation(location);
        if (chp.isInitialized()&&chp.getColor() == Color::White) {
            locations.push_back(location);
        }
    }else {

    }
    return locations;
}

bool ChessBoard::isInLocation(Location location) {
    for (ChessPiece chessPiece :chessPieces){
        if (chessPiece.isHere(location)){
            return true;
        }
    }
    return false;
}

bool ChessBoard::canGoHere(ChessPiece chess_piece, Location location) {
    if (chess_piece.getChessClass() == ChessClass::King) {
        if (chess_piece.distanceFrom(location.get_x(), location.get_y())==1) {
            ChessPiece chp = getByLocation(location);
            if (chp.isInitialized()) {
                if (chp.getColor() != chess_piece.getColor()) {
                    return true;
                }else {
                    return false;
                }
            }else {
                return true;
            }
        }else {
            return false;
        }
    }else if (chess_piece.getChessClass() == ChessClass::Queen) {
        if (chess_piece.isDiagonalFrom(location.get_x(), location.get_y())) {

        }else if (chess_piece.isStraitFrom(location.get_x(), location.get_y())) {

        }else {
            return false;
        }
    }
    else if (chess_piece.getChessClass() == ChessClass::Knight) {

    }else if (chess_piece.getChessClass() == ChessClass::Bishop) {

    }else if (chess_piece.getChessClass() == ChessClass::Rook) {

    }else if (chess_piece.getChessClass() == ChessClass::Pawn) {

    }
}


ChessPiece &ChessBoard::getChessPiece(int index) {
    return chessPieces[index];
}

void ChessBoard::setChessPiece(int chessPiece_index, int x, int y) {
    ChessPiece chess_piece = getChessPiece(chessPiece_index);
    chess_piece.move(x,y);
}

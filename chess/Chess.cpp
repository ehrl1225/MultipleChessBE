//
// Created by 정주신 on 24. 5. 7.
//

#include "Chess.h"
// 여기의 내용들은 대부분 안쓰입니다.

void Chess::init(){
    chessBoard = ChessBoard();
    chessBoard.init();
}

bool Chess::isCheck() {
    ChessPiece whiteKing = chessBoard.getKing(Color::White);
    ChessPiece BlackKing = chessBoard.getKing(Color::Black);


}

bool Chess::isCheckmate() {
    ChessPiece white_king = chessBoard.getKing(Color::White);
    ChessPiece black_king = chessBoard.getKing(Color::Black);

}

bool Chess::isStalemate() {

}

bool Chess::isCheckIn(Color color, int x, int y) {

}

Color Chess::getTurn() {
    return turn;
}

ChessBoard Chess::getChessBoard() {
    return chessBoard;
}

void Chess::setTurn(Color color) {
    turn = color;
}


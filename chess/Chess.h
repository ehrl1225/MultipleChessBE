//
// Created by 정주신 on 24. 5. 7.
//

#ifndef MULTIPLECHESS_CHESS_H
#define MULTIPLECHESS_CHESS_H

#include "ChessBoard.h"


class Chess {

private:

    ChessBoard chessBoard;
    Color turn = Color::White;
    int whiteScore = 0;
    int blackScore = 0;


public:
    Chess() = default;
    void init();
    bool isCheck();
    // 여기로 이동하면 체크를 당하는지를 보여줍니다.
    bool isCheckIn(Color color, int x, int y);
    //
    bool isStalemate();
    bool isCheckmate();
    ChessBoard getChessBoard();
    Color getTurn();
    void setTurn(Color color);

};
#endif //MULTIPLECHESS_CHESS_H

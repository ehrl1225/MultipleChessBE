//
// Created by 정주신 on 25. 9. 1..
//

#ifndef MULTIPLECHESS_MESSAGE_H
#define MULTIPLECHESS_MESSAGE_H
#include "DataInterface.h"
#include "../chess/ChessPiece.h"
#include "bytes/ByteConverterExtend.h"

class ChessPiecePositionRequest: DataInterface {
    size_t getSize() override {
        return 0;
    }
};

class NicknameSetting: DataInterface {
public:
    char nickname[100];

    size_t getSize() override {
        return ::getSize(nickname);
    }

    ByteConverter serialize() override {
        return packByteConverter(nickname);
    }
    void deserialize(ByteConverter& byte_converter) override {
        unpackByteConverter(byte_converter, nickname);
    }
};

class ChessColorSetting: DataInterface {
public:
    Color color;
};

class ChessPieceVote: DataInterface {
public:
    int chess_piece_id;
};

class ChessPiecePositionVote: DataInterface {
    int chess_piece_id;
    int x;
    int y;
    size_t getSize() override {
        return ::getSize(chess_piece_id, x, y);
    }

    ByteConverter serialize() override {
        return packByteConverter(chess_piece_id, x, y);
    }
    void deserialize(ByteConverter& byte_converter) override {
        unpackByteConverter(byte_converter, chess_piece_id, x, y);
    }
};

class ChessPiecePromotionVote: DataInterface {
    int chess_piece_id;
};

class ChessRequest: DataInterface {

};


#endif //MULTIPLECHESS_MESSAGE_H
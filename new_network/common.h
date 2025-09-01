//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_COMMON_H
#define MULTIPLECHESS_COMMON_H
#include "DataInterface.h"
#include "bytes/ByteConverterExtend.h"

enum MessageType:int32_t {
    // client to server
    CHESS_PIECE_POSITION_REQUEST,
    NICKNAME_SETTING,
    CHESS_COLOR_SETTING,
    CHESS_PIECE_VOTE,
    CHESS_PIECE_POSITION_VOTE,
    CHESS_PIECE_PROMOTION_VOTE,
    CHAT_SEND,
    DRAW_REQUEST,
    LOSE_REQUEST,
    // server to client
    CHESS_PIECE_POSITION_RESPONSE,
    CHESS_PIECE_VOTE_RESPONSE,
    CHESS_PIECE_POSITION_VOTE_RESPONSE,
    CHESS_PIECE_PROMOTION_VOTE_RESPONSE,
    CHESS_VOTE_DONE,
    CHAT_RECEIVE,
    CHESS_PIECE_CHANGED_POSITION,
    WIN_RESPONSE,
    DRAW_RESPONSE,
    LOSE_RESPONSE,
    NICKNAME_DUPLICATED,
    RECEIVED_VOTE,
    KILL,
    CHEAT_ACTIVATED,
    CHESS_PIECE_PROMOTION,
    EXIT,
    // message size 표시용
    MESSAGE_TYPE_LENGTH
};

enum MessageFrom:int32_t {
    SERVER,
    CLIENT,
};

class MessageInfo: DataInterface {
public:
    MessageType type;
    MessageFrom from;

    size_t getSize() override {
        return ::getSize(type, from);
    }

    ByteConverter serialize() override {
        return packByteConverter(type, from);
    }
    void deserialize(ByteConverter& byte_converter) override {
        unpackByteConverter(byte_converter, type, from);
    }
};

struct MessagePack {
    MessageInfo info;
    DataInterface* data;
};

#endif //MULTIPLECHESS_COMMON_H
//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_COMMON_H
#define MULTIPLECHESS_COMMON_H
#include "DataInterface.h"

enum MessageType {

    MESSAGE_TYPE_LENGTH
};

enum MessageFrom {
    SERVER,
    CLIENT,
};

struct MessageInfo {
    MessageType type;
    MessageFrom from;
};

struct MessagePack {
    MessageInfo info;
    DataInterface* data;
};

#endif //MULTIPLECHESS_COMMON_H
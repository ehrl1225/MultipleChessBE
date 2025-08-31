//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_COMMON_H
#define MULTIPLECHESS_COMMON_H

enum MessageType {

    MESSAGE_TYPE_LENGTH
};

enum MessageFrom {
    SERVER,
    CLIENT,
};

class Message {
public:
    char* serialize();

};


struct MessagePack {
    MessageType type;
    MessageFrom from;

};

#endif //MULTIPLECHESS_COMMON_H
//
// Created by 정주신 on 25. 9. 6..
//

#ifndef MULTIPLECHESS_RESPONSEHANDLER_H
#define MULTIPLECHESS_RESPONSEHANDLER_H
#include "../common.h"


class ResponseHandler {
    static ResponseHandler *instance;
    explicit ResponseHandler();
public:
    static ResponseHandler *getInstance();

    void sendMessage(MessageType messageType, MessageInterface* message, int32_t socket);
};


#endif //MULTIPLECHESS_RESPONSEHANDLER_H
//
// Created by 정주신 on 25. 9. 6..
//

#include "ResponseHandler.h"

#include <sys/socket.h>

ResponseHandler *ResponseHandler::instance = nullptr;

ResponseHandler *ResponseHandler::getInstance() {
    if (instance == nullptr) {
        instance = new ResponseHandler();
    }
    return instance;
}

ResponseHandler::ResponseHandler() {
}

void ResponseHandler::sendMessage(MessageType messageType, MessageInterface *message, int32_t socket) {
    MessageInfo info { messageType, SERVER};
    ByteConverter converter = message->serialize();
    ByteData data = converter.getData();
    send(socket, data.getData(), converter.getSize(), 0);
    delete message;
}


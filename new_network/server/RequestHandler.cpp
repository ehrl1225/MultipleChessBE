//
// Created by 정주신 on 25. 9. 4..
//

#include "RequestHandler.h"

RequestHandler *RequestHandler::instance = nullptr;

RequestHandler::RequestHandler() {
}

RequestHandler *RequestHandler::getInstance() {
    if (instance != nullptr) {
        return instance;
    }
    instance = new RequestHandler();
    return instance;
}

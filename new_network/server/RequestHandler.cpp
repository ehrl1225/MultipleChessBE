//
// Created by 정주신 on 25. 9. 4..
//

#include "RequestHandler.h"

RequestHandler::RequestHandler() {
}

RequestHandler *RequestHandler::getInstance() {
    return new RequestHandler();
}

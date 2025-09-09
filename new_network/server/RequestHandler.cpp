//
// Created by 정주신 on 25. 9. 4..
//

#include "RequestHandler.h"

#include <sys/socket.h>

#include "../message.h"

RequestHandler *RequestHandler::instance = nullptr;

RequestHandler::RequestHandler():memberController(MemberController::getInstance()) {
}

RequestHandler *RequestHandler::getInstance() {
    if (instance != nullptr) {
        return instance;
    }
    instance = new RequestHandler();
    return instance;
}

RequestHandler::~RequestHandler() {
    if (instance == nullptr) {
        return;
    }
    delete instance;
    instance = nullptr;
}


MessagePack* RequestHandler::handleRequest(int32_t socket, MessageInfo* message_info) {
    switch (message_info->type) {
        case LOGIN_REQUEST: {
            LoginRequest *login_request = receiveMessageInstance<LoginRequest>(socket);
            MessageInterface *login_response = memberController->login(login_request);
            MessagePack* message_pack = new MessagePack{
                MessageInfo{LOGIN_RESPONSE, SERVER},
                login_response
            };
            return message_pack;
        }
        case REGISTER_REQUEST: {

        }
        default: {
            return nullptr;
        }
    }
}

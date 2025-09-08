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

void RequestHandler::handleRequest(int32_t socket, MessageInfo &message_info) {
    switch (message_info.type) {
        case LOGIN_REQUEST: {
            LoginRequest* login_request = receiveMessageInstance<LoginRequest>(socket);
            if (!memberController->login(login_request)) {
                LoginResponse login_response = LoginResponse(FAIL, "fail to login");

                return;
            }

            LoginResponse login_response = LoginResponse(SUCCESS, "success to login");
        }
        default: ;
    }
}

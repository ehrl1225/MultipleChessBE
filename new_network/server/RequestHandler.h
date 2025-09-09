//
// Created by 정주신 on 25. 9. 4..
//

#ifndef MULTIPLECHESS_REQUESTHANDLER_H
#define MULTIPLECHESS_REQUESTHANDLER_H
#include <cstdint>

#include "../common.h"
#include "Member/MemberController.h"


class RequestHandler {
    static RequestHandler* instance;
    explicit RequestHandler();
    ~RequestHandler();
    MemberController* memberController;


public:
    static RequestHandler* getInstance();
    MessagePack* handleRequest(int32_t socket, MessageInfo* message_info);
};


#endif //MULTIPLECHESS_REQUESTHANDLER_H
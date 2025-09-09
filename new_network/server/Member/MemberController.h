//
// Created by 정주신 on 25. 9. 5..
//

#ifndef MULTIPLECHESS_MEMBERCONTROLLER_H
#define MULTIPLECHESS_MEMBERCONTROLLER_H
#include "MemberService.h"
#include "../../message.h"


class MemberController {
    static MemberController* instance;
    MemberService* memberService;
    explicit MemberController();

public:
    static MemberController* getInstance();
    MessageInterface* login(LoginRequest* login_request);
    void registerMember(std::string&& name, std::string&& password);
};


#endif //MULTIPLECHESS_MEMBERCONTROLLER_H
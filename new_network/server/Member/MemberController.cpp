//
// Created by 정주신 on 25. 9. 5..
//

#include "MemberController.h"

MemberController* MemberController::instance = nullptr;

MemberController::MemberController():memberService(MemberService::getInstance()) {

}

MemberController *MemberController::getInstance() {
    if (instance == nullptr) {
        instance = new MemberController();
    }
    return instance;
}

MessageInterface* MemberController::login(LoginRequest* login_request) {
    if (memberService->login(login_request->getNickname(), login_request->getPassword())) {
        return new LoginResponse(SUCCESS, "success to login");
    }
    return new LoginResponse(FAIL, "fail to login");
}

void MemberController::registerMember(std::string &&name, std::string &&password) {
    memberService->registerMember(std::move(name), std::move(password));
}


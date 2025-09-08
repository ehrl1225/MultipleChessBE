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

bool MemberController::login(LoginRequest* login_request) {
    return memberService->login(login_request->getNickname(), login_request->getPassword());
}

void MemberController::registerMember(std::string &&name, std::string &&password) {
    memberService->registerMember(std::move(name), std::move(password));
}


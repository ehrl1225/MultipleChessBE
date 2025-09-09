//
// Created by 정주신 on 25. 9. 5..
//

#include "MemberService.h"

#include "../../utils/PasswordHasher.h"

MemberService* MemberService::instance = nullptr;

MemberService::MemberService():database(Database::getInstance()) {}


MemberService *MemberService::getInstance() {
    if (instance == nullptr) {
        instance = new MemberService();
    }
    return instance;
}


bool MemberService::login(std::string&& name, std::string&& password) const {
    const ClientData& client =database->findClientByName(name)->get();
    return verify_password(password, client.getPassword());
}

void MemberService::registerMember(std::string&& name, std::string&& password) const {
    std::string hashed_password = hash_password(password);
    ClientData newClientData = ClientData(std::move(name), std::move(hashed_password));
    database->addClient(std::move(newClientData));
}

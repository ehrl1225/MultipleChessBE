//
// Created by 정주신 on 25. 9. 5..
//

#ifndef MULTIPLECHESS_MEMBERSERVICE_H
#define MULTIPLECHESS_MEMBERSERVICE_H
#include <string>

#include "../Database.h"


class MemberService {
    static MemberService* instance;
    Database* database;
    explicit MemberService();
public:
    static MemberService* getInstance();

    bool login(std::string name, std::string password);
    void registerMember(std::string&& name, std::string&& password);
};


#endif //MULTIPLECHESS_MEMBERSERVICE_H
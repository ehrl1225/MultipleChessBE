//
// Created by 정주신 on 25. 9. 4..
//

#ifndef MULTIPLECHESS_DATABASE_H
#define MULTIPLECHESS_DATABASE_H
#include <map>
#include <vector>

#include "ClientData.h"


class Database {
    std::map<std::string, ClientData> clients;


public:
    explicit Database();
    void addClient(const ClientData& client);


};


#endif //MULTIPLECHESS_DATABASE_H
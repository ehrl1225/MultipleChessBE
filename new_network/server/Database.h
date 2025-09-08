//
// Created by 정주신 on 25. 9. 4..
//

#ifndef MULTIPLECHESS_DATABASE_H
#define MULTIPLECHESS_DATABASE_H
#include <map>
#include <vector>

#include "ClientData.h"

using ConstClientDataRef = std::reference_wrapper<const ClientData>;

class Database {
    static Database* instance;
    std::map<std::string, ClientData> clients;
    explicit Database();

public:


    static Database* getInstance();
    void addClient(ClientData&& client);
    std::optional<ConstClientDataRef> findClientByName(const std::string &name);
    std::optional<ConstClientDataRef> findClientById(const std::string &id);
    const std::map<std::string, ClientData>& getClients();

};


#endif //MULTIPLECHESS_DATABASE_H
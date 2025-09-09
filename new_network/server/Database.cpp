//
// Created by 정주신 on 25. 9. 4..
//

#include "Database.h"

Database::Database() {
    clients = {};
}

Database::~Database() {
    if (instance == nullptr) {
        return;
    }
    delete instance;
    instance = nullptr;
}


void Database::addClient(ClientData&& client) {
    std::string id = client.getId();
    clients.emplace(id, std::move(client));
}

void Database::removeClient(ClientData &&client) {
    clients.erase(client.getId());
}


Database* Database::instance = nullptr;
Database* Database::getInstance() {
    if (instance == nullptr) {
        instance = new Database();
    }
    return instance;
}

std::optional<std::reference_wrapper<const ClientData>> Database::findClientByName(const std::string &name) {
    for (const std::pair<const std::string, ClientData>& pair: clients) {
        if (pair.second.getName() == name) {
            return std::cref(pair.second);
        }
    }
    return std::nullopt;
}

std::optional<ConstClientDataRef> Database::findClientById(const std::string &id) {
    return std::cref(clients.find(id)->second);
}

const std::map<std::string, ClientData>& Database::getClients() {
    return clients;
}
//
// Created by 정주신 on 25. 9. 4..
//

#include "Database.h"

Database::Database() {
    clients = {};
}


void Database::addClient(const ClientData& client) {
    clients.insert({client.getId(), client});
}


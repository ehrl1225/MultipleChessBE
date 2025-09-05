//
// Created by 정주신 on 25. 9. 1..
//

#include "ClientData.h"

#include <utility>

#include "../utils/UUID.h"

ClientData::ClientData(std::string name, std::string password):name(std::move(name)), password(std::move(password)),  id(UUID::generate()) {}


int32_t ClientData::getSocket() const {
    return socket;
}


void ClientData::setSocket(int32_t socket) {
    this->socket = socket;
}


const std::string &ClientData::getId() const {
    return id;
}

const std::string &ClientData::getName() const {
    return name;
}

const std::string &ClientData::getPassword() const {
    return password;
}

void ClientData::setName(const std::string &name) {
    this->name = name;
}

void ClientData::setId(const std::string &id) {
    this->id = id;
}

void ClientData::setPassword(const std::string &password) {
    this->password = password;
}



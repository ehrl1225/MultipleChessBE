//
// Created by 정주신 on 25. 9. 1..
//

#include "ClientData.h"

#include "../utils/UUID.h"

ClientData::ClientData(int32_t socket) {
    this->socket = socket;
    id = UUID::generate();
}


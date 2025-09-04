//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_SERVER_H
#define MULTIPLECHESS_SERVER_H
#include <cstdint>

#include "ServerInterface.h"


class Server {
public:
    static ServerInterface* getInstance(int16_t port, uint32_t thread_count = 1);
};


int main();

#endif //MULTIPLECHESS_SERVER_H
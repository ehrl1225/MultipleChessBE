//
// Created by 정주신 on 25. 8. 31..
//

#include "Server.h"

#include "KQueueServer.h"

inline ServerInterface *Server::getInstance(int16_t port, uint32_t threadCount) {
#ifdef __APPLE__
    return KQueueServer::getInstance(port, threadCount);
#endif
}

int main() {
    ServerInterface* server = Server::getInstance(8080, 10);
    server->start();
    return 0;
}

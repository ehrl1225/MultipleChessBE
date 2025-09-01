//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_SERVERINTERFACE_H
#define MULTIPLECHESS_SERVERINTERFACE_H

class ServerInterface {
protected:
    ServerInterface(int16_t port, uint32_t thread_count):port(port), thread_count(thread_count) {}
    int32_t listen_socket;
    int16_t port = 0;
    uint32_t thread_count = 0;
    virtual void start() = 0;
};

#endif //MULTIPLECHESS_SERVERINTERFACE_H
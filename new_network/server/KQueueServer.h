//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_KQUEUESERVER_H
#define MULTIPLECHESS_KQUEUESERVER_H

#ifdef __APPLE__
#include <cstdint>

#include "ServerInterface.h"

/**
 * 싱글톤 구조입니다.
 */
class KQueueServer: public ServerInterface {
private:
    static KQueueServer* instance;
    int32_t listen_socket = 0;
    int16_t port = 0;
    uint32_t thread_count = 1;
    explicit KQueueServer(int16_t port,uint32_t thread_count = 1):port(port) ,thread_count(thread_count) {};
    ~KQueueServer() = default;

public:
    void setThreadCount(uint32_t count);
    void createThreads() const;
    static KQueueServer* getInstance(int16_t port, uint32_t thread_count = 1);
    void start();
};

KQueueServer* KQueueServer::instance = nullptr;
#endif

#endif //MULTIPLECHESS_KQUEUESERVER_H
//
// Created by 정주신 on 25. 8. 31..
//
#ifdef __APPLE__
#include "KQueueServer.h"

#include <mutex>
#include <queue>
#include <thread>
#include <sys/socket.h>
#include <sys/event.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

#include "../common.h"

std::queue<int> task_queue;
std::mutex queue_mutex;
std::condition_variable condition;

#define PORT 9000

void KQueueServer::setThreadCount(uint32_t count) {
    this->thread_count = count;
}


[[noreturn]] void worker_thread_func() {
    while (true) {
        int client_socket;
        {
            std::unique_lock lock(queue_mutex);
            condition.wait(lock, []{return !task_queue.empty();});
            client_socket = task_queue.front();
            task_queue.pop();
        }
        MessageInfo info = MessageInfo();
        char* buffer = new char[info.getSize()];
        recv(client_socket, buffer, sizeof(buffer), 0);

        printf("Worker thread %d received: %s\n", std::this_thread::get_id(), buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    }
}

void KQueueServer::createThreads() const {
    const uint32_t NUM_THREADS = this->thread_count;
    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);
    for (int i = 0; i< NUM_THREADS; i++) {
        threads.emplace_back(worker_thread_func);
    }
}

KQueueServer * KQueueServer::getInstance(int16_t port, uint32_t thread_count) {
    if (instance == nullptr) {
        instance = new KQueueServer(port, thread_count);
    }
    return instance;
}

void check(int32_t value, const char* message) {
    if (value == -1) {
        throw std::runtime_error(message);
    }
}

void KQueueServer::setSocket() {
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    check(listen_socket, "socket() error");
    int optVal = 1;
    check(setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)), "setsockopt() error");
    sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    check(fcntl(listen_socket, F_SETFL, O_NONBLOCK), "fcntl() error");

    check(bind(listen_socket, (sockaddr*)&server_addr, sizeof(server_addr)), "bind() error");
    check(listen(listen_socket, SOMAXCONN), "listen() error");
}


void KQueueServer::start() {
    createThreads();
    setSocket();

    int kq = kqueue();

    struct kevent change_event{};
    EV_SET(&change_event, listen_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0,0,NULL);
    kevent(kq, &change_event, 1, nullptr, 0, nullptr);
    struct kevent event_list[32];
    while (true) {
        int num_events = kevent(kq, nullptr, 0, event_list, 32, nullptr);
        for (int i = 0; i<num_events; ++i) {
            if (event_list[i].ident == listen_socket) {
                sockaddr_in client_addr{};
                socklen_t client_addr_len = sizeof(client_addr);
                int client_socket = accept(listen_socket, (sockaddr*)&client_addr, &client_addr_len);
                fcntl(client_socket, F_SETFL, O_NONBLOCK);
                EV_SET(&change_event, client_socket, EVFILT_READ, EV_ADD|EV_ENABLE, 0,0, NULL);
                kevent(kq, &change_event, 1, nullptr, 0, nullptr);
# ifdef DEBUG
                printf("New client connected: %d\n", client_socket);
# endif
            }else if (event_list[i].flags & EV_EOF) {
                int client_socket = event_list[i].ident;
                EV_SET(&change_event, client_socket, EVFILT_READ, EV_DELETE, 0,0,NULL);
                kevent(kq, &change_event, 1, nullptr, 0, nullptr);
                close(client_socket);
            }else if (event_list[i].filter == EVFILT_READ) {
                int client_socket = event_list[i].ident;
                {
                    std::lock_guard lock(queue_mutex);
                    task_queue.push(client_socket);
                }
                condition.notify_one();
            }
        }
    }
}
#endif
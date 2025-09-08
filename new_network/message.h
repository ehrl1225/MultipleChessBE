//
// Created by 정주신 on 25. 9. 1
//

#ifndef MULTIPLECHESS_MESSAGE_H
#define MULTIPLECHESS_MESSAGE_H


#include <string>
#include <sys/socket.h>

#include "common.h"
#include "bytes/ByteConverterExtend.h"

#define MAX_NICKNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 72

template<typename T>
T* receiveMessageInstance(int32_t socket) {
    static_assert(std::is_base_of_v<MessageInterface, T>, "Template return type must be derived from DataInterface");
    T* t = new T();
    size_t size = t->getSize();
    int8_t* buffer = new int8_t[size];
    recv(socket, buffer, size, 0);
    ByteData byte_data(reinterpret_cast<uint8_t*>(buffer), size);
    ByteConverter converter(byte_data);
    t->deserialize(converter);
    delete[] buffer;
    return t;
}

template<typename T>
void sendMessageInstance(int32_t socket, T* t) {
    static_assert(std::is_base_of_v<MessageInterface, T>, "Template parameter type must be derived from DataInterface");
    ByteConverter converter = t->serialize();
    ByteData data = converter.getData();
    send(socket, data.getData(), converter.getSize(), 0);
}


class LoginRequest: MessageInterface {
    char nickname[MAX_NICKNAME_LENGTH]{};
    char password[MAX_PASSWORD_LENGTH]{};

public:
    explicit LoginRequest()= default;


    size_t getSize() override {
        return ::getSize(nickname, password);
    }

    void deserialize(ByteConverter &byte_converter) override {
        unpackByteConverter(byte_converter, nickname, password);
    }
    ByteConverter serialize() override {
        return packByteConverter(nickname, password);
    }

    std::string getNickname() {
        return nickname;
    }

    std::string getPassword() {
        return password;
    }
};

class LoginResponse: MessageInterface {
    ResponseStatus status{};
    std::string message;

public:
    explicit LoginResponse() = default;
    LoginResponse(
        ResponseStatus status,
        std::string&& message
        ) :
    status(status),
    message(std::move(message))
    {}

    size_t getSize() override {
        return ::getSize(message, status);
    }
    void deserialize(ByteConverter &byte_converter) override {
        unpackByteConverter(byte_converter, message, status);
    }

    ByteConverter serialize() override {
        return packByteConverter(message, status);
    }
};



#endif //MULTIPLECHESS_MESSAGE_H
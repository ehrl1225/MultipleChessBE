//
// Created by 정주신 on 25. 9. 1..
//

#ifndef MULTIPLECHESS_CLIENTDATA_H
#define MULTIPLECHESS_CLIENTDATA_H
#include <cstdint>
#include <string>

class ClientData {
    int32_t socket;
    std::string id;
public:
    explicit ClientData(int32_t socket);
    [[nodiscard]] int32_t getSocket() const;
    [[nodiscard]] std::string getId() const;
};


#endif //MULTIPLECHESS_CLIENTDATA_H
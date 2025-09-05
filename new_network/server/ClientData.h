//
// Created by 정주신 on 25. 9. 1..
//

#ifndef MULTIPLECHESS_CLIENTDATA_H
#define MULTIPLECHESS_CLIENTDATA_H
#include <cstdint>
#include <string>

class ClientData {
    std::string id;
    int32_t socket;
    std::string name;
    std::string password;
public:
    explicit ClientData(std::string name, std::string password);
    ~ClientData() = default;

    [[nodiscard]] int32_t getSocket() const;
    void setSocket(int32_t socket);
    [[nodiscard]] const std::string &getId() const;
    void setId(const std::string &id);
    [[nodiscard]] const std::string &getName() const;
    void setName(const std::string &name);
    [[nodiscard]] const std::string &getPassword() const;
    void setPassword(const std::string &password);
};


#endif //MULTIPLECHESS_CLIENTDATA_H
//
// Created by 정주신 on 25. 9. 4..
//

#include "PasswordHasher.h"

static std::string hash_password(const std::string& password) {
    std::vector<uint8_t> salt(16);
    try {
        std::random_device rd;
        std::generate(salt.begin(), salt.end(), [&](){return static_cast<uint8_t>(rd());});
    }catch (const std::exception& e) {
        throw std::runtime_error("Failed to generate salt");
    }

    std::vector<char> encoded_hash(128);
    uint32_t t_cost = 2;
    uint32_t m_cost = (1 << 16);
    uint32_t parallelism = 1;

    int result = argon2id_hash_encoded(t_cost, m_cost, parallelism,
        password.c_str(), password.length(),
        salt.data(), salt.size(), 32,
        encoded_hash.data(), encoded_hash.size());
    if (result != ARGON2_OK) {
        throw std::runtime_error("Failed to hash password");
    }
    return std::string(encoded_hash.data());
}

static bool verify_password(const std::string& password, const std::string& encoded_hash) {
    int result = argon2id_verify(encoded_hash.c_str(), password.c_str(), password.length());
    return result == ARGON2_OK;
}

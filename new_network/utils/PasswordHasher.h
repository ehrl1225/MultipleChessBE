//
// Created by 정주신 on 25. 9. 4..
//

#ifndef MULTIPLECHESS_PASSWORDHASHER_H
#define MULTIPLECHESS_PASSWORDHASHER_H
#include <random>
#include <string>
#include <vector>
#include <stdexcept>
#include "argon2.h"

static std::string hash_password(const std::string& password);

static bool verify_password(const std::string& password, const std::string& encoded_hash);


#endif //MULTIPLECHESS_PASSWORDHASHER_H
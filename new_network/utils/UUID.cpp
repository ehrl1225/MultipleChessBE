//
// Created by 정주신 on 25. 9. 1..
//

#include "UUID.h"
#include <uuid/uuid.h>
std::string UUID::generate() {
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_str[37];
    uuid_unparse_lower(uuid, uuid_str);
    return {uuid_str};
}
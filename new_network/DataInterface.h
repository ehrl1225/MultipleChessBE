//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_DATAINTERFACE_H
#define MULTIPLECHESS_DATAINTERFACE_H
#include "bytes/ByteConverter.h"

class DataInterface {
public:
    virtual ~DataInterface() = default;

    virtual size_t getSize() = 0;
    virtual ByteConverter serialize() = 0;
    virtual void deserialize(ByteConverter& byte_converter) = 0;
};

#endif //MULTIPLECHESS_DATAINTERFACE_H
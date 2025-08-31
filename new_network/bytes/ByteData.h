//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_BYTEDATA_H
#define MULTIPLECHESS_BYTEDATA_H
#include <cstdint>


class ByteData {
private:
    uint8_t* data = nullptr;
    size_t size = 0;
public:
    ByteData(uint8_t* data, size_t size);
    ByteData(const uint8_t* str, size_t size);
    ~ByteData();

    [[nodiscard]] size_t getSize() const;
    [[nodiscard]] const uint8_t* getData() const;
};


#endif //MULTIPLECHESS_BYTEDATA_H
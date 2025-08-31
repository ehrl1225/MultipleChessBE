//
// Created by 정주신 on 25. 8. 31..
//

#include "ByteData.h"

#include <cstring>

ByteData::ByteData(const uint8_t *str, size_t size) {
    this->data = new uint8_t[size];
    memcpy(this->data, str, size);
    this->size = size;
}

ByteData::ByteData(uint8_t *data, size_t size) {
    this->data = new uint8_t[size];
    memcpy(this->data, data, size);
    this->size = size;
}

const uint8_t *ByteData::getData() const {
    return data;
}

size_t ByteData::getSize() const {
    return size;
}

ByteData::~ByteData() {
    delete[] data;
}

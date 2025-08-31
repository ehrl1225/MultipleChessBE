//
// Created by 정주신 on 25. 8. 31..
//

#ifndef MULTIPLECHESS_BYTECONVERTER_H
#define MULTIPLECHESS_BYTECONVERTER_H

#include <cstdint>
#include <stdexcept>
#include <sys/_endian.h>

#include "ByteData.h"


class ByteConverter {
private:
    bool auto_grow = false;
    uint8_t* data = nullptr;
    size_t index = 0;
    size_t size = 0;
    size_t capacity = 0;
    uint8_t dequeueByte();
    void enqueueByte(uint8_t byte);
    void addCapacity(uint32_t size);
public:
    explicit ByteConverter(size_t capacity, bool auto_grow = false);
    ByteConverter(const ByteData& data);
    ~ByteConverter();

    ByteConverter& operator=(ByteConverter const& other);
    void resize();

    [[nodiscard]] ByteData getData() const;
    [[nodiscard]] size_t getSize() const;
    [[nodiscard]] size_t getCapacity() const;

    template<typename T>
    T dequeueValue() {
        size_t valueSize = sizeof(T);
        switch (valueSize) {
            case 1: {
                uint8_t bytes = data[index++];
                return *reinterpret_cast<T*>(&bytes);
            }
            case 2: {
                uint16_t bytes = ntohs(reinterpret_cast<uint16_t*>(&data[index])[0]);
                index += 2;
                return *reinterpret_cast<T*>(&bytes);
            }
            case 4: {
                uint32_t bytes = ntohl(reinterpret_cast<uint32_t*>(&data[index])[0]);
                index += 4;
                return *reinterpret_cast<T*>(&bytes);
            }
            case 8: {
                uint64_t bytes = ntohll(reinterpret_cast<uint64_t*>(&data[index])[0]);
                index += 8;
                return *reinterpret_cast<T*>(&bytes);
            }
            default:
                throw std::runtime_error("Invalid value size");
        }
    }

    template<typename T>
    void dequeueArray(T* (&array), size_t& pre_size) {
        size_t new_size = dequeueValue<size_t>();
        if (pre_size < new_size) {
            for (size_t i = 0; i < new_size; i++) {
                array[i] = dequeueValue<T>();
            }
            return;
        }
        delete[] array;
        T* new_array = new T[new_size];
        for (size_t i = 0; i < new_size; i++) {
            new_array[i] = dequeueValue<T>();
        }
        array = new_array;
        pre_size = new_size;
    }

    template<typename T>
    void enqueueValue(T value) {
        uint8_t* bytes;
        size_t byteSize = sizeof(T);
        if (size+byteSize > capacity) {
            if (auto_grow != true) {
                throw std::runtime_error("Too many bytes to enqueue");
            }
            addCapacity(byteSize);
        }
        switch (byteSize) {
            case 1: {
                bytes = reinterpret_cast<uint8_t*>(&value);
                break;
            }
            case 2: {
                uint16_t bytes16 = htons(*reinterpret_cast<uint16_t*>(&value));
                bytes = reinterpret_cast<uint8_t*>(&bytes16);
                break;
            }
            case 4: {
                uint32_t bytes32 = htonl(*reinterpret_cast<uint32_t*>(&value));
                bytes = reinterpret_cast<uint8_t*>(&bytes32);
                break;
            }
            case 8: {
                uint64_t bytes64 = htonll(*reinterpret_cast<uint64_t*>(&value));
                bytes = reinterpret_cast<uint8_t*>(&bytes64);
                break;
            }
            default:
                throw std::invalid_argument("Invalid value size");

        }
        for (size_t i = 0; i< byteSize; i++) {
            enqueueByte(bytes[i]);
        }
    }

    template<typename T>
    void enqueueArray(T* values, size_t length) {
        for (size_t i = 0; i<length; i++) {
            enqueueValue(values[i]);
        }
    }

    void extend(ByteConverter& other);

    void extend(ByteConverter* other, size_t length);

    template<typename... Args>
    void extend(ByteConverter* others, size_t length, Args&&... args) {
        enqueueValue(length);
        for (size_t i =0; i< length; i++) {
            extend(others[i]);
        }
        extend(args...);
    }
    template<typename... Args>
    void extend(ByteConverter& other, Args&&... args) {
        extend(other);
        extend(args...);
    }

    ByteConverter operator+(const ByteConverter& other);

};


#endif //MULTIPLECHESS_BYTECONVERTER_H
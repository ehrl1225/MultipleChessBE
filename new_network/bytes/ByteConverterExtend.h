//
// Created by 정주신 on 25. 8. 31
//

#ifndef MULTIPLECHESS_BYTECONVERTEREXTEND_H
#define MULTIPLECHESS_BYTECONVERTEREXTEND_H
#include <cstdint>

#include "ByteConverter.h"

/**
 *
 */
template<typename T, size_t N>
size_t getSize(T (&)[N]) {
    return N * sizeof(T);
}

template<typename T, size_t N, typename ...Args>
size_t getSize(T (&)[N], Args&&...args) {
    return N*sizeof(T) + getSize(args...);
}

template<typename T>
size_t getSize(T*, size_t size) {
    return size * sizeof(T) + sizeof(size_t);
}

template<typename T, typename ...Args>
size_t getSize(T*, size_t size, Args&&... args) {
    return size*sizeof(T) + sizeof(size_t) + getSize(args...);
}

template<typename T>
size_t getSize(T&) {
    return sizeof(T);
}

template<typename T, typename... Args>
size_t getSize(T&, Args&&... args) {
    return sizeof(T) + getSize(args...);
}

/**
 *
 */
template<typename T, size_t N>
void addBytes(ByteConverter& byte_converter, T (&t)[N]) {
    for (size_t i = 0; i< N; i++) {
        byte_converter.enqueueValue(t[i]);
    }
}

template<typename T, size_t N, typename ...Args>
void addBytes(ByteConverter& byte_converter, T (&t)[N], Args&&... args) {
    for (size_t i = 0; i< N; i++) {
        byte_converter.enqueueValue(t[i]);
    }
    addBytes(byte_converter, args...);
}

template<typename T>
void addBytes(ByteConverter& byte_converter, T t) {
    byte_converter.enqueueValue(t);
}

template<typename T, typename... Args>
void addBytes(ByteConverter& byte_converter, T t, Args&&... args) {
    byte_converter.enqueueValue(t);
    addBytes(byte_converter, args...);
}

template<typename T>
void addBytes(ByteConverter& byte_converter, T* t, size_t size) {
    byte_converter.enqueueValue(size);
    byte_converter.enqueueArray(t, size);
}

template<typename T, typename... Args>
void addBytes(ByteConverter& byte_converter, T* t, size_t size, Args&&... args) {
    byte_converter.enqueueValue(size);
    byte_converter.enqueueArray(t, size);
    addBytes(byte_converter, args...);
}

/**
 *
 */

template<typename T, size_t N>
void setValue(ByteConverter& byte_converter, T (&t)[N]) {
    for (size_t i = 0; i< N; i++) {
        t[i] = byte_converter.dequeueValue<T>();
    }
}

template<typename T, size_t N, typename ...Args>
void setValue(ByteConverter& byte_converter, T (&t)[N], Args&&... args) {
    for (size_t i = 0; i< N; i++) {
        t[i] = byte_converter.dequeueValue<T>();
    }
    setValue(byte_converter, args...);
}

template<typename T>
void setValue(ByteConverter& byte_converter, T* (&t), size_t (&size)) {
    byte_converter.dequeueArray(t, size);
}

template<typename T>
void setValue(ByteConverter& byte_converter, T (&t)) {
    *t = byte_converter.dequeueValue<T>();
}

template<typename T, typename  ...Args>
void setValue(ByteConverter& byte_converter, T* (&t), size_t (&size), Args&&... args) {
    byte_converter.dequeueArray(t, size);
    setValue(byte_converter, args...);
}

template<typename T, typename... Args>
void setValue(ByteConverter& byte_converter, T (&value), Args&&... args) {
    value = byte_converter.dequeueValue<T>();
    setValue(byte_converter, args...);
}

template<typename... Args>
ByteConverter packByteConverter(Args&&... args) {
    size_t size = getSize(args...);
    ByteConverter converter(size, false);
    addBytes(converter, args...);
    return converter;
}

template<typename... Args>
void unpackByteConverter(ByteConverter& byte_converter, Args&&... args) {
    setValue(byte_converter, args...);
}

template<typename... Args>
ByteConverter combileByteConverter(Args... args) {
    size_t size = getSize(args...);
    ByteConverter converter(size);
    converter.extend(args...);
    return converter;
}


#endif //MULTIPLECHESS_BYTECONVERTEREXTEND_H
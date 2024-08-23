#ifndef BUFFER_H
#define BUFFER_H
#include <cstdint>
#include "array_fun.h"
#include "math_fun.h"

class Buffer{
private:
    uint8_t* buffer{nullptr};
    uint16_t bufferSize{0};
    uint16_t bufferIndex{0};
protected:
    void addByte(uint8_t value);
    void addBytes(const uint8_t *const src, uint16_t start, uint16_t quantity);
    void addBytes(const uint8_t *const src, uint16_t len);
public:
    Buffer(uint16_t bufferSize);
    bool bufferOverFlow() const;
    bool bufferIsEmpty() const;
    bool bufferIsNotEmpty() const;
    uint16_t getBufferSize() const;
    uint16_t getBufferIndex() const;
    void getBytes(uint8_t *const dst, uint16_t start, uint16_t len);
    uint8_t getByte(uint16_t index);
    void clearBytes(uint16_t start, uint16_t quantity);
    void clearByte(uint16_t index);
    void getAll(uint8_t *const dst);
    void clearALl();
    ~Buffer();
};

#endif //BUFFER_H

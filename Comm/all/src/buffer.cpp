#include "buffer.h"

Buffer::Buffer(uint16_t bufferSize): bufferSize(bufferSize) {
    buffer = new uint8_t[bufferSize];
    fillArray(buffer, bufferSize, (uint8_t)0);
}

Buffer::~Buffer() {
    delete[] buffer;
}

bool Buffer::bufferOverFlow() const {
    return bufferIndex >= bufferSize;
}

void Buffer::getBytes(uint8_t *const dst, uint16_t start, uint16_t quantity){
    start = min(start, (uint16_t)(bufferSize - 1));
    quantity = min(quantity, (uint16_t)(bufferSize - start));
    getPartOfArray(buffer, bufferSize, start, quantity, dst);
}

uint8_t Buffer::getByte(uint16_t index){
    index = min(index, (uint16_t )(bufferSize - 1));
    return buffer[index];
}

void Buffer::clearBytes(uint16_t start, uint16_t quantity){
    start = min(start, (uint16_t)(bufferSize - 1));
    quantity = min(quantity, (uint16_t)(bufferSize - start));
    deleteElementInArray(buffer, bufferSize, start, quantity, (uint8_t)0);
    quantity = min(bufferIndex, quantity);
    bufferIndex -= quantity;
}

void Buffer::clearByte(uint16_t index){
    clearBytes(index, 1);
}

bool Buffer::bufferIsEmpty() const {
    return bufferIndex == 0;
}

bool Buffer::bufferIsNotEmpty() const {
    return !bufferIsEmpty();
}

void Buffer::addByte(uint8_t value) {
    if(bufferIndex < bufferSize) {
        buffer[bufferIndex] = value;
        bufferIndex++;
    }
}


void Buffer::addBytes(const uint8_t *const src, uint16_t start, uint16_t quantity) {
    for(uint16_t i = start; i < start + quantity; i++){
        addByte(src[i]);
    }
}

void Buffer::addBytes(const uint8_t *const src, uint16_t len) {
    addBytes(src, 0, len);
}

uint16_t Buffer::getBufferSize() const {
    return bufferSize;
}

void Buffer::getAll(uint8_t *const dst) {
    copyArrays(buffer, dst, bufferIndex);
}

void Buffer::clearALl() {
    fillArray(buffer, (uint32_t)bufferSize, (uint8_t)0);
}

uint16_t Buffer::getBufferIndex() const {
    return bufferIndex;
}

uint8_t Buffer::getAndClearByte(uint16_t index) {
    uint8_t res = getByte(index);
    clearByte(index);
    return res;
}

void Buffer::getAndClearBytes(uint8_t *const dst, uint16_t start, uint16_t quantity) {
    getBytes(dst, start, quantity);
    clearBytes(start, quantity);
}

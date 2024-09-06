#include "buffer.h"

Buffer::Buffer(uint16_t bufferSize): bufferSize(bufferSize) {
    buffer = new uint8_t[bufferSize];
    fillArray(buffer, bufferSize, (uint8_t)0);
}

Buffer::~Buffer() {
    delete[] buffer;
}

void Buffer::limIndex(uint16_t &index) const {
    index = min(index, (uint16_t)(bufferSize - 1));
}

void Buffer::limQuan(uint16_t &quantity, uint16_t shift) const {
    quantity = min(quantity, (uint16_t)(bufferSize - shift));
}

void Buffer::clearXBytes(uint16_t start, uint16_t quantity, uint8_t size) {
    quantity *= size;
    limIndex(start);
    limQuan(quantity, start);
    deleteElementInArray(buffer, bufferSize, start, quantity, (uint8_t)0);
    quantity = min(bufferIndex, quantity);
    bufferIndex -= quantity;
}

void Buffer::addByte(uint8_t value) {
    if(bufferIndex < bufferSize) {
        buffer[bufferIndex] = value;
        bufferIndex++;
    }
}
void Buffer::addByte(const uint8_t *const src, uint16_t start, uint16_t quantity) {
    for(uint16_t i = start; i < start + quantity; i++){
        addByte(src[i]);
    }
}
void Buffer::addByte(const uint8_t *const src, uint16_t len) {
    addByte(src, 0, len);
}

//buffer info
bool Buffer::bufferIsEmpty() const {
    return bufferIndex == 0;
}
bool Buffer::bufferIsOverFlow() const {
    return bufferIndex >= bufferSize;
}
bool Buffer::bufferIsNotEmpty() const {
    return !bufferIsEmpty();
}
uint16_t Buffer::getBufferSize() const {
    return bufferSize;
}
uint16_t Buffer::getBufferIndex() const {
    return bufferIndex;
}

//get
void Buffer::getByte(uint8_t *const dst, uint16_t start, uint16_t quantity){
    limIndex(start);
    limQuan(quantity, start);
    getPartOfArray(buffer, bufferSize, start, quantity, dst);
}
uint8_t Buffer::getByte(uint16_t index){
    limIndex(index);
    return buffer[index];
}
uint8_t Buffer::getByte() {
    return getByte(0);
}

uint16_t Buffer::getWord(uint16_t index) {
    return getX<uint16_t>(index, buffer, bufferSize);
}
uint16_t Buffer::getWord() {
    return getWord(0);
}
void Buffer::getWord(uint16_t *const dst, uint16_t start, uint16_t quantity) {
    for(uint16_t i = 0; i < quantity; i++){
        dst[i] = getWord(2 * i + start);
    }
}

uint32_t Buffer::getDWord(uint16_t index) {
    return getX<uint32_t>(index, buffer, bufferSize);
}
uint32_t Buffer::getDWord() {
    return getDWord(0);
}
void Buffer::getDWord(uint32_t *const dst, uint16_t start, uint16_t quantity) {
    for(uint16_t i = 0; i < quantity; i++){
        dst[i] = getDWord(4 * i + start);
    }
}

float Buffer::getFloat(uint16_t index) {
    return dWordToFloat(getDWord(index));
}
float Buffer::getFloat() {
    return getFloat(0);
}
void Buffer::getFloat(float *const dst, uint16_t start, uint16_t quantity) {
    for(uint16_t i = 0; i < quantity; i++){
        dst[i] = dWordToFloat(getDWord(4 * i + start));
    }
}

//clear
void Buffer::clearByte(uint16_t start, uint16_t quantity){
    clearXBytes(start, quantity, 1);
}

void Buffer::clearByte(uint16_t index){
    clearByte(index, 1);
}

void Buffer::clearByte() {
    clearByte(0);
}

void Buffer::clearWord(uint16_t start, uint16_t quantity) {
    clearXBytes(start, quantity, 2);
}
void Buffer::clearWord(uint16_t index) {
    clearWord(index, 1);
}
void Buffer::clearWord() {
    clearWord(0);
}

void Buffer::clearDWord(uint16_t start, uint16_t quantity) {
    clearXBytes(start, quantity, 4);
}
void Buffer::clearDWord(uint16_t index) {
    clearDWord(index, 1);
}
void Buffer::clearDWord() {
    clearDWord(0);
}

void Buffer::clearFloat(uint16_t start, uint16_t quantity) {
    clearDWord(start, quantity);
}
void Buffer::clearFloat(uint16_t index) {
    clearFloat(index, 1);
}
void Buffer::clearFloat() {
    clearFloat(0);
}

//getAndClear
uint8_t Buffer::getAndClearByte(uint16_t index) {
    uint8_t res = getByte(index);
    clearByte(index);
    return res;
}
uint8_t Buffer::getAndClearByte() {
    return getAndClearByte(0);
}
void Buffer::getAndClearByte(uint8_t *const dst, uint16_t start, uint16_t quantity) {
    getByte(dst, start, quantity);
    clearByte(start, quantity);
}

uint16_t Buffer::getAndClearWord(uint16_t index) {
    uint16_t res = getWord(index);
    clearWord(index);
    return res;
}
uint16_t Buffer::getAndClearWord() {
    return getAndClearWord(0);
}
void Buffer::getAndClearWord(uint16_t *dst, uint16_t start, uint16_t quantity) {
    getWord(dst, start, quantity);
    clearWord(start, quantity);
}

uint32_t Buffer::getAndClearDWord(uint16_t index) {
    uint32_t res = getDWord(index);
    clearDWord(index);
    return res;
}
uint32_t Buffer::getAndClearDWord() {
    return getAndClearDWord(0);
}
void Buffer::getAndClearDWord(uint32_t *const dst, uint16_t start, uint16_t quantity) {
    getDWord(dst, start, quantity);
    clearDWord(start, quantity);
}

float Buffer::getAndClearFloat(uint16_t index) {
    float res = getFloat(index);
    clearFloat(index);
    return res;
}
float Buffer::getAndClearFloat() {
    return getAndClearFloat(0);
}
void Buffer::getAndClearFloat(float *const dst, uint16_t start, uint16_t quantity) {
    getFloat(dst, start, quantity);
    clearFloat(start, quantity);
}


//all
void Buffer::getAll(uint8_t *const dst) {
    copyArrays(buffer, dst, bufferIndex);
}
void Buffer::clearALl() {
    fillArray(buffer, (uint32_t)bufferSize, (uint8_t)0);
}

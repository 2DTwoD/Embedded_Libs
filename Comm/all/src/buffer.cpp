#include "buffer.h"

//InternalBuffer
InternalBuffer::InternalBuffer(uint16_t bufferSize): bufferSize(bufferSize) {
    buffer = new uint8_t[bufferSize];
    fillArray(buffer, bufferSize, (uint8_t)0);
}

InternalBuffer::~InternalBuffer() {
    delete[] buffer;
}

void InternalBuffer::limIndex(uint16_t &index) const {
    index = min(index, (uint16_t)(bufferSize - 1));
}

void InternalBuffer::limQuan(uint16_t &quantity, uint16_t shift) const {
    quantity = min(quantity, (uint16_t)(bufferSize - shift));
}

void InternalBuffer::clearXBytes(uint16_t start, uint16_t quantity, uint8_t size) {
    quantity *= size;
    limIndex(start);
    limQuan(quantity, start);
    deleteElementInArray(buffer, bufferSize, start, quantity, (uint8_t)0);
    quantity = min(bufferIndex, quantity);
    bufferIndex -= quantity;
}

//add
void InternalBuffer::addByte(uint8_t value) {
    if(bufferIndex < bufferSize) {
        buffer[bufferIndex] = value;
        bufferIndex++;
    }
}
void InternalBuffer::addByte(const uint8_t *const src, uint16_t start, uint16_t quantity) {
    for(uint16_t i = start; i < start + quantity; i++){
        addByte(src[i]);
    }
}
void InternalBuffer::addByte(const uint8_t *const src, uint16_t len) {
    addByte(src, 0, len);
}

//buffer info
bool InternalBuffer::bufferIsEmpty() const {
    return bufferIndex == 0;
}
bool InternalBuffer::bufferIsOverFlow() const {
    return bufferIndex >= bufferSize;
}
bool InternalBuffer::bufferIsNotEmpty() const {
    return !bufferIsEmpty();
}
uint16_t InternalBuffer::getBufferSize() const {
    return bufferSize;
}
uint16_t InternalBuffer::getBufferIndex() const {
    return bufferIndex;
}

//get
void InternalBuffer::getByte(uint8_t *const dst, uint16_t start, uint16_t quantity){
    limIndex(start);
    limQuan(quantity, start);
    getPartOfArray(buffer, bufferSize, start, quantity, dst);
}
uint8_t InternalBuffer::getByte(uint16_t index){
    limIndex(index);
    return buffer[index];
}
uint8_t InternalBuffer::getByte() {
    return getByte(0);
}

uint16_t InternalBuffer::getWord(uint16_t index) {
    return getX<uint16_t>(index, buffer, bufferSize);
}
uint16_t InternalBuffer::getWord() {
    return getWord(0);
}
void InternalBuffer::getWord(uint16_t *const dst, uint16_t start, uint16_t quantity) {
    for(uint16_t i = 0; i < quantity; i++){
        dst[i] = getWord(2 * i + start);
    }
}

uint32_t InternalBuffer::getDWord(uint16_t index) {
    return getX<uint32_t>(index, buffer, bufferSize);
}
uint32_t InternalBuffer::getDWord() {
    return getDWord(0);
}
void InternalBuffer::getDWord(uint32_t *const dst, uint16_t start, uint16_t quantity) {
    for(uint16_t i = 0; i < quantity; i++){
        dst[i] = getDWord(4 * i + start);
    }
}

float InternalBuffer::getFloat(uint16_t index) {
    return dWordToFloat(getDWord(index));
}
float InternalBuffer::getFloat() {
    return getFloat(0);
}
void InternalBuffer::getFloat(float *const dst, uint16_t start, uint16_t quantity) {
    for(uint16_t i = 0; i < quantity; i++){
        dst[i] = dWordToFloat(getDWord(4 * i + start));
    }
}

//clear
void InternalBuffer::clearByte(uint16_t start, uint16_t quantity){
    clearXBytes(start, quantity, 1);
}

void InternalBuffer::clearByte(uint16_t index){
    clearByte(index, 1);
}

void InternalBuffer::clearByte() {
    clearByte(0);
}

void InternalBuffer::clearWord(uint16_t start, uint16_t quantity) {
    clearXBytes(start, quantity, 2);
}
void InternalBuffer::clearWord(uint16_t index) {
    clearWord(index, 1);
}
void InternalBuffer::clearWord() {
    clearWord(0);
}

void InternalBuffer::clearDWord(uint16_t start, uint16_t quantity) {
    clearXBytes(start, quantity, 4);
}
void InternalBuffer::clearDWord(uint16_t index) {
    clearDWord(index, 1);
}
void InternalBuffer::clearDWord() {
    clearDWord(0);
}

void InternalBuffer::clearFloat(uint16_t start, uint16_t quantity) {
    clearDWord(start, quantity);
}
void InternalBuffer::clearFloat(uint16_t index) {
    clearFloat(index, 1);
}
void InternalBuffer::clearFloat() {
    clearFloat(0);
}

//getAndClear
uint8_t InternalBuffer::getAndClearByte(uint16_t index) {
    uint8_t res = getByte(index);
    clearByte(index);
    return res;
}
uint8_t InternalBuffer::getAndClearByte() {
    return getAndClearByte(0);
}
void InternalBuffer::getAndClearByte(uint8_t *const dst, uint16_t start, uint16_t quantity) {
    getByte(dst, start, quantity);
    clearByte(start, quantity);
}

uint16_t InternalBuffer::getAndClearWord(uint16_t index) {
    uint16_t res = getWord(index);
    clearWord(index);
    return res;
}
uint16_t InternalBuffer::getAndClearWord() {
    return getAndClearWord(0);
}
void InternalBuffer::getAndClearWord(uint16_t *dst, uint16_t start, uint16_t quantity) {
    getWord(dst, start, quantity);
    clearWord(start, quantity);
}

uint32_t InternalBuffer::getAndClearDWord(uint16_t index) {
    uint32_t res = getDWord(index);
    clearDWord(index);
    return res;
}
uint32_t InternalBuffer::getAndClearDWord() {
    return getAndClearDWord(0);
}
void InternalBuffer::getAndClearDWord(uint32_t *const dst, uint16_t start, uint16_t quantity) {
    getDWord(dst, start, quantity);
    clearDWord(start, quantity);
}

float InternalBuffer::getAndClearFloat(uint16_t index) {
    float res = getFloat(index);
    clearFloat(index);
    return res;
}
float InternalBuffer::getAndClearFloat() {
    return getAndClearFloat(0);
}
void InternalBuffer::getAndClearFloat(float *const dst, uint16_t start, uint16_t quantity) {
    getFloat(dst, start, quantity);
    clearFloat(start, quantity);
}


//all
void InternalBuffer::getAll(uint8_t *const dst) {
    copyArrays(buffer, dst, bufferIndex);
}
void InternalBuffer::clearALl() {
    fillArray(buffer, (uint32_t)bufferSize, (uint8_t)0);
}

//----------------------------------------------------------------------
//Buffer
Buffer::Buffer(uint16_t bufferSize) : InternalBuffer(bufferSize) {}

//add
void Buffer::addByte(uint8_t value) {
    InternalBuffer::addByte(value);
}

void Buffer::addByte(const uint8_t *const src, uint16_t start, uint16_t quantity) {
    InternalBuffer::addByte(src, start, quantity);
}

void Buffer::addByte(const uint8_t *const src, uint16_t len) {
    InternalBuffer::addByte(src, len);
}

void Buffer::addWord(uint16_t value) {
    addByte((uint8_t) (value >> 8));
    addByte((uint8_t) (value & 0xFF));
}

void Buffer::addWord(const uint16_t *const src, uint16_t start, uint16_t quantity) {
    for(uint16_t i = start; i < start + quantity; i++){
        addWord(src[i]);
    }
}

void Buffer::addWord(const uint16_t *const src, uint16_t len) {
    addWord(src, 0, len);
}

void Buffer::addDWord(uint32_t value) {
    addByte((uint8_t) (value >> 24));
    addByte((uint8_t) ((value >> 16) & 0xFF));
    addByte((uint8_t) ((value >> 8) & 0xFF));
    addByte((uint8_t) (value & 0xFF));
}

void Buffer::addDWord(const uint32_t *const src, uint16_t start, uint16_t quantity) {
    for(uint16_t i = start; i < start + quantity; i++){
        addDWord(src[i]);
    }
}

void Buffer::addDWord(const uint32_t *src, uint16_t len) {
    addDWord(src, 0, len);
}

void Buffer::addFloat(float value) {
    addDWord(floatToDWord(value));
}

void Buffer::addFloat(const float *src, uint16_t start, uint16_t quantity) {
    for(uint16_t i = start; i < start + quantity; i++){
        addFloat(src[i]);
    }
}

void Buffer::addFloat(const float *src, uint16_t len) {
    addFloat(src, 0, len);
}

void Buffer::insertByte(uint16_t index, uint8_t value) {
    if(index > getBufferSize() - 1) return;
    insertElementInArray(buffer, getBufferSize(), index, value);
}

void Buffer::insertByte(uint8_t value) {
    insertByte(0, value);
}

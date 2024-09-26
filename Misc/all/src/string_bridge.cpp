#include <cstring>
#include "string_bridge.h"

StringBridgeIndex::StringBridgeIndex(const char *name, uint16_t byte, uint8_t bit, StringListType type):
                                                        name(name), byte(byte), bit(bit), type(type) {}

StringBridge::StringBridge(uint16_t size) {
    data = new Buffer(size);
    keys = new ArrayList<StringBridgeIndex*>(nullptr);
}

StringBridge::~StringBridge() {
    for(uint16_t i = 0; i < keys->size(); i++){
        delete keys->get(i);
    }
    delete data;
    delete keys;
}

StringBridgeIndex *StringBridge::getCoordinate(const char *name) {
    for(uint16_t i = 0; i < keys->size(); i++){
        StringBridgeIndex *stringListIndex = keys->get(i);
        if(strcmp(stringListIndex->name, name) == 0){
            return stringListIndex;
        }
    }
    return nullptr;
}
bool StringBridge::add(const char *name, uint8_t len, uint16_t byte, uint8_t bit, StringListType type) {
    if(currentIndex + len > data->getBufferSize()) return false;
    keys->add(new StringBridgeIndex(name, byte, bit, type));
    currentIndex += len;
    return true;
}
void StringBridge::dataSetBit(uint16_t byte, uint8_t pos, bool value) {
    data->setByte(byte, ~(1 << pos) & data->getByte(byte));
    data->setByte(byte, (value << pos) | data->getByte(byte));
}

//add
void StringBridge::addBit(const char *name, bool value) {
    uint8_t len = 0;
    if(curBitByte < 0){
        curBitByte = currentIndex;
        len = 1;
    }
    if(add(name, len, curBitByte, curBit, SL_BIT)){
        if(len == 0){
            dataSetBit(curBitByte, curBit, value);
        } else {
            data->addByte(value);
        }
    }
    curBit++;
    if(curBit > 7){
        curBit = 0;
        curBitByte = -1;
    }
}
void StringBridge::addBit(const char *name) {
    addBit(name, false);
}
void StringBridge::addByte(const char *name, uint8_t value) {
    if(add(name, 1, currentIndex, 0, SL_BYTE)){
        data->addByte(value);
    }
}
void StringBridge::addByte(const char *name) {
    addByte(name, 0);
}
void StringBridge::addWord(const char *name, uint16_t value) {
    if(add(name, 2, currentIndex, 0, SL_WORD)){
        data->addWord(value);
    }
}
void StringBridge::addWord(const char *name) {
    addWord(name, 0);
}
void StringBridge::addDWord(const char *name, uint32_t value) {
    if(add(name, 4, currentIndex, 0, SL_DWORD)){
        data->addDWord(value);
    }
}
void StringBridge::addDWord(const char *name) {
    addDWord(name, 0);
}
void StringBridge::addFloat(const char *name, float value) {
    if(add(name, 4, currentIndex, 0, SL_FLOAT)){
        data->addFloat(value);
    }
}
void StringBridge::addFloat(const char *name) {
    addFloat(name, 0.0);
}

//get
bool StringBridge::getBit(const char *name) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_BIT) {
        return (data->getByte(stringListIndex->byte) & (1 << stringListIndex->bit)) > 0;
    }
    return false;
}
uint8_t StringBridge::getByte(const char *name) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_BYTE) {
        return data->getByte(stringListIndex->byte);
    }
    return 0;
}

uint16_t StringBridge::getWord(const char *name) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_WORD) {
        return data->getWord(stringListIndex->byte);
    }
    return 0;
}
uint32_t StringBridge::getDWord(const char *name) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_DWORD) {
        return data->getDWord(stringListIndex->byte);
    }
    return 0;
}
float StringBridge::getFloat(const char *name) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_FLOAT) {
        return data->getFloat(stringListIndex->byte);
    }
    return 0.0;
}

//set
void StringBridge::setBit(const char *name, bool value) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_BIT) {
        dataSetBit(stringListIndex->byte, stringListIndex->bit, value);
    }
}
void StringBridge::setByte(const char *name, uint8_t value) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_BYTE) {
        data->setByte(stringListIndex->byte, value);
    }
}
void StringBridge::setWord(const char *name, uint16_t value) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_WORD) {
        data->setWord(stringListIndex->byte, value);
    }
}
void StringBridge::setDWord(const char *name, uint32_t value) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_DWORD) {
        data->setDWord(stringListIndex->byte, value);
    }
}
void StringBridge::setFloat(const char *name, float value) {
    StringBridgeIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_FLOAT) {
        data->setFloat(stringListIndex->byte, value);
    }
}

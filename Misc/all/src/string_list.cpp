#include <cstring>
#include "string_list.h"

StringListIndex::StringListIndex(const char *name, uint16_t byte, uint8_t bit, StringListType type):
                                                        name(name), byte(byte), bit(bit), type(type) {}

StringList::StringList(uint16_t size) {
    data = new Buffer(size);
    keys = new ArrayList<StringListIndex*>(nullptr);
}

StringList::~StringList() {
    for(uint16_t i = 0; i < keys->size(); i++){
        delete keys->get(i);
    }
    delete data;
    delete keys;
}

StringListIndex *StringList::getCoordinate(const char *name) {
    for(uint16_t i = 0; i < keys->size(); i++){
        StringListIndex *stringListIndex = keys->get(i);
        if(strcmp(stringListIndex->name, name) == 0){
            return stringListIndex;
        }
    }
    return nullptr;
}
bool StringList::add(const char *name, uint8_t len, uint16_t byte, uint8_t bit, StringListType type) {
    if(currentIndex + len > data->getBufferSize()) return false;
    keys->add(new StringListIndex(name, byte, bit, type));
    currentIndex += len;
    return true;
}
void StringList::dataSetBit(uint16_t byte, uint8_t pos, bool value) {
    data->setByte(byte, ~(1 << pos) & data->getByte(byte));
    data->setByte(byte, (value << pos) | data->getByte(byte));
}

//add
void StringList::addBit(const char *name, bool value) {
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
void StringList::addByte(const char *name, uint8_t value) {
    if(add(name, 1, currentIndex, 0, SL_BYTE)){
        data->addByte(value);
    }
}
void StringList::addWord(const char *name, uint16_t value) {
    if(add(name, 2, currentIndex, 0, SL_WORD)){
        data->addWord(value);
    }
}
void StringList::addDWord(const char *name, uint32_t value) {
    if(add(name, 4, currentIndex, 0, SL_DWORD)){
        data->addDWord(value);
    }
}
void StringList::addFloat(const char *name, float value) {
    if(add(name, 4, currentIndex, 0, SL_FLOAT)){
        data->addFloat(value);
    }
}

//get
bool StringList::getBit(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_BIT) {
        return (data->getByte(stringListIndex->byte) & (1 << stringListIndex->bit)) > 0;
    }
    return false;
}
uint8_t StringList::getByte(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_BYTE) {
        return data->getByte(stringListIndex->byte);
    }
    return 0;
}

uint16_t StringList::getWord(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_WORD) {
        return data->getWord(stringListIndex->byte);
    }
    return 0;
}
uint32_t StringList::getDWord(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_DWORD) {
        return data->getDWord(stringListIndex->byte);
    }
    return 0;
}
float StringList::getFloat(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_FLOAT) {
        return data->getFloat(stringListIndex->byte);
    }
    return 0.0;
}

//set
void StringList::setBit(const char *name, bool value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_BIT) {
        dataSetBit(stringListIndex->byte, stringListIndex->bit, value);
    }
}
void StringList::setByte(const char *name, uint8_t value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_BYTE) {
        data->setByte(stringListIndex->byte, value);
    }
}
void StringList::setWord(const char *name, uint16_t value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_WORD) {
        data->setWord(stringListIndex->byte, value);
    }
}
void StringList::setDWord(const char *name, uint32_t value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_DWORD) {
        data->setDWord(stringListIndex->byte, value);
    }
}
void StringList::setFloat(const char *name, float value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->type == SL_FLOAT) {
        data->setFloat(stringListIndex->byte, value);
    }
}

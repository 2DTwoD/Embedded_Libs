#include <cstring>
#include "string_list.h"

StringListIndex::StringListIndex(const char *name, uint16_t byte, uint8_t bit, uint8_t len) : name(name), byte(byte),
                                                                                              bit(bit), len(len) {}

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
bool StringList::add(const char *name, uint8_t len) {
    if(currentIndex + len > data->getBufferSize()) return false;
    keys->add(new StringListIndex(name, currentIndex, 0, len));
    currentIndex += len;
    return true;
}

//add
void StringList::addByte(const char *name, uint8_t value) {
    if(add(name, 1)){
        data->addByte(value);
    }
}
void StringList::addWord(const char *name, uint16_t value) {
    if(add(name, 2)){
        data->addWord(value);
    }
}
void StringList::addDWord(const char *name, uint32_t value) {
    if(add(name, 4)){
        data->addDWord(value);
    }
}
void StringList::addFloat(const char *name, float value) {
    if(add(name, 4)){
        data->addFloat(value);
    }
}

//get
uint8_t StringList::getByte(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->len == 1) {
        return data->getByte(stringListIndex->byte);
    }
    return 0;
}

uint8_t StringList::getWord(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->len == 2) {
        return data->getWord(stringListIndex->byte);
    }
    return 0;
}
uint8_t StringList::getDWord(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->len == 4) {
        return data->getDWord(stringListIndex->byte);
    }
    return 0;
}
uint8_t StringList::getFloat(const char *name) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->len == 4) {
        return data->getFloat(stringListIndex->byte);
    }
    return 0.0;
}

//set
void StringList::setByte(const char *name, uint8_t value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->len == 1) {
        data->setByte(stringListIndex->byte, value);
    }
}
void StringList::setWord(const char *name, uint16_t value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->len == 2) {
        data->setWord(stringListIndex->byte, value);
    }
}
void StringList::setDWord(const char *name, uint32_t value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->len == 4) {
        data->setDWord(stringListIndex->byte, value);
    }
}
void StringList::setFloat(const char *name, float value) {
    StringListIndex *stringListIndex = getCoordinate(name);
    if(stringListIndex != nullptr && stringListIndex->len == 4) {
        data->setFloat(stringListIndex->byte, value);
    }
}

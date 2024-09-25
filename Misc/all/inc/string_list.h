#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <cstdint>
#include "array_list.h"
#include "buffer.h"

enum StringListType{
    SL_BIT, SL_BYTE, SL_WORD, SL_DWORD, SL_FLOAT
};

struct StringListIndex{
    StringListIndex(const char *name, uint16_t byte, uint8_t bit, StringListType type);

    const char *name{""};
    uint16_t byte{0};
    uint8_t bit{0};
    StringListType type;
};

class StringList{
private:
    Buffer *data;
    ArrayList<StringListIndex*> *keys;
    uint16_t currentIndex{0};
    int32_t curBitByte{-1};
    uint8_t curBit{0};
    StringListIndex* getCoordinate(const char *name);
    bool add(const char* name, uint8_t len, uint16_t byte, uint8_t bit, StringListType type);
    void dataSetBit(uint16_t byte, uint8_t pos, bool value);
public:
    explicit StringList(uint16_t size);
    virtual ~StringList();
    //add
    void addBit(const char *name, bool value);
    void addByte(const char *name, uint8_t value);
    void addWord(const char *name, uint16_t value);
    void addDWord(const char *name, uint32_t value);
    void addFloat(const char *name, float value);
    //get
    bool getBit(const char *name);
    uint8_t getByte(const char *name);
    uint16_t getWord(const char *name);
    uint32_t getDWord(const char *name);
    float getFloat(const char *name);
    //set
    void setBit(const char *name, bool value);
    void setByte(const char *name, uint8_t value);
    void setWord(const char *name, uint16_t value);
    void setDWord(const char *name, uint32_t value);
    void setFloat(const char *name, float value);
};

#endif //STRING_LIST_H

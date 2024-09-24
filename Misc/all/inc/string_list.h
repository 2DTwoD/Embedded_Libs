#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <cstdint>
#include "array_list.h"
#include "buffer.h"

struct StringListIndex{
    StringListIndex(const char *name, uint16_t byte, uint8_t bit, uint8_t len);

    const char *name{""};
    uint16_t byte{0};
    uint8_t bit{0};
    uint8_t len{0};
};

class StringList{
private:
    Buffer *data;
    ArrayList<StringListIndex*> *keys;
    uint16_t currentIndex{0};
    StringListIndex* getCoordinate(const char *name);
    bool add(const char* name, uint8_t len);
public:
    explicit StringList(uint16_t size);
    virtual ~StringList();
    //add
    void addByte(const char *name, uint8_t value);
    void addWord(const char *name, uint16_t value);
    void addDWord(const char *name, uint32_t value);
    void addFloat(const char *name, float value);
    //get
    uint8_t getByte(const char *name);
    uint8_t getWord(const char *name);
    uint8_t getDWord(const char *name);
    uint8_t getFloat(const char *name);
    //set
    void setByte(const char *name, uint8_t value);
    void setWord(const char *name, uint16_t value);
    void setDWord(const char *name, uint32_t value);
    void setFloat(const char *name, float value);
};

#endif //STRING_LIST_H

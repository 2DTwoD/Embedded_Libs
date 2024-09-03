#ifndef LOGGER_H
#define LOGGER_H

#include <cstdint>
#include "array_fun.h"
#include "math_fun.h"

struct Message{
    uint16_t num;
    uint16_t id;
    const char* description;
};

class MemoryLogger{
private:
    Message* history;
    uint16_t counter{0};
    uint16_t size{0};
    void addMessage(Message message);
public:
    explicit MemoryLogger(uint16_t size);
    ~MemoryLogger();
    void newMessage(uint16_t id, const char* description);
};
#endif //LOGGER_H

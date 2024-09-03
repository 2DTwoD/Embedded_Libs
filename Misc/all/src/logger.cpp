#include "logger.h"


MemoryLogger::MemoryLogger(uint16_t size) {
    this->size = max(size, (uint16_t)1);
    history = new Message[size];
}

MemoryLogger::~MemoryLogger() {
    delete[] history;
}

void MemoryLogger::newMessage(uint16_t id, const char *description) {
    Message message = {counter++, id, description};
    addMessage(message);
}

void MemoryLogger::addMessage(Message message) {
    insertElementInArray(history, size, (uint16_t)0, message);
}

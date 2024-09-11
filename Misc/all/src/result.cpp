#include "result.h"

Result ResultBuilder::getResult(ResultLabel label, uint8_t id, const char *const description) {
    return {label, id, description};
}

Result ResultBuilder::getOK() {
    return getResult(rOK, 0, "OK");
}

Result ResultBuilder::getOK(uint8_t id) {
    return getResult(rOK, id, "OK");
}

Result ResultBuilder::getOK(const char *const text) {
    return getResult(rOK, 0, text);
}

Result ResultBuilder::getOK(uint8_t id, const char *const text) {
    return getResult(rOK, id, text);
}

Result ResultBuilder::getError() {
    return getResult(rERROR, 0, "Error");
}

Result ResultBuilder::getError(uint8_t id) {
    return getResult(rERROR, id, "Error");
}

Result ResultBuilder::getError(const char *const text) {
    return getResult(rERROR, 0, text);
}

Result ResultBuilder::getError(uint8_t id, const char *const text) {
    return getResult(rERROR, id, text);
}

Result ResultBuilder::getInfo() {
    return getResult(rINFO, 0, "Info");
}

Result ResultBuilder::getInfo(uint8_t id) {
    return getResult(rINFO, id, "Info");
}

Result ResultBuilder::getInfo(const char *const text) {
    return getResult(rINFO, 0, text);
}

Result ResultBuilder::getInfo(uint8_t id, const char *const text) {
    return getResult(rINFO, id, text);
}

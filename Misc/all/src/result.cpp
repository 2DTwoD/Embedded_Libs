#include "result.h"
#include "array_fun.h"

//Result
bool Result::operator==(ResultLabel lb) const{
    return label == lb;
}

bool Result::operator!=(ResultLabel lb) const{
    return label != lb;
}

Result& Result::operator=(Result res) {
    this->label = res.label;
    //strcpy(this->description, res.description);
    copyStr(this->description, res.description);
    this->id = res.id;
    return *this;
}

//ResultBuilder
Result ResultBuilder::getResult(ResultLabel label, uint8_t id, const char* description) {
    return {label, id, description};
}

Result ResultBuilder::getOK() {
    return getResult(rOK, 0, "OK");
}

Result ResultBuilder::getOK(uint8_t id) {
    return getResult(rOK, id, "OK");
}

Result ResultBuilder::getOK(const char* text) {
    return getResult(rOK, 0, text);
}

Result ResultBuilder::getOK(uint8_t id, const char* text) {
    return getResult(rOK, id, text);
}

Result ResultBuilder::getError() {
    return getResult(rERROR, 0, "Error");
}

Result ResultBuilder::getError(uint8_t id) {
    return getResult(rERROR, id, "Error");
}

Result ResultBuilder::getError(const char* text) {
    return getResult(rERROR, 0, text);
}

Result ResultBuilder::getError(uint8_t id, const char* text) {
    return getResult(rERROR, id, text);
}

Result ResultBuilder::getInfo() {
    return getResult(rINFO, 0, "Info");
}

Result ResultBuilder::getInfo(uint8_t id) {
    return getResult(rINFO, id, "Info");
}

Result ResultBuilder::getInfo(const char* text) {
    return getResult(rINFO, 0, text);
}

Result ResultBuilder::getInfo(uint8_t id, const char* text) {
    return getResult(rINFO, id, text);
}

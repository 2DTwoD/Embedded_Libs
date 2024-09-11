#ifndef RESULT_H
#define RESULT_H

#include <cstdint>

enum  ResultLabel{
    rOK = 0, rERROR = 1, rINFO = 2, rDUMMY = 3
};

struct Result{
    ResultLabel label;
    uint8_t id;
    const char *const description;
};

namespace ResultBuilder{
    Result getResult(ResultLabel label, uint8_t id, const char *const description);
    Result getOK();
    Result getOK(uint8_t id);
    Result getOK(const char *const text);
    Result getOK(uint8_t id, const char *const text);
    Result getError();
    Result getError(uint8_t id);
    Result getError(const char *const text);
    Result getError(uint8_t id, const char *const text);
    Result getInfo();
    Result getInfo(uint8_t id);
    Result getInfo(const char *const text);
    Result getInfo(uint8_t id, const char *const text);
};

#endif //RESULT_H

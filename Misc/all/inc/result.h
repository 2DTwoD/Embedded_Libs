#ifndef RESULT_H
#define RESULT_H

#include <cstdint>
#include <cstring>

enum  ResultLabel{
    rOK = 0, rERROR = 1, rINFO = 2, rDUMMY = 3
};

struct Result{
    ResultLabel label;
    uint8_t id;
    char *const description;

    bool operator==(ResultLabel lb) const;
    bool operator!=(ResultLabel lb) const;
    Result& operator=(Result lb);
};

template<typename T>
struct ResultV: public Result{
    T value;
    ResultV(Result result, T value) : Result(result), value(value) {
    }
    T getValue(){
        return value;
    }
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

    //VResult
    template <typename T>
    ResultV<T> getResultV(ResultLabel label, uint8_t id, const char *const description, T value){
        return ResultV<T>(getResult(label, id, (char*)description), value);
    }
    template <typename T>
    ResultV<T> getOKv(T value){
        return ResultV<T>(getOK(), value);
    }
    template <typename T>
    ResultV<T> getOKv(uint8_t id, T value){
        return ResultV<T>(getOK(id), value);
    }
    template <typename T>
    ResultV<T> getOKv(const char* text, T value){
        return ResultV<T>(getOK(text), value);
    }
    template <typename T>
    ResultV<T> getOKv(uint8_t id, const char* text, T value){
        return ResultV<T>(getOK(id, text), value);
    }
    template <typename T>
    ResultV<T> getErrorV(T value){
        return ResultV<T>(getError(), value);
    }
    template <typename T>
    ResultV<T> getErrorV(uint8_t id, T value){
        return ResultV<T>(getError(id), value);
    }
    template <typename T>
    ResultV<T> getErrorV(const char* text, T value){
        return ResultV<T>(getError(text), value);
    }
    template <typename T>
    ResultV<T> getErrorV(uint8_t id, const char* text, T value){
        return ResultV<T>(getError(id, text), value);
    }
    template <typename T>
    ResultV<T> getInfoV(T value){
        return ResultV<T>(getInfo(), value);
    }
    template <typename T>
    ResultV<T> getInfoV(uint8_t id, T value){
        return ResultV<T>(getInfo(id), value);
    }
    template <typename T>
    ResultV<T> getInfoV(const char* text, T value){
        return ResultV<T>(getInfo(text), value);
    }
    template <typename T>
    ResultV<T> getInfoV(uint8_t id, const char* text, T value){
        return ResultV<T>(getInfo(id, text), value);
    }
};

#endif //RESULT_H

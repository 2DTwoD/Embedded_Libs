#ifndef ARRAY_WORK_H
#define ARRAY_WORK_H

#include <cstdint>

template<typename T>
void copyArrays(const T *const src, T *const dst, uint8_t len){
    for(int i = 0; i < len; i++){
        dst[i] = src[i];
    }
}

template<typename T>
void fillArray(T *const array, uint32_t len, T fillValue){
    for(int i = 0; i < len; i++){
        array[i] = fillValue;
    }
}

template<typename T, typename X>
void insertElementInArray(T *const array, X len, X index, T value){
    if(index > len) return;
    for(int i = len - 1; i > index; i--){
        array[i] = array[i - 1];
    }
    array[index] = value;
}

template<typename T, typename X>
void deleteElementInArray(T *const array, X len, X start, X quantity, T endValue){
    if(start + quantity >= len) return;
    for(int i = start; i < len; i++){
        if(i + quantity < len) {
            array[i] = array[i + quantity];
        } else {
            array[i] = endValue;
        }
    }
}

template<typename T, typename X>
void getPartOfArray(T *const src, X srcLen, X start, X quantity, T *const dst){
    if(start > srcLen || start + quantity > srcLen) return;
    for(int i = 0; i < quantity; i++){
        dst[i] = src[i + start];
    }
}

template<typename T>
T getRange(const T *const limits){
    return limits[1] - limits[0];
}

#endif //ARRAY_WORK_H
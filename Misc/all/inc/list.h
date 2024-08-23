#ifndef LIST_H
#define LIST_H

#include <cstdint>
#include "math_fun.h"
#include "array_fun.h"

template<typename T>
class List{
private:
    uint8_t increase;
    uint16_t totalSize;
    uint16_t curIndex{0};
    T* array{nullptr};

    void extend(){
        totalSize += increase;
        T* newArray = new T[totalSize];
        copyArrays(array, newArray, size());
        delete[] array;
        array = newArray;
    }

public:
    explicit List(uint8_t increaseStep) {
        increase = max((uint8_t )1, increaseStep);
        totalSize = increase;
        array = new T[totalSize];
    }

    List(const T* const src, uint16_t len, uint8_t increaseStep): List(increaseStep) {
        add(src, len);
    }

    List(const T* const src, uint16_t len): List(src, len, 10) {
    }

    List(): List(10){
    }

    uint16_t size() const {
        return curIndex;
    }

    bool isEmpty() const {
        return curIndex == 0;
    }

    T get(uint16_t index){
        if(isEmpty()) return (T)0;
        index = min(index, (uint16_t )(size() - 1));
        return array[index];
    }

    T get(){
        return get(size() - 1);
    }

    void add(uint16_t index, T value){
        index = min(size(), index);
        if(size() >= totalSize){
            extend();
        }
        insertElementInArray(array, totalSize, index, value);
        curIndex++;
    }

    void add(T value){
        add(size(), value);
    }

    void add(const T* const src, uint16_t start, uint16_t quantity, uint16_t index){
        for(uint16_t i = 0; i < quantity; i++){
            add(index + i, src[i + start]);
        }
    }

    void add(const T* const src, uint16_t len, uint16_t index){
        add(src, 0, len, index);
    }

    void add(const T* const src, uint16_t len){
        add(src, 0, len, size());
    }

    void remove(uint16_t index, uint16_t quantity){
        if(index >= size()) return;
        quantity = min(quantity, (uint16_t )(size() - index));
        deleteElementInArray(array, size(), index, quantity, 0);
        curIndex -= quantity;
    }

    void remove(uint16_t index){
        remove(index, 1);
    }

    void remove(){
        remove(size() - 1);
    }

    int32_t firstIndexOf(T value){
        for(uint16_t i = 0; i < size(); i++){
            if(array[i] == value) return i;
        }
        return -1;
    }

    int32_t lastIndexOf(T value){
        for(int32_t i = size() - 1; i >= 0; i--){
            if(array[i] == value) return i;
        }
        return -1;
    }

    void copyTo(T* dst, uint16_t len){
        copyArrays(array, dst, len);
    }

    void copyTo(T* dst){
        copyTo(dst, size());
    }

    void clear(){
        curIndex = 0;
    }

    void fill(T value){
        fillArray(array, size(), value);
    }

    ~List(){
        delete[] array;
    }
};

#endif //LIST_H

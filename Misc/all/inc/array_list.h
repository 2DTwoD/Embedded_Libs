#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <cstdint>
#include "math_fun.h"
#include "array_fun.h"
#include "ilist.h"

template<typename T>
class ArrayList: public IList<T>{
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
    using IList<T>::get;
    using IList<T>::set;
    using IList<T>::add;
    using IList<T>::remove;
    using IList<T>::copyTo;

    explicit ArrayList(uint8_t increaseStep): IList<T>() {
        increase = max((uint8_t )1, increaseStep);
        totalSize = increase;
        array = new T[totalSize];
    }

    ArrayList(const T* const src, uint16_t len, uint8_t increaseStep): ArrayList(increaseStep) {
        IList<T>::add(src, len);
    }

    ArrayList(const T* const src, uint16_t len): ArrayList(src, len, 10) {}

    ArrayList(): ArrayList(10){}

    uint16_t size() const override {
        return curIndex;
    }

    bool isEmpty() const override {
        return curIndex == 0;
    }

    T get(uint16_t index) const override {
        if(isEmpty()) return (T)0;
        index = min(index, (uint16_t )(size() - 1));
        return array[index];
    }

    void set(uint16_t index, T value) {
        if(isEmpty()) return;
        index = min(size() - 1, index);
        array[index] = value;
    };

    void add(uint16_t index, T value) override {
        index = min(size(), index);
        if(size() >= totalSize){
            extend();
        }
        insertElementInArray(array, totalSize, index, value);
        curIndex++;
    }

    void remove(uint16_t index, uint16_t quantity) override {
        if(index >= size()) return;
        quantity = min(quantity, (uint16_t )(size() - index));
        deleteElementInArray(array, size(), index, quantity, (T)0);
        curIndex -= quantity;
    }

    int32_t firstIndexOf(T value) const override {
        for(uint16_t i = 0; i < size(); i++){
            if(array[i] == value) return i;
        }
        return -1;
    }

    int32_t lastIndexOf(T value) const override {
        for(int32_t i = size() - 1; i >= 0; i--){
            if(array[i] == value) return i;
        }
        return -1;
    }

    void copyTo(T* dst, uint16_t len) const override {
        copyArrays(array, dst, len);
    }


    void clear() override {
        curIndex = 0;
    }

    void fill(T value) override {
        fillArray(array, size(), value);
    }

    ~ArrayList(){
        delete[] array;
    }
};

#endif //ARRAY_LIST_H

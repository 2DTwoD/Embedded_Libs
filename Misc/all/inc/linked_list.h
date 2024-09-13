#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "ilist.h"
#include "math_fun.h"

template<typename T>
class LLEntity{
private:
    LLEntity<T>* prev;
    LLEntity<T>* next;
    T value;
public:
    LLEntity(LLEntity<T> *prev, LLEntity<T> *next, T value) : prev(prev), next(next), value(value) {}
    LLEntity<T>* getPrev(){
        return prev;
    }
    LLEntity<T>* getNext(){
        return next;
    }
    T getValue(){
        return value;
    }
    void setPrev(LLEntity<T>* val){
        prev = val;
    }
    void setNext(LLEntity<T>* val){
        next = val;
    }
    void setValue(T val){
        value = val;
    }
    bool hasNext(){
        return next != nullptr;
    }
    bool hasPrev(){
        return prev != nullptr;
    }
};

template<typename Callable>
void DoSomething(Callable c) { c(); }

template<typename T>
class LinkedList: public IList<T>{
private:
    LLEntity<T>* first{nullptr};
    LLEntity<T>* last{nullptr};
    uint16_t lastIndex{0};
    LLEntity<T>* newEntity(LLEntity<T>* prev, LLEntity<T>* next, T value){
        lastIndex++;
        LLEntity<T>* entity = new LLEntity<T>(prev, next, value);
        if(prev == nullptr || next == nullptr){
            last = entity;
        }
        return entity;
    }
    void deleteEntity(LLEntity<T>* entity){
        if(entity == nullptr) return;
        if(entity->getNext() == nullptr){
            last = entity->getPrev();
        }
        delete entity;
        lastIndex--;
    }
public:
    using IList<T>::get;
    using IList<T>::add;
    using IList<T>::remove;
    using IList<T>::copyTo;
    LinkedList(): IList<T>() {
    }

    ~LinkedList() {
        clear();
    }

    uint16_t size() const override {
        return lastIndex;
    }

    bool isEmpty() const override {
        return lastIndex == 0;
    }

    T get(uint16_t index) const override {
        if(isEmpty()) return (T)0;
        LLEntity<T>* cur = first;
        uint16_t count = 0;
        do{
            if(count == index){
                break;
            }
            count++;
            cur = cur->getNext();
        } while(cur != nullptr || cur->hasNext());
        return cur->getValue();
    }

    void add(uint16_t index, T value) override {
        if(first == nullptr){
            first = newEntity(nullptr, nullptr, value);
            return;
        }
        LLEntity<T>* cur = first;
        uint16_t count = 0;
        do{
            if(cur->getNext() == nullptr || count == index) break;
            count++;
            cur = cur->getNext();
        } while(cur->hasNext());
        cur->setNext(newEntity(cur, cur->getNext(), value));
    }

    void remove(uint16_t index, uint16_t quantity) override {
        if(isEmpty()){
            return;
        }
        LLEntity<T>* cur = first;
        uint16_t count = 0;
        do{
            if(count == index){
                for(uint16_t i = 0; i < quantity; i++){
                    if(cur == nullptr) return;
                    LLEntity<T>* next = cur->getNext();
                    deleteEntity(cur);
                    cur = next;
                }
                break;
            }
            count++;
            cur = cur->getNext();
        } while (cur != nullptr || cur->hasNext());
    }

    int32_t firstIndexOf(T value) const override {
        if(isEmpty()) return -1;
        LLEntity<T>* cur = first;
        uint16_t count = 0;
        do{
            if(cur->getValue() == value){
                return count;
            }
            count++;
            cur = cur->getNext();
        } while(cur != nullptr || cur->hasNext());
        return -1;
    }

    int32_t lastIndexOf(T value) const override {
        if(isEmpty()) return -1;
        LLEntity<T>* cur = last;
        uint16_t count = lastIndex - 1;
        do{
            if(cur->getValue() == value){
                return count;
            }
            count--;
            cur = cur->getPrev();
        } while(cur != nullptr || cur->hasPrev());
        return -1;
    }

    void copyTo(T *dst, uint16_t len) const override {
        LLEntity<T>* cur = first;
        for(uint16_t i = 0; i < len; i++){
            if(cur == nullptr) return;
            dst[i] = cur->getValue();
            cur = cur->getNext();
        }
    }

    void clear() override {
        if(isEmpty()){
            return;
        }
        LLEntity<T>* cur = first;
        LLEntity<T>* next;
        do{
            next = cur->getNext();
            delete cur;
            cur = next;
        } while(cur != nullptr || cur->hasNext());
        first = nullptr;
        last = nullptr;
        lastIndex = 0;
    }

    void fill(T value) override {
        LLEntity<T>* cur = first;
        do{
            cur->setValue(value);
            cur = cur->getNext();
        } while(cur != nullptr || cur->hasNext());
    }
};

#endif //LINKED_LIST_H

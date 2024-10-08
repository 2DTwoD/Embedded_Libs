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

template<typename T>
class LinkedList: public IList<T>{
private:
    LLEntity<T>* first{nullptr};
    LLEntity<T>* last{nullptr};
    uint16_t lastIndex{0};
    T zeroValue;

    void newEntity(LLEntity<T>* prev, LLEntity<T>* next, T value){
        lastIndex++;
        LLEntity<T>* entity = new LLEntity<T>(prev, next, value);
        if(prev == nullptr){
            first = entity;
        } else {
            prev->setNext(entity);
        }
        if(next == nullptr){
            last = entity;
        } else {
            next->setPrev(entity);
        }
    }

    void deleteEntity(LLEntity<T>* entity){
        if(entity == nullptr) return;
        if(entity->getPrev() == nullptr){
            first = entity->getNext();
            if(first != nullptr) {
                first->setPrev(nullptr);
            }
        } else {
            entity->getPrev()->setNext(entity->getNext());
        }
        if(entity->getNext() == nullptr){
            last = entity->getPrev();
            if(last != nullptr){
                last->setNext(nullptr);
            }
        } else {
            entity->getNext()->setPrev(entity->getPrev());
        }
        delete entity;
        lastIndex--;
    }
public:
    using IList<T>::get;
    using IList<T>::set;
    using IList<T>::add;
    using IList<T>::remove;
    using IList<T>::grab;
    using IList<T>::copyTo;

    explicit LinkedList(T zeroValue): zeroValue(zeroValue){};

    LinkedList(T zeroValue, T value): LinkedList(zeroValue) {
        add(value);
    }

    LinkedList(T zeroValue, const T* const src, uint16_t len): LinkedList(zeroValue) {
        add(src, len);
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
        if(isEmpty()) return zeroValue;
        LLEntity<T>* cur = first;
        uint16_t count = 0;
        while(cur != nullptr){
            if(count == index){
                return cur->getValue();
            }
            count++;
            cur = cur->getNext();
        }
        return last->getValue();
    }

    void set(uint16_t index, T value) override{
        if(isEmpty() || index >= size()) return;
        if(index < size() / 2){
            LLEntity<T>* cur = first;
            uint16_t count = 0;
            while(cur != nullptr){
                if(count == index){
                    cur->setValue(value);
                    return;
                }
                count++;
                cur = cur->getNext();
            }
        } else {
            LLEntity<T>* cur = last;
            uint16_t count = size() - 1;
            while(cur != nullptr){
                if(count == index){
                    cur->setValue(value);
                    return;
                }
                count--;
                cur = cur->getPrev();
            }
        }
    };

    void add(uint16_t index, T value) override {
        if(first == nullptr){
            newEntity(nullptr, nullptr, value);
            return;
        }
        if(index == 0){
            newEntity(nullptr, first, value);
            return;
        } else if(index >= size()){
            newEntity(last, nullptr, value);
            return;
        }
        if(index < size() / 2){
            LLEntity<T>* cur = first;
            uint16_t count = 0;
            while(cur != nullptr){
                if(count == index){
                    newEntity(cur->getPrev(), cur, value);
                    return;
                }
                count++;
                cur = cur->getNext();
            }
        } else {
            LLEntity<T>* cur = last;
            uint16_t count = size() - 1;
            while(cur != nullptr){
                if(count == index){
                    newEntity(cur->getPrev(), cur, value);
                    return;
                }
                count--;
                cur = cur->getPrev();
            }
        }
    }

    void remove(uint16_t index, uint16_t quantity) override {
        if(isEmpty()){
            return;
        }
        LLEntity<T>* cur = first;
        uint16_t count = 0;
        while (cur != nullptr){
            if(count == index){
                LLEntity<T>* next;
                for(uint16_t i = 0; i < quantity; i++){
                    next = cur->getNext();
                    deleteEntity(cur);
                    if(next == nullptr) return;
                    cur = next;
                }
                break;
            }
            count++;
            cur = cur->getNext();
        }
    }

    int32_t firstIndexOf(T value) const override {
        if(isEmpty()) return -1;
        LLEntity<T>* cur = first;
        uint16_t count = 0;
        while(cur != nullptr){
            if(cur->getValue() == value){
                return count;
            }
            count++;
            cur = cur->getNext();
        }
        return -1;
    }

    int32_t lastIndexOf(T value) const override {
        if(isEmpty()) return -1;
        LLEntity<T>* cur = last;
        uint16_t count = size() - 1;
        while(cur != nullptr){
            if(cur->getValue() == value){
                return count;
            }
            count--;
            cur = cur->getPrev();
        }
        return -1;
    }

    void copyTo(T *dst, uint16_t start, uint16_t quantity) const override {
        LLEntity<T>* cur = first;
        uint16_t count = 0;
        while(cur != nullptr && count < (start + quantity)){
            if(count >= start) {
                dst[count - start] = cur->getValue();
            }
            cur = cur->getNext();
            count++;
        }
    }

    void clear() override {
        if(isEmpty()){
            return;
        }
        LLEntity<T>* cur = first;
        LLEntity<T>* next;
        while(cur != nullptr){
            next = cur->getNext();
            delete cur;
            cur = next;
        }
        first = nullptr;
        last = nullptr;
        lastIndex = 0;
    }

    void fill(T value) override {
        LLEntity<T>* cur = first;
        while(cur != nullptr){
            cur->setValue(value);
            cur = cur->getNext();
        }
    }
    void forEach(std::function<void(T)> lambda) const{
        if(isEmpty()) return;
        LLEntity<T>* cur = first;
        while(cur != nullptr){
            lambda(cur->getValue());
            cur = cur->getNext();
        }
    }

    void forEachModify(std::function<T(T)> lambda){
        if(isEmpty()) return;
        LLEntity<T>* cur = first;
        while(cur != nullptr){
            cur->setValue(lambda(cur->getValue()));
            cur = cur->getNext();
        }
    }
};

#endif //LINKED_LIST_H

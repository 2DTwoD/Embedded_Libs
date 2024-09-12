#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "ilist.h"
#include "math_fun.h"

template<typename T>
struct LLEntity{
    LLEntity<T>* prev;
    LLEntity<T>* next;
    T value;
    bool hasNext;
};

template<typename T>
class LinkedList: public IList<T>{
private:
    LLEntity<T>* first{nullptr};
    LLEntity<T>* last{nullptr};
    uint16_t lastIndex{0};

    void addEntity(LLEntity<T>* cur, T value){
        auto* newEntity = new LLEntity<T>{cur, cur->next, value, cur->hasNext};
        if(!newEntity->hasNext){
            last = newEntity;
        }
        cur->prev = newEntity;
        cur->hasNext = true;
    }
    /*void removeEntity(LLEntity<T>* cur, uint16_t quantity){
        LLEntity<T>* prev = cur->prev;
        LLEntity<T>* next{nullptr};
        for(uint16_t i = 0; i < quantity; i++){
            if(cur == nullptr){
                if(prev != nullptr) {
                    prev->next = nullptr;
                    last = prev;
                } else {
                    last =
                }
                break;
            }
            next = cur->next;
            delete cur;
            cur = next;
            lastIndex--;
        }
    }*/
public:
    using IList<T>::get;
    using IList<T>::add;
    using IList<T>::remove;
    using IList<T>::copyTo;
    LinkedList() {
    }

    ~LinkedList() {
        if(isEmpty()){
            return;
        }
        LLEntity<T>* current = first;
        LLEntity<T>* next{nullptr};
        while(current->hasNext){
            next = current->next;
            delete current;
            current = next;
        }
        delete current;
    }

    uint16_t size() const override {
        return lastIndex;
    }

    bool isEmpty() const override {
        return lastIndex == 0;
    }

    T get(uint16_t index) const override {
        if(isEmpty()) return (T)0;
        LLEntity<T>* entity = first;
        uint16_t count = 0;
        while(entity->hasNext){
            if(count == index) return entity->value;
            entity = entity->next;
            count++;
        }
        return entity->value;
    }

    void add(uint16_t index, T value) override {
        if(isEmpty()){
            first = new LLEntity<T>{nullptr, nullptr, value, false};
            last = first;
            return;
        }
        index = min(index, size());
        LLEntity<T>* entity = first;
        uint16_t count = 0;
        do{
            if(count == index){
                addEntity(entity, value);
                return;
            }
            entity = entity->next;
            count++;
        } while(entity->hasNext);
        addEntity(entity, value);
    }

    void remove(uint16_t index, uint16_t quantity) override {
        if(isEmpty()){
            return;
        }
        if(index == 0){
            removeEntity(first, quantity);
            return;
        }
        LLEntity<T>* entity = first;
        uint16_t count = 0;
        while(entity->hasNext){
            if(count == index - 1){
                removeEntity(entity->next, quantity);
                return;
            }
            entity = entity->next;
            count++;
        }
    }

    int32_t firstIndexOf(T value) const override {
        LLEntity<T>* entity = first;
        uint16_t count = 0;
        while(entity->hasNext){
            if(entity->value == value){
                return count;
            }
            entity = entity->next;
            count++;
        }
        if(entity->value == value){
            return count;
        }
    }

    int32_t lastIndexOf(T value) const override {
        return 0;
    }

    void copyTo(T *dst, uint16_t len) const override {

    }

    void clear() override {

    }

    void fill(T value) override {

    }
};
#endif //LINKED_LIST_H

#ifndef STRING_MAP_H
#define STRING_MAP_H
#include "array_list.h"

template<typename T>
struct StringMapIndex{
    const char *key;
    T value;
};

template<typename T>
struct SMIPair{
    StringMapIndex<T> *stringMapIndex;
    uint16_t num;
};

template<typename T>
class StringMap: private ArrayList<StringMapIndex<T>*>{
private:
    T zeroValue;
    SMIPair<T> find(const char *name){
        StringMapIndex<T> *stringMapIndex;
        uint16_t size = ArrayList<StringMapIndex<T>*>::size();
        for(uint16_t i = 0; i < size; i++){
            stringMapIndex = ArrayList<StringMapIndex<T>*>::carefulGet(i);
            if(strcmp(stringMapIndex->key, name) == 0){
                return {stringMapIndex, i};
            }
        }
        return {nullptr, 0};
    }
public:
    explicit StringMap(T zeroValue): ArrayList<StringMapIndex<T>*>(nullptr), zeroValue(zeroValue) {}
    ~StringMap() {
        clear();
    }

    void add(const char *name, T value){
        auto smiPair = find(name);
        if(smiPair.stringMapIndex == nullptr){
            auto *stringMapIndex = new StringMapIndex<T>{name, value};
            ArrayList<StringMapIndex<T>*>::add(stringMapIndex);
        }
    }

    void remove(const char *name){
        auto smiPair = find(name);
        if(smiPair.stringMapIndex != nullptr){
            ArrayList<StringMapIndex<T>*>::remove(smiPair.num);
            delete smiPair.stringMapIndex;
        }
    }

    void clear(){
        for(uint16_t i = 0; i < this->size(); i++){
            delete ArrayList<StringMapIndex<T>*>::carefulGet(i);
        }
        ArrayList<StringMapIndex<T>*>::clear();
    }

    T get(const char *name){
        auto smiPair = find(name);
        if(smiPair.stringMapIndex != nullptr){
            return smiPair.stringMapIndex->value;
        }
        return zeroValue;
    }

    void set(const char *name, T value){
        auto smiPair = find(name);
        if(smiPair.stringMapIndex != nullptr){
            smiPair.stringMapIndex->value = value;
        }
    }

    bool isExist(const char *name){
        auto smiPair = find(name);
        return smiPair.stringMapIndex != nullptr;
    }
};
#endif //STRING_MAP_H

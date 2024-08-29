#ifndef REGISTER_H
#define REGISTER_H

#include <cstdint>

template <typename T>
struct MaskVal {
    T mask;
    T value;
};

template<typename T>
int8_t getPosition(T mask){
    int8_t size = (int8_t)sizeof(T) * 8;
    for(int8_t i = 0; i < size; i++){
        if(mask & (1<<i)) return i;
    }
    return -1;
}

template<typename T>
bool getBit(T& reg, uint32_t pos){
    return (reg & (1 << pos)) > 0;
}

template<typename T>
void setBit(T& reg, uint32_t pos){
    reg |= (1 << pos);
}

template<typename T>
void resetBit(T& reg, uint32_t pos){
    reg &= ~(1 << pos);
}

template<typename T>
void toggleBit(T& reg, uint32_t pos){
    if(getBit(reg, pos)){
        resetBit(reg, pos);
        return;
    }
    setBit(reg, pos);
}

template<typename T>
void toggleBitTwice(T& reg, uint32_t pos){
    toggleBit(reg, pos);
    toggleBit(reg, pos);
}

template<typename T>
void setRegister(T& reg, uint32_t mask, uint32_t value){
    reg &= ~mask;
    reg |= value;
}

template<typename T>
void setRegister(T& reg, MaskVal<T> maskVal){
    setRegister(reg, maskVal.mask, maskVal.value);
}

template<typename T>
void setRegValShift(T& reg, uint32_t mask, uint32_t value){
    int8_t pos = getPosition(mask);
    if(pos < 0) return;
    reg &= ~mask;
    reg |= (value << pos);
}

template<typename T>
void setRegValShift(T& reg, MaskVal<T> maskVal){
    setRegValShift(reg, maskVal.mask, maskVal.value);
}

template<typename T>
class Register {
private:
    volatile T& reg;
    uint8_t size;

public:
    explicit Register(volatile T& reg): reg(reg) {
        size = (int8_t)sizeof(T) * 8;
    }

    void setBit(uint8_t pos){
        if(pos > size) return;
        reg |= (1 << pos);
    }

    void resetBit(uint8_t pos){
        if(pos > size) return;
        reg &= ~(1 << pos);
    }

    bool getBit(uint8_t pos){
        if(pos > size) return false;
        return (reg & (1 << pos)) > 0;
    }

    bool isZero(uint8_t pos){
        if(pos > size) return false;
        return getBit(pos) == false;
    }

    bool isOne(uint8_t pos){
        return getBit(pos) == true;
    }

    void setValue(T mask, uint32_t val){
        int8_t pos = getPosition(mask);
        if(pos < 0) return;
        reg &= ~mask;
        reg |= (val << pos);
    }

    T getValue(T mask){
        int8_t pos = getPosition(mask);
        if(pos < 0) return 0;
        return (reg & mask) >> pos;
    }

    Register<T>& operator=(T val){
        reg = val;
        return *this;
    }

    Register<T>& operator=(MaskVal<T> maskVal) {
        setValue(maskVal.mask, maskVal.value);
        return *this;
    }

    Register<T>& operator|=(uint8_t pos){
        setBit(pos);
        return *this;
    }

    Register<T>& operator^=(uint8_t pos){
        resetBit(pos);
        return *this;
    }
};
#endif //REGISTER_H

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
bool getBit(T& reg, uint32_t mask){
    return (reg & mask) > 0;
}

template<typename T>
bool bitIsZero(T& reg, uint32_t mask){
    return getBit(reg, mask) == false;
}

template<typename T>
bool bitIsOne(T& reg, uint32_t mask){
    return getBit(reg, mask) == true;
}

template<typename T>
void setBit(T& reg, uint32_t mask){
    reg |= mask;
}

template<typename T>
void resetBit(T& reg, uint32_t mask){
    reg &= ~mask;
}

template<typename T>
void toggleBit(T& reg, uint32_t mask){
    if(getBit(reg, mask)){
        resetBit(reg, mask);
        return;
    }
    setBit(reg, mask);
}

template<typename T>
void toggleBitTwice(T& reg, uint32_t mask){
    toggleBit(reg, mask);
    toggleBit(reg, mask);
}

template<typename T>
bool getBitByPos(T& reg, uint32_t pos){
    return (reg & (1 << pos)) > 0;
}

template<typename T>
bool bitIsZeroByPos(T& reg, uint32_t pos){
    return getBitByPos(reg, pos) == false;
}

template<typename T>
bool bitIsOneByPos(T& reg, uint32_t pos){
    return getBitByPos(reg, pos) == true;
}

template<typename T>
void setBitByPos(T& reg, uint32_t pos){
    reg |= (1 << pos);
}

template<typename T>
void resetBitByPos(T& reg, uint32_t pos){
    reg &= ~(1 << pos);
}

template<typename T>
void toggleBitByPos(T& reg, uint32_t pos){
    if(getBitByPos(reg, pos)){
        resetBitByPos(reg, pos);
        return;
    }
    setBitByPos(reg, pos);
}

template<typename T>
void toggleBitTwiceByPos(T& reg, uint32_t pos){
    toggleBitByPos(reg, pos);
    toggleBitByPos(reg, pos);
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

    void set(T mask){
        reg |= mask;
    }

    void reset(T mask){
        reg &= ~mask;
    }

    T get(T mask){
        return (reg & mask);
    }

    bool isZero(T mask){
        return getBit(mask) == 0;
    }

    bool isOne(T mask){
        return getBit(mask) > 0;
    }

    void setBitByPos(uint8_t pos){
        if(pos > size) return;
        reg |= (1 << pos);
    }

    void resetBitByPos(uint8_t pos){
        if(pos > size) return;
        reg &= ~(1 << pos);
    }

    bool getBitByPos(uint8_t pos){
        if(pos > size) return false;
        return (reg & (1 << pos)) > 0;
    }

    bool isZeroByPos(uint8_t pos){
        if(pos > size) return false;
        return getBitByPos(pos) == false;
    }

    bool isOneByPos(uint8_t pos){
        return getBitByPos(pos) == true;
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
        setBitByPos(pos);
        return *this;
    }

    Register<T>& operator^=(uint8_t pos){
        resetBitByPos(pos);
        return *this;
    }
};
#endif //REGISTER_H

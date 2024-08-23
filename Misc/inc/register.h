#ifndef REGISTER_H
#define REGISTER_H

template<typename T>
class Register {
private:
    volatile T& reg;
    uint8_t size;
    int8_t getPosition(T mask){
        for(uint8_t i = 0; i < size; i++){
            if(mask & (1<<i)) {
                return (int8_t) i;
            }
        }
        return -1;
    }
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
};
#endif //REGISTER_H

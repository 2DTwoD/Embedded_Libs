#ifndef COMMON_H
#define COMMON_H
#include <cstdint>

enum IMPULSE {
	RISE = 0,
	FALL,
	RISE_FALL
};

template <typename T>
struct MaskVal {
    T mask;
    T value;
};

void badTimeOut(uint32_t);

int8_t getPosition(volatile uint32_t mask);

template<typename T>
void setRegister(T * const reg, uint32_t mask, uint32_t value){
	*reg &= ~mask;
	*reg |= value;
}

template<typename T>
void setRegisterWithAutoShift(T * const reg, uint32_t mask, uint32_t value){
	int8_t pos = getPosition(mask);
	if(pos < 0) return;
	*reg &= ~mask;
	*reg |= (value << pos);
}

#endif //COMMON_H

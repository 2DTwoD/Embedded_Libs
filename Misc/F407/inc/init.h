#ifndef MISC_INIT_H
#define MISC_INIT_H
#include "stm32f407xx.h"
#include "common.h"

void commonInit();
void rccInit();
void tickInit(TIM_TypeDef* tim, uint16_t busFreqMHz);
void spiInit();
void i2cInit();

#endif //MISC_INIT_H
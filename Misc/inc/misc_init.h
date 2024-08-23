#ifndef MISC_INIT_H
#define MISC_INIT_H
#include "stm32f4xx.h"
#include "common.h"

void commonInit();
void rccInit();
void tickInit();
void spiInit();
void i2cInit();

#endif //MISC_INIT_H
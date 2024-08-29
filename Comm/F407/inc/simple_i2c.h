#ifndef SIMPLE_I2C_H
#define SIMPLE_I2C_H
#include "stm32f407xx.h"

#include "gpio_common.h"
#include "register.h"
#include "math_fun.h"

enum I2C_Mode{
    FM, SM
};

enum I2C_Duty{
    _2_1, _16_9
};

class SimpleI2C{
private:
    I2C_TypeDef* i2c;
    void adjustGPIO(GPIO_Info gpioInfo);
    void resetI2C();
public:
    SimpleI2C(I2C_TypeDef* i2c, GPIO_Info sda, GPIO_Info scl, uint8_t busFreqMHz,
              I2C_Mode mode, I2C_Duty duty);
};

#endif //SIMPLE_I2C_H

#ifndef SIMPLE_I2C_H
#define SIMPLE_I2C_H
#include "stm32f407xx.h"

#include "gpio_common.h"
#include "register.h"
#include "math_fun.h"
#include "array_fun.h"
#include "simple_input.h"
#include "on_delay.h"
#include "buffer.h"

enum I2C_Mode{
    FM, SM
};

enum I2C_Duty{
    _2_1, _16_9
};

class SimpleI2C: private OnDelayCommon, public IUpdated1ms, public Buffer {
private:
    I2C_TypeDef* i2c;
    GPIO_Info sda;
    GPIO_Info scl;
    void adjustGPIO(GPIO_Info gpioInfo);
    void resetI2C();
    void solveProblem();
public:
    SimpleI2C(I2C_TypeDef* i2c, GPIO_Info sda, GPIO_Info scl, uint8_t busFreqMHz,
              I2C_Mode mode, I2C_Duty duty, uint16_t bufferSize, uint32_t errorDelay);
    void getSlaves(uint8_t* const dst, uint8_t len);
    bool slaveIsExist(uint8_t address);
    void update1ms() override;
};

#endif //SIMPLE_I2C_H

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

enum I2C_RW{
    WRITE = 0, READ = 1
};

class SimpleI2C: private OnDelayCommon, public IUpdated1ms, public Buffer {
private:
    I2C_TypeDef* i2c;
    GPIO_Info sda;
    GPIO_Info scl;
    void adjustGPIO(GPIO_Info gpioInfo);
    void resetI2C();
    void solveProblem();
    bool commonRW(uint8_t address, I2C_RW rw, bool (*action)(SimpleI2C* ths, uint8_t *const src, uint8_t len),
                  uint8_t *const src, uint8_t len);
    bool startBus(uint8_t address, I2C_RW rw);
public:
    SimpleI2C(I2C_TypeDef* i2c, GPIO_Info sda, GPIO_Info scl, uint8_t busFreqMHz,
              I2C_Mode mode, I2C_Duty duty, uint16_t bufferSize, uint32_t errorDelay);
    bool slaveIsExist(uint8_t address);
    void getSlaves(uint8_t* const dst, uint8_t len);
    void transmit(uint8_t address, uint8_t *const src, uint16_t len);
    void transmit(uint8_t address, uint8_t value);
    void receive(uint8_t address, uint16_t len);
    void receive(uint8_t address);
    void update1ms() override;
};

#endif //SIMPLE_I2C_H

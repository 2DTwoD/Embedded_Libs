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

class SimpleI2C: private OnDelayCommon, public IUpdated1ms, public InternalBuffer {
private:
    I2C_TypeDef* i2c;
    GPIO_Info sda;
    GPIO_Info scl;
    void adjustGPIO(GPIO_Info gpioInfo);
    void resetI2C();
    void solveBusy();
    bool startBus(uint8_t address, I2C_RW rw);
    void stopBus();
    void resetAF();
    void resetPE();
    bool write(uint8_t address, uint8_t *const src, uint16_t len);
    bool read(uint8_t address, uint16_t len);
public:
    SimpleI2C(I2C_TypeDef* i2c, GPIO_Info sda, GPIO_Info scl, uint8_t busFreqMHz,
              I2C_Mode mode, I2C_Duty duty, uint16_t bufferSize, uint32_t errorDelay);
    void update1ms() override;
    bool slaveIsExist(uint8_t address);
    void getSlaves(uint8_t* const dst, uint8_t len);
    bool send(uint8_t address, uint8_t *const src, uint16_t len);
    bool send(uint8_t address, uint8_t value);
    bool receive(uint8_t address, uint16_t len);
    bool receive(uint8_t address);
    bool sendAndReceive(uint8_t address, uint8_t *const writeSrc, uint16_t writeLen, uint16_t readLen);
    bool sendAndReceive(uint8_t address, uint16_t writeValue, uint16_t readLen);
};

#endif //SIMPLE_I2C_H

#ifndef GPIO_COMMON_H
#define GPIO_COMMON_H
#include "stm32f407xx.h"
#include "register.h"
#include "init.h"
#include "math_fun.h"

typedef struct {
    volatile GPIO_TypeDef* gpio;
    uint8_t pin;
} GPIO_Info;

enum GPIOmoder{
    GPIO_MODER_INPUT = 0b00,
    GPIO_MODER_OUTPUT = 0b01,
    GPIO_MODER_AF = 0b10,
    GPIO_MODER_ANALOG = 0b11
};

enum GPIOotyper{
    GPIO_OTYPER_PUSH_PULL = 0,
    GPIO_OTYPER_OPEN_DRAIN = 1
};

enum GPIOospeedr{
    GPIO_OSPEEDR_LOW_SPEED = 0b00,
    GPIO_OSPEEDR_MED_SPEED = 0b01,
    GPIO_OSPEEDR_HIGH_SPEED = 0b10,
    GPIO_OSPEEDR_VHIGH_SPEED = 0b11
};

enum GPIOpupdr{
    GPIO_PUPDR_NO_PULL = 0b00,
    GPIO_PUPDR_PULL_UP = 0b01,
    GPIO_PUPDR_PULL_DOWN = 0b10
};
enum GPIOafr{
    GPIO_AFR0 = 0b0000,
    GPIO_AFR1 = 0b0001,
    GPIO_AFR2 = 0b0010,
    GPIO_AFR3 = 0b0011,
    GPIO_AFR4 = 0b0100,
    GPIO_AFR5 = 0b0101,
    GPIO_AFR6 = 0b0110,
    GPIO_AFR7 = 0b0111,
    GPIO_AFR8 = 0b1000,
    GPIO_AFR9 = 0b1001,
    GPIO_AFR10 = 0b1010,
    GPIO_AFR11 = 0b1011,
    GPIO_AFR12 = 0b1100,
    GPIO_AFR13 = 0b1101,
    GPIO_AFR14 = 0b1110,
    GPIO_AFR15 = 0b1111,
};

class GPIOconfig{
private:
    bool noErr{true};
    GPIO_Info gpioInfo;
    void init();
public:
    explicit GPIOconfig(const GPIO_Info &gpioInfo);
    bool noErrors();
    GPIOconfig& start();
    GPIOconfig& focus(GPIO_Info newGpioInfo);
    GPIOconfig& setMODER(GPIOmoder moder);
    GPIOconfig& setOTYPER(GPIOotyper otyper);
    GPIOconfig& setOSPEEDR(GPIOospeedr ospeedr);
    GPIOconfig& setPUPDR(GPIOpupdr pupdr);
    bool getIDR() const;
    GPIOconfig& setODR(bool value);
    bool getODR() const;
    GPIOconfig& setWithBSRR(bool value);
    GPIOconfig& setLCK(bool value);
    bool lockAllPortLCKK() const;
    GPIOconfig& setAFR(GPIOafr afr);
    void fin();
};

class GPIOcommon{
protected:
    GPIOconfig gpio;
public:
    explicit GPIOcommon(GPIO_Info gpioInfo);
};

#endif //GPIO_COMMON_H
#ifndef GPIO_COMMON_H
#define GPIO_COMMON_H
#include "stm32f407xx.h"

typedef struct {
    volatile GPIO_TypeDef* gpio;
    uint8_t pin;
} GPIO_Info;


class GPIOcommon{
	protected:
		uint8_t pin;
		volatile GPIO_TypeDef * gpio;
  public:
		explicit GPIOcommon(GPIO_Info gpioInfo);
};

#endif //GPIO_COMMON_H
#ifndef GPIO_COMMON_H
#define GPIO_COMMON_H
#include "stm32f103xb.h"

typedef struct {
    volatile GPIO_TypeDef* gpio;
    uint8_t pin;
} GPIO_Info;

class GPIOcommon{
	protected:
		uint8_t pin;
		GPIO_TypeDef * gpio;
  public:
		GPIOcommon(GPIO_TypeDef * gpio, uint8_t pin);
};

#endif //GPIO_COMMON_H
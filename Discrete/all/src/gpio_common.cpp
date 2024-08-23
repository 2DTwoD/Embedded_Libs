#include "gpio_common.h"

GPIOcommon::GPIOcommon(GPIO_TypeDef * gpio, uint8_t pin): gpio(gpio), pin(pin){
	if(pin > 15){
		pin = 0;
	}
	if(gpio == GPIOA){
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	} else if(gpio == GPIOB){
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	} else if(gpio == GPIOC){
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	} else if(gpio == GPIOD){
		RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	} else if(gpio == GPIOE){
		RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	}
}
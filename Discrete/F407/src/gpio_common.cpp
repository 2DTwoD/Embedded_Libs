#include "gpio_common.h"

GPIOcommon::GPIOcommon(GPIO_Info gpioInfo){
    gpio = gpioInfo.gpio;
    pin = gpioInfo.pin;
	if(pin > 15){
		pin = 0;
	}
	if(gpio == GPIOA){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	} else if(gpio == GPIOB){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	} else if(gpio == GPIOC){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	} else if(gpio == GPIOD){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	} else if(gpio == GPIOE){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	} else if(gpio == GPIOF){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	} else if(gpio == GPIOF){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	} else if(gpio == GPIOF){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
	} else if(gpio == GPIOF){
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
	}
}
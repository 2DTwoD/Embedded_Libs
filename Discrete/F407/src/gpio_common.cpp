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
//GPIOconfig
GPIOconfig::GPIOconfig(const GPIO_Info &gpioInfo) : gpioInfo(gpioInfo) {
    if(gpioInfo.pin > 15) error = true;
    if(gpioInfo.gpio == GPIOA){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    } else if(gpioInfo.gpio == GPIOB){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    } else if(gpioInfo.gpio == GPIOC){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    } else if(gpioInfo.gpio == GPIOD){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    } else if(gpioInfo.gpio == GPIOE){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    } else if(gpioInfo.gpio == GPIOF){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    } else if(gpioInfo.gpio == GPIOF){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    } else if(gpioInfo.gpio == GPIOF){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    } else if(gpioInfo.gpio == GPIOF){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
    } else {
        error = true;
    }
}

void GPIOconfig::setMODER(GPIOmoder moder) const {
    if(error) return;
    setRegValShift(gpioInfo.gpio->MODER, 0b11 << (gpioInfo.pin * 2), moder);
}

void GPIOconfig::setOTYPER(GPIOotyper otyper) const {
    if(error) return;
    setRegValShift(gpioInfo.gpio->OTYPER, 1 << gpioInfo.pin, otyper);
}

void GPIOconfig::setOSPEEDR(GPIOospeedr ospeedr) const {
    if(error) return;
    setRegValShift(gpioInfo.gpio->OSPEEDR, 0b11 << (gpioInfo.pin * 2), ospeedr);
}

void GPIOconfig::setPUPDR(GPIOpupdr pupdr) const {
    if(error) return;
    setRegValShift(gpioInfo.gpio->PUPDR, 0b11 << (gpioInfo.pin * 2), pupdr);
}

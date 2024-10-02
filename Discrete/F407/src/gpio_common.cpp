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
    init();
}

void GPIOconfig::init() {
    noErr = true;
    if(gpioInfo.pin > 15) noErr = false;
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
        noErr = false;
    }
}

bool GPIOconfig::noErrors() {
    return noErr;
}

GPIOconfig &GPIOconfig::start() {
    if(noErr) setMODER(GPIO_MODER_INPUT).setOTYPER(GPIO_OTYPER_PUSH_PULL).setOSPEEDR(GPIO_OSPEEDR_LOW_SPEED).
                setPUPDR(GPIO_PUPDR_NO_PULL).setAFR(GPIO_AFR0).setODR(false).setLCK(false);
    return *this;
}

GPIOconfig &GPIOconfig::focus(GPIO_Info newGpioInfo) {
    gpioInfo = newGpioInfo;
    init();
    return *this;
}

GPIOconfig& GPIOconfig::setMODER(GPIOmoder moder) {
    if(noErr) setRegValShift(gpioInfo.gpio->MODER, 0b11 << (gpioInfo.pin * 2), moder);
    return *this;
}

GPIOconfig& GPIOconfig::setOTYPER(GPIOotyper otyper) {
    if(noErr) setRegValShift(gpioInfo.gpio->OTYPER, 1 << gpioInfo.pin, otyper);
    return *this;
}

GPIOconfig& GPIOconfig::setOSPEEDR(GPIOospeedr ospeedr) {
    if(noErr) setRegValShift(gpioInfo.gpio->OSPEEDR, 0b11 << (gpioInfo.pin * 2), ospeedr);
    return *this;
}

GPIOconfig& GPIOconfig::setPUPDR(GPIOpupdr pupdr) {
    if(noErr) setRegValShift(gpioInfo.gpio->PUPDR, 0b11 << (gpioInfo.pin * 2), pupdr);
    return *this;
}

bool GPIOconfig::getIDR() const {
    return noErr && getBitByPos(gpioInfo.gpio->IDR, gpioInfo.pin);
}

GPIOconfig& GPIOconfig::setODR(bool value) {
    if(noErr) setBitByPosAndVal(gpioInfo.gpio->ODR, gpioInfo.pin, value);
    return *this;
}

GPIOconfig &GPIOconfig::setWithBSRR(bool value) {
    if(noErr){
        uint8_t shift = value? 0: 16;
        setBitByPos(gpioInfo.gpio->BSRR, shift + gpioInfo.pin);
    }
    return *this;
}

GPIOconfig &GPIOconfig::setLCK(bool value) {
    if(noErr) setBitByPosAndVal(gpioInfo.gpio->LCKR, gpioInfo.pin, value);
    return *this;
}

bool GPIOconfig::lockAllPortLCKK() const {
    if(!noErr) return false;
    setBitByPos(gpioInfo.gpio->LCKR, 16);
    resetBitByPos(gpioInfo.gpio->LCKR, 16);
    setBitByPos(gpioInfo.gpio->LCKR, 16);
    gpioInfo.gpio->LCKR;
    return getBitByPos(gpioInfo.gpio->LCKR, 16);
}

GPIOconfig &GPIOconfig::setAFR(GPIOafr afr) {
    if(noErr){
        if(gpioInfo.pin < 8){
            setRegValShift(gpioInfo.gpio->AFR[0], 0b1111 << (gpioInfo.pin * 4), afr);
        } else {
            setRegValShift(gpioInfo.gpio->AFR[1], 0b1111 << ((gpioInfo.pin - 8) * 4), afr);
        }
    }
    return *this;
}

void GPIOconfig::fin() {
    //No code, only for finalize (without return)
}

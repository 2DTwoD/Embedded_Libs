#include "coil.h"

Coil::Coil(GPIO_Info gpioInfo): GPIOcommon(gpioInfo) {
    setRegValShift(gpio->MODER, 0b11 << (2 * pin), 0b01);
}
bool Coil::isActive(){
	return (gpio->ODR & (1 << pin)) > 0;
}
void Coil::setValue(bool value){
	ProgrammCoil::setValue(value);
	if(value){
		//if(isNotActive()) 
		gpio->BSRR |= (1 << pin);
		return;
	}
	//if(isActive())
	gpio->BSRR |= (1 << (pin + 16));
}
Coil& Coil::operator=(bool value){
	setValue(value);
	return *this;
}

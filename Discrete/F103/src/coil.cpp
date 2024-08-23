#include "coil.h"

Coil::Coil(GPIO_Info gpioInfo): GPIOcommon(gpioInfo) {
	if(pin < 8){
        setRegisterWithAutoShift(&gpio->CRL, 0x1111 << (4 * pin), 0b0001);
	} else {
        setRegisterWithAutoShift(&gpio->CRH, 0x1111 << (4 * pin - 32), 0b0001);
	}
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
	gpio->BRR |= (1 << pin);
}
Coil& Coil::operator=(bool value){
	setValue(value);
	return *this;
}

#include "simple_input.h"
//SimpleInput
SimpleInput::SimpleInput(GPIO_Info gpioInfo): GPIOcommon(gpioInfo){
	uint8_t shift = 4 * pin;
	if(pin < 8){
		setRegister(&gpio->CRL, 0xF << shift, 0x8 << shift);
	} else {
		shift -= 32;
		setRegister(&gpio->CRH, 0xF << shift, 0x8 << shift);
	}
	gpio->ODR |= 1 << pin;
	#ifdef SIM_ON
	sim_on = true;
	#endif
}

bool SimpleInput::isActive(){
	return (gpio->IDR & (1 << pin)) == 0
	#ifdef SIM_ON
	|| sim_on && sim_val
	#endif
	;
}

bool SimpleInput::isNotActive(){
	return !isActive();
}

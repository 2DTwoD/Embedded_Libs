#include "simple_input.h"
//SimpleInput
SimpleInput::SimpleInput(GPIO_Info gpioInfo): GPIOcommon(gpioInfo){
    setRegValShift(gpio->MODER, 0b11 << 2 * pin, 0);
    setRegValShift(gpio->PUPDR, 0b11 << 2 * pin, 0b01);
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

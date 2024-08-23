#include "simple_input.h"
//SimpleInput
SimpleInput::SimpleInput(GPIO_TypeDef *gpio, uint8_t pin): GPIOcommon(gpio, pin){
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

//SimpleInputDelayed
SimpleInputDelayed::SimpleInputDelayed(GPIO_TypeDef *gpio, uint8_t pin, uint16_t delay): SimpleInput(gpio, pin), CommonTimer(delay){
}
void SimpleInputDelayed::update1ms(){
	CommonTimer::setStart(SimpleInput::isActive());
	CommonTimer::update();
}
bool SimpleInputDelayed::isActive(){
	return finished();
}

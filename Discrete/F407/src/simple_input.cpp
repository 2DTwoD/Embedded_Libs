#include "simple_input.h"
//SimpleInput
SimpleInput::SimpleInput(GPIO_Info gpioInfo): GPIOcommon(gpioInfo){
    gpio.start().setMODER(GPIO_MODER_INPUT).setPUPDR(GPIO_PUPDR_PULL_UP).fin();
	#ifdef SIM_ON
	sim_on = true;
	#endif
}

bool SimpleInput::isActive(){
	return gpio.getIDR() == 0
	#ifdef SIM_ON
	|| sim_on && sim_val
	#endif
	;
}

bool SimpleInput::isNotActive(){
	return !isActive();
}

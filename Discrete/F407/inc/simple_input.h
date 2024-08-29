#ifndef SIMPLE_INPUT_H
#define SIMPLE_INPUT_H
#include "stm32f407xx.h"
#include "register.h"
#include "interfaces.h"
#include "gpio_common.h"
#include "common_timer.h"

//SimpleInput
class SimpleInput: public GPIOcommon, public ISwitch{
	#ifdef SIM_ON
	private:
		bool sim_on;
		bool sim_val;
	#endif
	public:
		explicit SimpleInput(GPIO_Info gpioInfo);
	
		bool isActive() override;
	
		bool isNotActive();
};


#endif //SIMPLE_INPUT_H
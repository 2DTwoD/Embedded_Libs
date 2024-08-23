#ifndef SIMPLE_INPUT_H
#define SIMPLE_INPUT_H
#include "stm32f103xb.h"
#include "common.h"
#include "interfaces.h"
#include "gpio_common.h"
#include "common_timer.h"

#define SIM_ON

//SimpleInput
class SimpleInput: public GPIOcommon, public ISwitch{
	#ifdef SIM_ON
	private:
		bool sim_on;
		bool sim_val;
	#endif
	public:
		SimpleInput(GPIO_TypeDef *gpio, uint8_t pin);
	
		bool isActive() override;
	
		bool isNotActive();
};

//SimpleInputDelayed (implements IUpdated1ms)
class SimpleInputDelayed: private CommonTimer, public SimpleInput, public IUpdated1ms {
	public:
		SimpleInputDelayed(GPIO_TypeDef *gpio, uint8_t pin, uint16_t delay);
		void update1ms() override;
		bool isActive() override;
};

#endif //SIMPLE_INPUT_H
#ifndef COIL_H
#define COIL_H

#include "stm32f103xb.h"
#include "interfaces.h"
#include "gpio_common.h"
#include "common.h"
#include "common_timer.h"

class Coil: public GPIOcommon, public ProgrammCoil {
  public:
		Coil(GPIO_TypeDef *gpio, uint8_t pin);
	
		bool isActive() override;
	
		void setValue(bool value) override;
	
		Coil& operator=(bool value);
};

#endif //COIL_H
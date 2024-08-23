#ifndef COIL_H
#define COIL_H

#include "stm32f407xx.h"
#include "interfaces.h"
#include "gpio_common.h"
#include "common.h"
#include "common_timer.h"
#include "programm_coil.h"

class Coil: public GPIOcommon, public ProgrammCoil {
  public:
		explicit Coil(GPIO_Info gpioInfo);
	
		bool isActive() override;
	
		void setValue(bool value) override;
	
		Coil& operator=(bool value);
};

#endif //COIL_H
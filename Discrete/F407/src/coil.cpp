#include "coil.h"

Coil::Coil(GPIO_Info gpioInfo): GPIOcommon(gpioInfo) {
    gpio.start().setMODER(GPIO_MODER_OUTPUT).fin();
}
bool Coil::isActive(){
	return gpio.getODR() > 0;
}
void Coil::setValue(bool value){
	ProgrammCoil::setValue(value);
    gpio.setWithBSRR(value);
}
Coil& Coil::operator=(bool value){
	setValue(value);
	return *this;
}

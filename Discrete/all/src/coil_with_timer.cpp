#include "coil_with_timer.h"

//CoilOnDelayed
CoilOnDelay::CoilOnDelay(GPIO_TypeDef *gpio, uint8_t pin, uint16_t delay): Coil(gpio, pin), OnDelayCommon(delay) {
}
void CoilOnDelay::update1ms() {
	OnDelayCommon::update();
	Coil::setValue(OnDelayCommon::get());
}
void CoilOnDelay::setValue(bool value){
	OnDelayCommon::set(value);
}
CoilOnDelay& CoilOnDelay::operator=(bool value){
	setValue(value);
	return *this;
}

//CoilOffDelayed
CoilOffDelay::CoilOffDelay(GPIO_TypeDef *gpio, uint8_t pin, uint16_t delay): Coil(gpio, pin), OffDelayCommon(delay) {
}
void CoilOffDelay::update1ms() {
	OffDelayCommon::update();
	Coil::setValue(OffDelayCommon::get());
}
void CoilOffDelay::setValue(bool value){
	OffDelayCommon::set(value);
}
CoilOffDelay& CoilOffDelay::operator=(bool value){
	setValue(value);
	return *this;
}

//CoilPulse
CoilPulse::CoilPulse(GPIO_TypeDef *gpio, uint8_t pin, uint16_t delay): Coil(gpio, pin), PulseCommon(delay) {
}
void CoilPulse::update1ms() {
	PulseCommon::update();
	Coil::setValue(PulseCommon::get());
}
void CoilPulse::setValue(bool value){
	PulseCommon::set(value);
}
CoilPulse& CoilPulse::operator=(bool value){
	setValue(value);
	return *this;
}
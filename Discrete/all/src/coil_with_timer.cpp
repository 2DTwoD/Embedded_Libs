#include "coil_with_timer.h"

//CoilOnDelayed
CoilOnDelay::CoilOnDelay(GPIO_Info gpioInfo, uint16_t delay): Coil(gpioInfo), OnDelayCommon(delay) {
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
CoilOffDelay::CoilOffDelay(GPIO_Info gpioInfo, uint16_t delay): Coil(gpioInfo), OffDelayCommon(delay) {
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
CoilPulse::CoilPulse(GPIO_Info gpioInfo, uint16_t delay): Coil(gpioInfo), PulseCommon(delay) {
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
#include "analog_out.h"

AnalogOut::AnalogOut(uint16_t rawRange, uint32_t fullRangeTime, float outMin, float outMax, float limitMin, float limitMax):
	Ramp(fullRangeTime, outMin, outMax, outMin, outMax){
	scale = new Scale(outMin, outMax, (uint16_t)0, rawRange);
}
void AnalogOut::update1ms(){
	Ramp::update1ms();
	scale->set(Ramp::get());
}
AnalogOut::~AnalogOut(){
	delete scale;
}
void AnalogOut::set(float value){
	Ramp::set(value);
}
uint16_t AnalogOut::get(){
	return Ramp::get();
}
AnalogOut& AnalogOut::operator=(float value){
	Ramp::set(value);
	return *this;
}
float *const AnalogOut::getInRef(){
	return getSpRef();
}
uint16_t *const AnalogOut::getOutRef(){
	return scale->getOutRef();
}
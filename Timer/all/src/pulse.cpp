#include "pulse.h"

//PulseCommon
PulseCommon::PulseCommon(uint32_t period): CommonTimer(period){
}
void PulseCommon::update(){
	if(CommonTimer::finished()){
		CommonTimer::stop();
	}
	CommonTimer::update();
}
void PulseCommon::set(bool value){
	if(value){
		if(!startFlag){
			startFlag = true;
			CommonTimer::start();
		}
	} else {
		if(CommonTimer::isFree()){
			startFlag = false;
		}
	}
}
bool PulseCommon::get(){
	return CommonTimer::inWork();
}
void PulseCommon::reset(){
	startFlag = false;
	CommonTimer::stop();
}

void PulseCommon::again() {
    CommonTimer::prepareAndStart();
}

//Pulse
Pulse::Pulse(uint32_t period): PulseCommon(period){
}
void Pulse::update1ms(){
	PulseCommon::update();
}
Pulse& Pulse::operator=(bool value){
	set(value);
	return *this;
}

//PulseInterrapt
PulseInterrapt::PulseInterrapt(uint32_t period): PulseCommon(period){
}
void PulseInterrapt::update1ms(){
	PulseCommon::update();
}
void PulseInterrapt::set(bool value){
	PulseCommon::set(value);
	if(!value) {
		PulseCommon::reset();
	}
}
PulseInterrapt& PulseInterrapt::operator=(bool value){
	set(value);
	return *this;
}
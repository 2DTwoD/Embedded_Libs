#include "off_delay.h"

//OffDelayCommon
OffDelayCommon::OffDelayCommon(uint32_t period): CommonTimer(period){
}
void OffDelayCommon::update(){
	if(CommonTimer::finished()){
		CommonTimer::stop();
	}
	CommonTimer::update();
}
void OffDelayCommon::set(bool value){
	if(!value){
		if(startFlag){
			CommonTimer::prepareAndStart();
		}
	} else {
		CommonTimer::stop();
	}
	startFlag = value;
}
bool OffDelayCommon::get(){
	return startFlag || CommonTimer::inWork();
}
void OffDelayCommon::reset(){
	CommonTimer::finish();
}

//OffDelay
OffDelay::OffDelay(uint32_t period): OffDelayCommon(period){
}
void OffDelay::update1ms(){
	OffDelayCommon::update();
}
OffDelay& OffDelay::operator=(bool value){
	set(value);
	return *this;
}
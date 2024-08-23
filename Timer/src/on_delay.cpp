#include "on_delay.h"

//OnDelayCommon
OnDelayCommon::OnDelayCommon(uint32_t period): CommonTimer(period){
}
void OnDelayCommon::update(){
	CommonTimer::update();
}
void OnDelayCommon::set(bool value){
	CommonTimer::setStart(value);
}
void OnDelayCommon::setPause(bool value){
	CommonTimer::setPause(value);
}
bool OnDelayCommon::get(){
	return CommonTimer::finished();
}
void OnDelayCommon::reset(){
	CommonTimer::reset();
}

//OnDelay
OnDelay::OnDelay(uint32_t period): OnDelayCommon(period){
}
void OnDelay::update1ms(){
	OnDelayCommon::update();
}
OnDelay& OnDelay::operator=(bool value){
	set(value);
	return *this;
}
OnDelay& OnDelay::operator+=(bool value){
	setPause(value);
	return *this;
}
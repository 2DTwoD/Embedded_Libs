#include "switch_delay.h"

SwitchDelay::SwitchDelay(ISwitch *swtch, uint16_t period): CommonTimer(period){
	this->swtch = swtch;
}
void SwitchDelay::update1ms(){
	CommonTimer::update();
}
bool SwitchDelay::started(){
	return swtch->isActive();
}
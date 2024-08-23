#include "ramp.h"

Ramp::Ramp(uint32_t fullRangeTime, float outMin, float outMax, float limitMin, float limitMax){
	if(outMin > outMax){
		toggle(&outMin, &outMax);
	}
	if(limitMin > limitMax){
		toggle(&limitMin, &limitMax);
	}
	outRange[0] = outMin;
	outRange[1] = outMax;
	outLimits[0] = limitMin;
	setLimMax(limitMax);
	setFullRangeTime(fullRangeTime);
}
Ramp::Ramp(uint32_t fullRangeTime, float outMin, float outMax): Ramp(fullRangeTime, outMin, outMax, outMin, outMax){
}

Ramp::Ramp(uint32_t fullRangeTime): Ramp(fullRangeTime, 0.0f, 100.0f, 0.0f, 100.0f){
}
float Ramp::getOutMin(){
	return outRange[0];
}
float Ramp::getOutRange(){
	return getRange(outRange);
}
void Ramp::update1ms(){
	if(!step){
		out = sp;
	} else {
		if(out < sp){
			out += step;
			if (out > sp) out = sp;
		} else if(out > sp){
			out -= step;
			if (out < sp) out = sp;
		}
	}
	out = limit(out, outLimits[0], outLimits[1]);
}
float Ramp::get(){
	return out;
}
void Ramp::set(float value){
	sp = limit(value, outRange[0], outRange[1]);
}
uint32_t Ramp::getFullRangeTime(){
	return fullRangeTime;
}
void Ramp::setFullRangeTime(uint32_t value){
	if(value > 0){
		step = getOutRange() / value;
	} else {
		step = 0;
	}
	this->fullRangeTime = value;
}
void Ramp::setLimMin(float value){
	outLimits[0] = min(value, outLimits[1]);
}
void Ramp::setLimMax(float value){
	outLimits[1] = max(value, outLimits[0]);
}
Ramp& Ramp::operator=(float value){
	set(value);
	return *this;
}
float *const Ramp::getSpRef(){
	return &sp;
}
float *const Ramp::getOutRef(){
	return &out;
}
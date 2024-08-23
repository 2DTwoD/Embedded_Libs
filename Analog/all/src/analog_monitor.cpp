#include "analog_monitor.h"

AnalogMonitor::AnalogMonitor(uint8_t adcCapacity, float valueMin, float valueMax): Monitor(valueMin, valueMax){
	uint16_t adcMax = 0;
	for(int i = 0; i < adcCapacity; i++){
		adcMax |= 1 << i;
	}
	analogScale = new Scale((uint16_t)0, adcMax, valueMin, valueMax);
}

AnalogMonitor::~AnalogMonitor(){
	delete analogScale;
}

void AnalogMonitor::set(uint16_t value) {
	*analogScale = value;
	Monitor::set(analogScale->get());
}

AnalogMonitor& AnalogMonitor::operator=(uint16_t value){
	set(value);
	return *this;
}

uint16_t *const AnalogMonitor::getInRef(){
	return analogScale->getInRef();
}

float *const AnalogMonitor::getOutRef(){
	return analogScale->getOutRef();
}
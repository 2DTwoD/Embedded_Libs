#ifndef ANALOG_MONITOR_H
#define ANALOG_MONITOR_H

#include <cstdint>
#include "interfaces.h"
#include "common_timer.h"
#include "monitor.h"
#include "common.h"
#include "scale.h"

class AnalogMonitor: public Monitor{
	private:
		Scale<uint16_t, float> *analogScale = nullptr;
	public:
		AnalogMonitor(uint8_t adcCapacity, float valueMin = 0.0f, float valueMax = 0.0f);
		~AnalogMonitor();
		void set(uint16_t value);
		AnalogMonitor& operator=(uint16_t value);
		uint16_t *const getInRef();
		float *const getOutRef();
};

#endif //ANALOG_MONITOR_H
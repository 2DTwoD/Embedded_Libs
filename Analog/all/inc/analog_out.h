#ifndef ANALOG_OUT_H
#define ANALOG_OUT_H

#include <cstdint>

#include "interfaces.h"
#include "common.h"
#include "ramp.h"
#include "scale.h"

class AnalogOut: public Ramp{
	private:
		Scale<float, uint16_t> *scale = nullptr;
	public:
		AnalogOut(uint16_t rawRange, uint32_t fullRangeTime = 0,
							float outMin = 0.0f, float outMax = 100.0f, float limitMin = 0.0f, float limitMax = 100.0f);
		~AnalogOut();
		void set(float value);
		uint16_t get();
		void update1ms() override;
		AnalogOut& operator=(float value);
		float *const getInRef();
		uint16_t *const getOutRef();
};

#endif //ANALOG_OUT_H
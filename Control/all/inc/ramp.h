#ifndef RAMP_H
#define RAMP_H

#include <cstdint>

#include "interfaces.h"
#include "math_fun.h"
#include "array_fun.h"

class Ramp: public IUpdated1ms{
	private:
		float out;
		float sp;
		float step;
		uint32_t fullRangeTime;
		float outRange[2];
		float outLimits[2];
	protected:
		float getOutMin();
		float getOutRange();
	public:
		Ramp(uint32_t fullRangeTime = 0);
		Ramp(uint32_t fullRangeTime, float outMin, float outMax);
		Ramp(uint32_t fullRangeTime, float outMin, float outMax, float limitMin, float limitMax);
		void update1ms() override;
		float get();
		void set(float value);
		uint32_t getFullRangeTime();
		void setFullRangeTime(uint32_t value);
		void setLimMin(float value);
		void setLimMax(float value);
		Ramp& operator=(float value);
		float *const getSpRef();
		float *const getOutRef();
};

#endif //RAMP_H
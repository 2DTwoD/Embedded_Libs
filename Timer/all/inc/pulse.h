#ifndef PULSE_H
#define PULSE_H

#include <cstdint>
#include "common_timer.h"

class PulseCommon: private CommonTimer {
	private:
		bool startFlag;
	public:
		explicit PulseCommon(uint32_t period);
		void update();
		void set(bool value);
		bool get();
		void reset();
        void again();
};

class Pulse: public PulseCommon, public IUpdated1ms {
	public:
		explicit Pulse(uint32_t period);
		void update1ms() override;
		Pulse& operator=(bool value);
};

class PulseInterrapt: public PulseCommon, public IUpdated1ms {
	public:
		explicit PulseInterrapt(uint32_t period);
		void update1ms() override;
		void set(bool value);
		PulseInterrapt& operator=(bool value);
};

#endif //PULSE_H
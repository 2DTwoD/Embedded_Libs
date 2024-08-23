#ifndef OFF_DELAY_H
#define OFF_DELAY_H

#include <cstdint>
#include "common_timer.h"

class OffDelayCommon: private CommonTimer{
	private:
		bool startFlag;
	public:
		explicit OffDelayCommon(uint32_t period);
		void update();
		void set(bool value);
		bool get();
		void reset();
};

class OffDelay: public OffDelayCommon, public IUpdated1ms {
	public:
		explicit OffDelay(uint32_t period);
		void update1ms() override;
		OffDelay& operator=(bool value);
};

#endif //OFF_DELAY_H
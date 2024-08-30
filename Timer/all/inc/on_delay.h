#ifndef ON_DELAY_H
#define ON_DELAY_H
#include <cstdint>
#include "common_timer.h"

class OnDelayCommon: private CommonTimer {
	public:
		explicit OnDelayCommon(uint32_t period);
		void update();
		void set(bool value);
		void setPause(bool value);
		bool get();
		void reset();
        void again();
};

class OnDelay: public OnDelayCommon, public IUpdated1ms {
	public:
		explicit OnDelay(uint32_t period);
		void update1ms() override;
		OnDelay& operator=(bool value);
		OnDelay& operator+=(bool value);
};

#endif //ON_DELAY_H
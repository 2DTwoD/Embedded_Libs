#ifndef COMMON_TIMER_H
#define COMMON_TIMER_H
#include <cstdint>
#include "interfaces.h"

class CommonTimer {
	private:
		uint32_t period;
		uint32_t curTime;
		bool go;
		bool impulse;
	public:
		CommonTimer(uint32_t period);
		void update();
		uint32_t getPeriod();
		void setPeriod(uint32_t value);
		uint32_t getCurrentTime();
		void setCurrentTime(uint32_t value);
		void start();
		void pause();
		void stop();
		void reset();
		void prepareAndStart();
		void finish();
		void setStart(bool value);
		void setPause(bool value);
		CommonTimer& operator=(bool value);
		CommonTimer& operator+=(bool value);
		virtual bool started();
	  bool notStarted();
		bool finished();
		bool notFinished();
		bool finishedImpulse();
		bool inWork();
		bool isFree();
};

#endif //COMMON_TIMER_H
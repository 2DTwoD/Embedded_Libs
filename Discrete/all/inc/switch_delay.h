#ifndef SWITCH_DELAY_H
#define SWITCH_DELAY_H
#include "interfaces.h"
#include "common_timer.h"

class SwitchDelay: private CommonTimer, public IUpdated1ms{
	private:
		ISwitch *swtch;
	public:
		SwitchDelay(ISwitch *swtch, uint16_t period);
		void update1ms() override;
		bool started() override;
};
#endif //SWITCH_DELAY_H
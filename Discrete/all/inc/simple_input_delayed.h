#ifndef SIMPLE_INPUT_DELAYED_H
#define SIMPLE_INPUT_DELAYED_H

#include "interfaces.h"
#include "gpio_common.h"
#include "simple_input.h"
#include "common_timer.h"

//implements IUpdated1ms
class SimpleInputDelayed: private CommonTimer, public SimpleInput, public IUpdated1ms {
public:
    SimpleInputDelayed(GPIO_Info gpioInfo, uint16_t delay);
    void update1ms() override;
    bool isActive() override;
};

#endif //SIMPLE_INPUT_DELAYED_H

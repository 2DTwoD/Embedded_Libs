#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include "coil_with_timer.h"
#include "scale.h"
#include "simple_input_delayed.h"
#include "mux.h"


CoilOffDelay led1({GPIOE, 13}, 1000);
Coil led2({GPIOE, 14});
SimpleInputDelayed button({GPIOE, 10}, 10);
OnDelay delay(1000);
Mux<int> mux(10);

IUpdated1ms *updateObjects[] = {
	&button,
	&led1,
    &delay
};

uint8_t updateObjectsSize = sizeof(updateObjects) / sizeof(*updateObjects);
#endif //GLOBAL_VARS_H
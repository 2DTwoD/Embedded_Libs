#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include "../../Discrete/F103/inc/coil.h"
#include "coil_with_timer.h"
#include "common_timer.h"
#include "simple_input.h"
#include "analog_monitor.h"
#include "analog_out.h"
#include "pid.h"
#include "scale.h"
#include "moving_avg.h"
#include "two_pos.h"
#include "three_pos.h"
#include "mux.h"
#include "switch_delay.h"
#include "on_delay.h"
#include "off_delay.h"
#include "pulse.h"
#include "counter.h"
#include "sequence.h"

CoilOffDelay led(GPIOC, 13, 1000);
SimpleInputDelayed button(GPIOA, 0, 10);

OnDelay delay(10);

volatile uint16_t adcValues[2] = {0, 0};
AnalogMonitor adcMonitor(12, 0, 100);
AnalogOut analogOut(1000, 15000, 100, 0, 0, 100);
PIDreg pid(20, 50.0);
Scale<float, int16_t> scale(0.0f, 100.0f, (int16_t)0, (int16_t)100);
Ramp ramp(15000);
MovAvg avg(10);
TwoPosTim twoPos(50.0f, 5000, 1000);
ThreePosReg threePosReg(50.0f, 5.0f, 15.0f, 1000, 1000);
OnDelay onDelay(5000);
OffDelay offDelay(5000);
PulseInterrapt pulse(5000);
Counter counter(RISE_FALL, 100);
uint8_t step = 0;
Sequence seq1(&step, 0);
SequenceDelayed seq2(&step, 1, 5000);

IUpdated1ms *updateObjects[] = {
	&delay,
	&adcMonitor,
	&analogOut,
	&ramp,
	&twoPos, 
	&threePosReg,
	&button,
	&onDelay,
	&offDelay,
	&pulse,
	&seq2,
	&led
};

uint8_t updateObjectsSize = sizeof(updateObjects) / sizeof(*updateObjects);
#endif //GLOBAL_VARS_H
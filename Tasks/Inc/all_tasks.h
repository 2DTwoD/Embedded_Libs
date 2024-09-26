#ifndef ALL_TASKS_H
#define ALL_TASKS_H
#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "common.h"
#include "interfaces.h"
#include "coil.h"
#include "coil_with_timer.h"
#include "simple_input_delayed.h"
#include "counter.h"
#include "pid.h"
#include "analog_monitor.h"
#include "scale.h"
#include "moving_avg.h"
#include "ramp.h"
#include "pulse.h"
#include "analog_out.h"
#include "rf_impulse.h"
#include "sequence.h"
#include "simple_uart.h"
#include "simple_i2c.h"
#include "simple_spi.h"
#include "logger.h"
#include "linked_list.h"
#include "string_bridge.h"
#include "string_map.h"

void ledTask(void *pvParameters);
void pidTask(void *pvParameters);
void testTask(void *pvParameters);

#endif //ALL_TASKS_H
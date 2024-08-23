#include "all_tasks.h"

extern PIDreg pid;
extern AnalogMonitor adcMonitor;

void pidTask(void *pvParameters){
	pid.setKp(0.1f);
	pid.setTi(200.0f);
	pid.setTd(1.0f);
	pid.setAuto(true);
	while(true){
		pid = adcMonitor.get();
		vTaskDelay(pid.getT());
	}
}
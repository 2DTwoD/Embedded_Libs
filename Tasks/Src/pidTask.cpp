#include "all_tasks.h"

extern PIDreg pid;

void pidTask(void *pvParameters){
	/*pid.setKp(0.1f);
	pid.setTi(200.0f);
	pid.setTd(1.0f);
	pid.setAuto(true);*/
	while(true){
		/*pid = 50;
		vTaskDelay(pid.getT());*/
		vTaskDelay(1);
	}
}
#include "all_tasks.h"

extern CoilOffDelay led1;
extern Coil led2;
extern OnDelay delay;
extern SimpleInputDelayed button;

void ledTask(void *pvParameters){
    delay = true;
	while(true){
		led1 = button.isActive();
        if(delay.get()){
            led2.toggle();
            delay.reset();
        }
		vTaskDelay(1);
	}
}
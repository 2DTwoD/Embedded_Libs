#include "all_tasks.h"

extern SimpleUART uart;

void testTask(void *pvParameters){
	while(true){
        uart.click();
		vTaskDelay(1000);
	}
}
#include "all_tasks.h"

extern SimpleUART uart;

void testTask(void *pvParameters){
	while(true){
        char mes[20];
        sprintf(mes, "first in buffer: %c; ", uart.getAndClearByte(0));
        uart.print(mes);
		vTaskDelay(1000);
	}
}
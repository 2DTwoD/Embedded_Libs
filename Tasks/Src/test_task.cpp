#include "all_tasks.h"

extern SimpleUART uart;

void testTask(void *pvParameters){
	while(true){
        char mes[20];
        sprintf(mes, "first in buffer: %c; ", uart.getByte(0));
        uart.print(mes);
        uart.clearByte(0);
		vTaskDelay(1000);
	}
}
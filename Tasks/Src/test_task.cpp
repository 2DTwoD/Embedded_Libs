#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
void testTask(void *pvParameters){
    char mes[20];
    uint8_t slaves[10];
    bool exist = i2c.slaveIsExist(80);
    i2c.getSlaves(slaves, 10);
	while(true){
        sprintf(mes, "first in buffer: %c; ", uart.getAndClearByte(0));
        uart.print(mes);
		vTaskDelay(1000);
	}
}
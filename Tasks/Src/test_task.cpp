#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;

void testTask(void *pvParameters){
    //char mes[20];
    uint8_t slaves[10];
    uint8_t send_data[] = {10,1,2,3,4, 5};
    uint8_t receive_data[10]{0};
    bool exist = i2c.slaveIsExist(80);
    i2c.getSlaves(slaves, 10);
    i2c.transmit(80, send_data, 6);
	while(true){
        //sprintf(mes, "first in buffer: %c; ", i2c.getAndClearByte(0));
        i2c.transmit(80,send_data, 1);
        i2c.receive(80, 1);
        int a = i2c.getByte(0);
        //uart.print(mes);
		vTaskDelay(1000);
	}
}
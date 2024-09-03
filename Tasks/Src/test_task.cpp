#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
MemoryLogger logger(10);

void testTask(void *pvParameters){
    char mes[25];
    uint8_t slaves[10];
    uint8_t send_data[] = {0,'1','2','3','4', '5'};
    uint8_t receive_data[10]{0};
    badTimeOut(100000);
    i2c.getSlaves(slaves, 10);
    i2c.transmit(80, send_data, 6);
    int counter = 0;
	while(true){
        logger.newMessage(counter, "ebula huebule");
        logger.newMessage(counter, "vot ono chto");
        sprintf(mes, "first in buffer: %c; ", i2c.getAndClearByte());
        uart.print(mes);
        taskENTER_CRITICAL();
        i2c.writeAndRead(80, counter++, 1);
        if(counter > 4){
            counter = 0;
        }
        taskEXIT_CRITICAL();
        i2c.getBytes(receive_data, 0, 10);
		vTaskDelay(1000);
	}
}
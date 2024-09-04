#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
extern SimpleSPI spi;
MemoryLogger logger(10);

void testTask(void *pvParameters){
    char mes[25];
    uint8_t reset_data[] = {0x66, 0x99};
    uint8_t send_data[] = {0x90,0, 0, 0};
    spi.send(reset_data, 2);
	while(true){
        sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);
        taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 2);
        taskEXIT_CRITICAL();
		vTaskDelay(1000);
	}
}
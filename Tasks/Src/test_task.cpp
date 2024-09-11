#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
extern SimpleSPI spi;
MemoryLogger logger(10);

#include "result.h"

Result test(){
    return ResultBuilder::getOK();
}

void testTask(void *pvParameters){
    char mes[25];
    uint8_t reset_data[] = {0x66, 0x99};
    uint8_t send_data[] = {0x0b,0xFF, 0x0, 0x0};
    spi.send(reset_data, 2);
	while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);*/
        taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 11);
        taskEXIT_CRITICAL();
        Result result = test();
        if(result.label == rOK){
            int a = 0;
        }
		vTaskDelay(1000);
	}
}
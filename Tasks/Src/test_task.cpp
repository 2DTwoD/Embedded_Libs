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
    float src[] = {1,2,3,4,5};
    uint8_t dst[] = {11,22,33,44,55,66,77,88,99};
    Buffer buffer(100);
    buffer.insertByte(dst, 9);
    uint8_t reset_data[] = {0x66, 0x99};
    uint8_t send_data[] = {0x0b,0xFF, 0x0, 0x0};
    spi.send(reset_data, 2);
	while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);*/
        taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 11);
        taskEXIT_CRITICAL();
        buffer.insertFloat(2, src, 0, 5);
        float a = buffer.getFloat(6);
		vTaskDelay(1000);
	}
}
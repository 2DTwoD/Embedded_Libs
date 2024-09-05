#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
extern SimpleSPI spi;
MemoryLogger logger(10);

void testTask(void *pvParameters){
    char mes[25];
    /*uint8_t reset_data[] = {0x66, 0x99};
    uint8_t send_data[] = {0x90,0, 0, 0};
    spi.send(reset_data, 2);*/
    uint16_t ar[10] = {0, };
    Buffer buffer(10);
    buffer.addByte(1);
    buffer.addByte(2);
    buffer.addByte(3);
    buffer.addByte(4);
    buffer.addByte(5);
    buffer.addByte(6);
    buffer.addByte(7);
    buffer.addByte(8);
    buffer.addByte(9);
    buffer.addByte(10);

	while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);
        taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 2);
        taskEXIT_CRITICAL();*/
        buffer.getWord(ar, 0, 6);
        buffer.clearWord(1);
		vTaskDelay(1000);
	}
}
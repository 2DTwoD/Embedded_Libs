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
    float a = 13.14;
    uint8_t fp[4];
    uint8_t fp2[4];
    floatToBytes(a, fp);
    a = 11.12;
    floatToBytes(a, fp2);
    uint8_t add[] = {fp[0],fp[1],fp[2],fp[3],fp2[0],fp2[1],fp2[2],fp2[3]};
    uint16_t ar[10] = {0, };
    Buffer buffer(10);
    buffer.addByte(add, 8);
    uint32_t c = floatToDWord(a);
    float d = dWordToFloat(c);
	while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);
        taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 2);
        taskEXIT_CRITICAL();*/
        //buffer.getAndClearWord(ar, 0, 2);
        float b = buffer.getAndClearFloat(1);
		vTaskDelay(1000);
	}
}
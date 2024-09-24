#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
extern SimpleSPI spi;
MemoryLogger logger(10);
StringList stringList(10);

struct Test{
    uint8_t a;
    uint8_t b;
};

union test{
    uint8_t ab[2];
    Test tst;
};

void testTask(void *pvParameters){
    /*char mes[25];
    float src[] = {1,2,3,4,5};
    uint8_t dst[] = {11,22,33,44,55,66,77,88,99};
    Buffer buffer(100);
    buffer.insertByte(dst, 9);
    uint8_t reset_data[] = {0x66, 0x99};
    uint8_t send_data[] = {0x0b,0xFF, 0x0, 0x0};
    spi.send(reset_data, 2);*/
    stringList.addByte("opa", 11);
    stringList.addByte("opa2", 22);
    Buffer buffer(10);
    buffer.addWord(10101);
    buffer.addWord(10102);
	while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);*/
        /*taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 11);
        taskEXIT_CRITICAL();*/
        uint16_t a = buffer.getWord(2);
        uint8_t opa = stringList.getByte("opa");
        uint8_t opa2 = stringList.getByte("opa2");
        buffer.setWord(2, 10103);
		vTaskDelay(1000);
	}
}
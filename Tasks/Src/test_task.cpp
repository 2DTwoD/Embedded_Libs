#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
extern SimpleSPI spi;
MemoryLogger logger(10);

void testTask(void *pvParameters){
    /*char mes[25];
    float src[] = {1,2,3,4,5};
    uint8_t dst[] = {11,22,33,44,55,66,77,88,99};
    Buffer buffer(100);
    buffer.insertByte(dst, 9);
    uint8_t reset_data[] = {0x66, 0x99};
    uint8_t send_data[] = {0x0b,0xFF, 0x0, 0x0};
    spi.send(reset_data, 2);*/
    uint8_t check[25]{0};
    uint8_t test[] = {11,22,33,44,55,66,77,88,99};
    LinkedList<uint8_t> list(test, 9);
	while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);*/
        /*taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 11);
        taskEXIT_CRITICAL();*/
        if(list.isEmpty()){
            list.add(test, 9);
        }else {
            list.remove(0, 4);
        }
        fillArray(check, 25, (uint8_t)0);
        list.copyTo(check, 25);
		vTaskDelay(1000);
	}
}
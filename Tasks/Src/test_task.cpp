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
    LinkedList<uint8_t> list;
	while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);*/
        /*taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 11);
        taskEXIT_CRITICAL();*/
        //list.remove(1);
        //list.fill(10);
        /*if(list.size() % 2 == 0){
            list.add(1, 11);
        } else {
            list.add(list.size() - 1, 21);
        }*/
        list.add(test, 9);
        fillArray(check, 25, (uint8_t)0);
        list.copyTo(check, 25);
		vTaskDelay(1000);
	}
}
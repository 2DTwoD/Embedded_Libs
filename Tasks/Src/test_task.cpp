#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
extern SimpleSPI spi;
MemoryLogger logger(10);
StringBridge stringBridge(10);
StringMap<uint32_t> stringMap(0);

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
    stringMap.add("1", 3);
    stringMap.add("2", 1);
    stringMap.add("3", 2);

    while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);*/
        /*taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 11);
        taskEXIT_CRITICAL();*/
        uint8_t a1 = stringMap.get("1");
        uint8_t a2 = stringMap.get("2");
        uint8_t a3 = stringMap.get("3");
        bool a4 = stringMap.isExist("2");
        bool a5 = stringMap.isExist("21");
        stringMap.set("2", 10);
    	vTaskDelay(1000);
	}
}
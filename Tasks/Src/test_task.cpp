#include "all_tasks.h"


extern SimpleUART uart;
extern SimpleI2C i2c;
extern SimpleSPI spi;
MemoryLogger logger(10);
StringBridge stringBridge(10);
StringMap<uint32_t> stringMap(0);
ArrayList<uint8_t> list1(0);
LinkedList<uint8_t> list2(0);
GPIOconfig gpio({GPIOE, 15});
extern SimpleSDIO sdio;

void testTask(void *pvParameters){
    Result res = sdio.init();
    uint32_t buf[1024]{0};
    taskENTER_CRITICAL();
    /*res = sdio.writeBlock(0, buf, 1024);
    res = sdio.readBlock(0, buf, 1024);*/
    taskEXIT_CRITICAL();
    while(true){
        vTaskDelay(1000);
	}
}
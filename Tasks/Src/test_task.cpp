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
    Result res2 = ResultBuilder::getError("Errror");
    Result res3 = ResultBuilder::getOK();
    res3 = res2;
    char mes[] = "111";
    copyStr(mes, "222");
    while(true){
        vTaskDelay(1000);
	}
}
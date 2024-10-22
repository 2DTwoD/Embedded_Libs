#include "all_tasks.h"
#include "ff.h"

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
//    Result res = sdio.init();
//    uint32_t buf[1024]{0};
//    taskENTER_CRITICAL();1
//    /*res = sdio.writeBlock(0, buf, 1024);
//    res = sdio.readBlock(0, buf, 1024);*/
//    taskEXIT_CRITICAL();
    FATFS fatfs;
    FIL file;
    FRESULT res = f_mount(&fatfs, "", 0);
    if(f_mount(&fatfs, "", 0) == FR_OK){
        FRESULT res = f_open(&file, "txt.txt", FA_CREATE_ALWAYS | FA_WRITE);
        if(res == FR_OK){
            uint8_t wtext[] = "тестовая строка";
            unsigned int byteswritten;
            res = f_write(&file, wtext, sizeof(wtext), &byteswritten);
            f_close(&file);
        }
    }
    while(true){
        vTaskDelay(1000);
	}
}
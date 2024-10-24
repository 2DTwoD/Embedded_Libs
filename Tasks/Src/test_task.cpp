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
//    taskENTER_CRITICAL();
//    res = sdio.writeBlock(0, buf, 1024);
//    res = sdio.readBlock(0, buf, 512);
//    taskEXIT_CRITICAL();
    taskENTER_CRITICAL();
    FATFS fatfs;
    FIL file;
    uint8_t readData[100]{};
    UINT *howMany;
    FRESULT res = f_mount(&fatfs, "", 0);
    if(f_mount(&fatfs, "", 0) == FR_OK){
        res = f_open(&file, "text.txt", FA_CREATE_ALWAYS | FA_WRITE);
        if(res == FR_OK){
            //res = f_read(&file, readData, 10, howMany);
            uint8_t wtext[] = "тестовая строка";
            unsigned int byteswritten;

            res = f_write(&file, wtext, sizeof(wtext), &byteswritten);
            f_close(&file);
        }
    }
    taskEXIT_CRITICAL();
    while(true){
        vTaskDelay(1000);
	}
}
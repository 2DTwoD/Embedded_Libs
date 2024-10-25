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

FATFS fatfs;
FIL file;

void testTask(void *pvParameters){
//    Result res = sdio.init();
//    uint32_t buf[1024]{1, 2, 3};
//    taskENTER_CRITICAL();
//    res = sdio.writeBlock(0, buf, 512);
//    fillArray(buf, 1024, (uint32_t)0);
//    res = sdio.readBlock(0, buf, 512);
//    taskEXIT_CRITICAL();
    taskENTER_CRITICAL();
    uint8_t readData[100]{0};
    UINT *howMany = nullptr;
    FRESULT res = f_mount(&fatfs, "", 0);
//    MKFS_PARM fmt_opt = {FM_FAT32, 0, 0, 0, 0};
//    BYTE work[FF_MAX_SS];
//    res = f_mkfs("0:", &fmt_opt, work, sizeof work);
    if(res == FR_OK){
        res = f_open(&file, "example.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
        if(res == FR_OK){
            res = f_read(&file, readData, 100, howMany);
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
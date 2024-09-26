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
    stringList.addBit("bit1", true);
    stringList.addBit("bit2", true);
    stringList.addFloat("float1", 1.1);
    stringList.addByte("byte3", 1);
    stringList.addWord("word1", 300);
    stringList.addBit("bit3", true);
    stringList.addBit("bit4", true);
    stringList.addBit("bit5", true);
    stringList.addBit("bit6", true);
    stringList.addBit("bit7", true);
    stringList.addBit("bit8", true);
    stringList.addBit("bit9", true);
	while(true){
        /*sprintf(mes, "first in buffer: %c; ", '1');
        uart.print(mes);*/
        /*taskENTER_CRITICAL();
        spi.sendAndReceive(send_data, 4, 11);
        taskEXIT_CRITICAL();*/
        uint8_t opa = stringList.getByte("opa");
        uint8_t opa2 = stringList.getByte("opa2");
        bool bit1 = stringList.getBit("bit1");
        bool bit2 = stringList.getBit("bit2");
        bool bit3 = stringList.getBit("bit3");
        bool bit8 = stringList.getBit("bit8");
        bool bit9 = stringList.getBit("bit9");
        float float1 = stringList.getFloat("float1");
        uint8_t byte3 = stringList.getByte("byte3");
        uint16_t word1 = stringList.getWord("word1");
        if(bit2){
            stringList.setBit("bit2", false);
            stringList.setBit("bit3", false);
            stringList.setFloat("float1", float1 + 0.12);
            stringList.addByte("opa2", 22);
        }else{
            stringList.setBit("bit2", true);
            stringList.setBit("bit3", true);
        }
		vTaskDelay(1000);
	}
}
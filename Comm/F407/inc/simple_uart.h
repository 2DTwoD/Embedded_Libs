#ifndef SIMPLE_UART_H
#define SIMPLE_UART_H
#include <cstdio>
#include "stm32f407xx.h"

#include "common.h"
#include "register.h"
#include "on_delay.h"
#include "buffer.h"
#include "gpio_common.h"

enum USARTbaudRate {
    _4800 = 4800,
    _9600 = 9600,
    _19200 = 19200,
    _38400 = 38400,
    _57600 = 57600,
    _115200 = 115200
};

enum USARTparity {
    NO_PARITY,
    EVEN,
    ODD
};

enum USARTstopBits {
    _1,
    _1_5,
    _2
};

class SimpleUART: private OnDelayCommon, public IUpdated1ms, public Buffer {
private:
    volatile USART_TypeDef* uart;
    uint16_t clickCount{0};
    static void adjustGPIO(GPIO_Info& uartGPIO, uint8_t AFcode);
public:
    SimpleUART(volatile USART_TypeDef *uart, GPIO_Info RxGPIO, GPIO_Info TxGPIO,
               uint32_t busFreq, USARTbaudRate baudRate, USARTparity parity, USARTstopBits stopBits,
               uint16_t bufferSize, uint32_t errorDelay);
    void sendByte(uint8_t byte);
    void send(uint8_t* bytes, uint16_t len);
    void print(char* message);
    void click();
    void IRQhandler();
    void update1ms() override;
};

#endif //SIMPLE_UART_H

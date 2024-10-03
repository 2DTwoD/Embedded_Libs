#include "simple_uart.h"
//For STM32F407
SimpleUART::SimpleUART(volatile USART_TypeDef *uart, GPIO_Info RxGPIO, GPIO_Info TxGPIO, uint32_t busFreqMHz,
                       USARTbaudRate baudRate, USARTparity parity, USARTstopBits stopBits, uint16_t bufferSize,
                       uint32_t errorDelay)
        : OnDelayCommon(errorDelay), uart(uart), InternalBuffer(bufferSize){
    //Сбросить все
    uart->CR1 = 0;
    uart->CR2 = 0;
    uart->CR3 = 0;
    uart->BRR = 0;
    uart->GTPR = 0;
    //RCC, IRQ set
    enableRCC(uart);
    ResultV<IRQn_Type> irqRes = getIRQ(uart);
    if(irqRes == rOK){
        NVIC_EnableIRQ(irqRes.getValue());
        NVIC_SetPriority(irqRes.getValue(), 3);
    }
    //define alternate function code
    GPIOafr AFcode = GPIO_AFR8;
    if(uart == USART1 || uart == USART2 || uart == USART3){
        AFcode = GPIO_AFR7;
    }
    //set GPIO
    adjustGPIO(RxGPIO, AFcode);
    adjustGPIO(TxGPIO, AFcode);
    //baudRate
    float div = busFreqMHz * 1000000 / (16.0f * baudRate);
    uint16_t mantissa = (uint16_t) div;
    uint8_t fraction = (uint8_t)((div - (float)mantissa) * 16.0f);
    setRegValShift(uart->BRR, USART_BRR_DIV_Mantissa_Msk, mantissa);
    setRegValShift(uart->BRR, USART_BRR_DIV_Fraction_Msk, fraction);
    //parity
    if(parity != NO_PARITY){
        setBit(uart->CR1, USART_CR1_PCE);
        setBit(uart->CR1, USART_CR1_M);
        if(parity == ODD){
            setBit(uart->CR1, USART_CR1_PS);
        }
    }
    //stop bits
    switch(stopBits){
        case _1_5:
            setRegValShift(uart->CR2, USART_CR2_STOP_Msk, 0b11);
            break;
        case _2:
            setRegValShift(uart->CR2, USART_CR2_STOP_Msk, 0b10);
            break;
        default:
            break;
    }
    //uart settings
    setBit(uart->CR1, USART_CR1_UE);
    setBit(uart->CR1, USART_CR1_RXNEIE);
    setBit(uart->CR1, USART_CR1_TE);
    setBit(uart->CR1, USART_CR1_RE);
}

void SimpleUART::adjustGPIO(GPIO_Info &uartGPIO, GPIOafr AFcode) {
    uartGPIO.pin = min((uint8_t )15, uartGPIO.pin);
    GPIOconfig gpio(uartGPIO);
    gpio.start().setMODER(GPIO_MODER_AF).setOSPEEDR(GPIO_OSPEEDR_VHIGH_SPEED).setAFR(AFcode).fin();
}

void SimpleUART::sendByte(uint8_t byte) {
    OnDelayCommon::again();
    while(!(uart->SR & USART_SR_TXE)){
        if(OnDelayCommon::get()) return;
    }
    uart->DR = byte;
}

void SimpleUART::print(const char* message) {
    while(*message != '\0'){
        sendByte(*message);
        message++;
    }
}

void SimpleUART::send(uint8_t *bytes, uint16_t len) {
    for (int i = 0; i < len; i++){
        sendByte(bytes[i]);
    }
}
void SimpleUART::click() {
    char mes[10];
    sprintf(mes, "click%d", clickCount++);
    print(mes);
}

void SimpleUART::IRQhandler() {
    if(uart->SR & USART_SR_RXNE) {
        InternalBuffer::addByte(uart->DR);
    }
}

void SimpleUART::update1ms() {
    OnDelayCommon::update();
}

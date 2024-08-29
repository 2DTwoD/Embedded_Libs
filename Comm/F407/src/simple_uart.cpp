#include "simple_uart.h"
//For STM32F407
SimpleUART::SimpleUART(volatile USART_TypeDef *uart, GPIO_Info RxGPIO, GPIO_Info TxGPIO, uint32_t busFreqMHz,
                       USARTbaudRate baudRate, USARTparity parity, USARTstopBits stopBits, uint16_t bufferSize,
                       uint32_t errorDelay)
        : OnDelayCommon(errorDelay), uart(uart), Buffer(bufferSize){
    //Сбросить все
    uart->CR1 = 0;
    uart->CR2 = 0;
    uart->CR3 = 0;
    uart->BRR = 0;
    uart->GTPR = 0;
    //RCC, IRQ set
    IRQn_Type irqType;
    if(uart == USART2){
        RCC->APB2ENR |= RCC_APB1ENR_USART2EN;
        irqType = USART2_IRQn;
    } else if(uart == USART3){
        RCC->APB2ENR |= RCC_APB1ENR_USART3EN;
        irqType = USART3_IRQn;
    } else if(uart == UART4){
        RCC->APB2ENR |= RCC_APB1ENR_UART4EN;
        irqType = UART4_IRQn;
    } else if(uart == UART5){
        RCC->APB2ENR |= RCC_APB1ENR_UART5EN;
        irqType = UART5_IRQn;
    } else if(uart == USART6){
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
        irqType = USART6_IRQn;
    } else {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        irqType = USART1_IRQn;
    }
    NVIC_EnableIRQ(irqType);
    NVIC_SetPriority(irqType, 10);
    //define alternate function code
    uint8_t AFcode;
    if(uart == USART1 || uart == USART2 || uart == USART3){
        //AF7
        AFcode = 0b0111;
    } else {
        //AF8
        AFcode = 0b1000;
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
        uart->CR1 |= USART_CR1_PCE;
        uart->CR1 |= USART_CR1_M;
        if(parity == ODD){
            uart->CR1 |= USART_CR1_PS;
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
    uart->CR1 |= USART_CR1_UE;
    uart->CR1 |= USART_CR1_RXNEIE;
    uart->CR1 |= USART_CR1_TE;
    uart->CR1 |= USART_CR1_RE;
}

void SimpleUART::adjustGPIO(GPIO_Info &uartGPIO, uint8_t AFcode) {
    //RCC
    if(uartGPIO.gpio == GPIOA){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    } else if(uartGPIO.gpio == GPIOB){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    } else if(uartGPIO.gpio == GPIOC){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    } else if(uartGPIO.gpio == GPIOD){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    } else if(uartGPIO.gpio == GPIOE){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    } else if(uartGPIO.gpio == GPIOF){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    } else if(uartGPIO.gpio == GPIOG){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    } else if(uartGPIO.gpio == GPIOH){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    } else if(uartGPIO.gpio == GPIOI){
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
    }
    //GPIO
    uartGPIO.pin = min((uint8_t )15, uartGPIO.pin);
    setRegValShift(uartGPIO.gpio->MODER, 0b11 << (uartGPIO.pin * 2), 0b10);
    setRegValShift(uartGPIO.gpio->OSPEEDR, 0b11 << (uartGPIO.pin * 2), 0b11);
    if(uartGPIO.pin < 8){
        setRegValShift(uartGPIO.gpio->AFR[0], 0b1111 << (uartGPIO.pin * 4), AFcode);
    } else {
        setRegValShift(uartGPIO.gpio->AFR[1], 0b1111 << ((uartGPIO.pin - 8) * 4), AFcode);
    }
}

void SimpleUART::sendByte(uint8_t byte) {
    OnDelayCommon::set(true);
    OnDelayCommon::reset();
    while(!(uart->SR & USART_SR_TXE)){
        if(OnDelayCommon::get()) return;
    }
    uart->DR = byte;
}

void SimpleUART::print(char* message) {
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
        Buffer::addByte(uart->DR);
    }
}

void SimpleUART::update1ms() {
    OnDelayCommon::update();
}

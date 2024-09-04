#include "simple_spi.h"

SimpleSPI::SimpleSPI(SPI_TypeDef *spi, GPIO_Info mosi, GPIO_Info miso, GPIO_Info clk,
                     uint16_t SPIfreqMHz, uint16_t busFreqMHz, POLPHA pol, POLPHA pha,
                     SPI_data dataFormat, SPI_frame frameFormat, Coil& nss, uint16_t bufferSize,
                     uint32_t errorDelay): spi(spi), Buffer(bufferSize), OnDelayCommon(errorDelay), nss(nss) {
    nss = true;
    uint8_t AFcode = 0b101;
    //Включить тактирование SPI
    if(spi == SPI2) {
        setBit(RCC->APB1ENR, RCC_APB1ENR_SPI2EN);
    } else if(spi == SPI3) {
        AFcode = 0b110;
        setBit(RCC->APB1ENR, RCC_APB1ENR_SPI3EN);
    } else {
        setBit(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
    }
    //Настройка mosi miso clk
    adjustGPIO(mosi, AFcode);
    adjustGPIO(miso, AFcode);
    adjustGPIO(clk, AFcode);
    //Настройка baud rate
    uint16_t brDiv[] = {2, 4, 8, 16, 32, 64, 128, 256};
    uint16_t brDivCode[] = {0b0, 0b1, 0b10, 0b11, 0b100, 0b101, 0b110, 0b111};
    uint8_t br;
    for(uint8_t i = 0; i < 8; i++){
        br = brDivCode[i];
        if(busFreqMHz / brDiv[i] <= SPIfreqMHz){
            break;
        }
    }
    //сброс регистров
    spi->CR1 = 0;
    spi->CR2 = 0;
    spi->I2SCFGR = 0;
    //Делитель частоты
    setRegValShift(spi->CR1, SPI_CR1_BR_Msk, br);
    //Полярность
    setRegValShift(spi->CR1, SPI_CR1_CPOL_Msk, pol);
    //Фаза
    setRegValShift(spi->CR1, SPI_CR1_CPHA_Msk, pha);
    //Формат
    setRegValShift(spi->CR1, SPI_CR1_DFF_Msk, dataFormat);
    //Frame format
    setRegValShift(spi->CR1, SPI_CR1_LSBFIRST_Msk, frameFormat);

    //SSM SSI установить, т.к. NSS меняется программно
    setBit(spi->CR1, SPI_CR1_SSM);
    setBit(spi->CR1, SPI_CR1_SSI);
    //Режим мастера
    setBit(spi->CR1, SPI_CR1_MSTR);
    //Включение прерываний
    setBit(spi->CR2, SPI_CR2_RXNEIE);
    //Включить шину
    setBit(spi->CR1, SPI_CR1_SPE);
}

void SimpleSPI::adjustGPIO(GPIO_Info gpioInfo, uint8_t AFcode) {
    //Включить тактирование порта
    if(gpioInfo.gpio == GPIOA){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    } else if(gpioInfo.gpio == GPIOB){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    } else if(gpioInfo.gpio == GPIOC){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
    } else if(gpioInfo.gpio == GPIOD){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
    } else if(gpioInfo.gpio == GPIOE){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN);
    } else if(gpioInfo.gpio == GPIOF){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);
    } else if(gpioInfo.gpio == GPIOG){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);
    } else if(gpioInfo.gpio == GPIOH){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN);
    } else if(gpioInfo.gpio == GPIOI){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN);
    }
    //В режиме AF
    setRegValShift(gpioInfo.gpio->MODER, 0b11 << (gpioInfo.pin * 2), 0b10);
    //Скорость пинов very high
    setRegValShift(gpioInfo.gpio->OSPEEDR, 0b11 << (gpioInfo.pin * 2), 0b11);
    //Режим альтернативной функции AF5
    if(gpioInfo.pin < 8){
        setRegValShift(gpioInfo.gpio->AFR[0], 0b1111 << (gpioInfo.pin * 4), AFcode);
    } else {
        setRegValShift(gpioInfo.gpio->AFR[1], 0b1111 << ((gpioInfo.pin - 8) * 4), AFcode);
    }
}

void SimpleSPI::update1ms() {
    OnDelayCommon::update();
}

void SimpleSPI::newNss(Coil &newNss) {
    nss = true;
    nss = newNss;
}

bool SimpleSPI::write(uint8_t *data, uint16_t len) {
    if(bitIsZero(spi->SR, SPI_SR_BSY)){
        for(uint16_t i = 0; i < len; i++){
            spi->DR = data[i];
            OnDelayCommon::again();
            while(bitIsZero(spi->SR, SPI_SR_TXE)){
                if(OnDelayCommon::get()) return false;
            }
        }
        OnDelayCommon::again();
        while(bitIsOne(spi->SR, SPI_SR_BSY)){
            if(OnDelayCommon::get()) return false;
        }
        return true;
    }
    return false;
}

bool SimpleSPI::read(uint16_t len) {
    if(bitIsZero(spi->SR, SPI_SR_BSY)){
        //Если статус перегрузки или RX не пуст, прочитаем DR, чтобы сбросить статусы
        if(bitIsOne(spi->SR, SPI_SR_OVR) || bitIsOne(spi->SR, SPI_SR_RXNE)){
            spi->DR;
        }
        for(uint16_t i = 0; i < len; i++){
            spi->DR = 0;
            //Ожидание пока RX будет не пуст
            OnDelayCommon::again();
            while(bitIsZero(spi->SR, SPI_SR_RXNE)){
                if(OnDelayCommon::get()) return false;
            }
            //Копируем данные в буфер
            Buffer::addByte(spi->DR);
        }
        //Ожидание пока шина занята
        OnDelayCommon::again();
        while(bitIsOne(spi->SR, SPI_SR_BSY)){
            if(OnDelayCommon::get()) return false;
        }
        return true;
    }
    return false;
}

bool SimpleSPI::send(uint8_t *src, uint16_t srcLen) {
    nss = false;
    bool result = write(src, srcLen);
    nss = true;
    return result;
}

bool SimpleSPI::send(uint8_t value) {
    uint8_t container[1] = {value};
    return send(container, 1);
}
bool SimpleSPI::receive(uint16_t readLen) {
    nss = false;
    bool result = read(readLen);
    nss = true;
    return result;
}

bool SimpleSPI::receive() {
    return receive(1);
}

bool SimpleSPI::sendAndReceive(uint8_t* src, uint16_t srcLen, uint16_t readLen) {
    nss = false;
    bool result = write(src, srcLen);
    if(result){
        result = read(readLen);
    }
    nss = true;
    return result;
}

bool SimpleSPI::sendAndReceive(uint8_t value, uint16_t readLen) {
    uint8_t container[1] = {value};
    return sendAndReceive(container, 1, readLen);
}

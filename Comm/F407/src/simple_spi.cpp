#include "simple_spi.h"

SimpleSPI::SimpleSPI(SPI_TypeDef *spi, GPIO_Info mosi, GPIO_Info miso, GPIO_Info clk,
                     uint16_t SPIfreqMHz, uint16_t busFreqMHz, POLPHA pol, POLPHA pha,
                     SPI_data dataFormat, SPI_frame frameFormat, Coil& nss, uint16_t bufferSize, uint32_t errorDelay):
        spi(spi), InternalBuffer(bufferSize), OnDelayCommon(errorDelay), nss(nss), dataFormat(dataFormat) {
    nss = true;
    //Номер AF
    GPIOafr AFcode = (spi == SPI3)? GPIO_AFR6: GPIO_AFR5;
    //Включить тактирование SPI
    enableRCC(spi);
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
    //Сброс регистров
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
    //setBit(spi->CR2, SPI_CR2_RXNEIE);
    //Включить шину
    setBit(spi->CR1, SPI_CR1_SPE);
}

void SimpleSPI::adjustGPIO(GPIO_Info gpioInfo, GPIOafr AFcode) {
    GPIOconfig gpio(gpioInfo);
    gpio.start().setMODER(GPIO_MODER_AF).setOSPEEDR(GPIO_OSPEEDR_VHIGH_SPEED).setAFR(AFcode).fin();
}

void SimpleSPI::update1ms() {
    OnDelayCommon::update();
}

void SimpleSPI::newNss(Coil &newNss) {
    nss = true;
    nss = newNss;
}

bool SimpleSPI::write(uint8_t *const data, uint16_t len) {
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


bool SimpleSPI::write(uint16_t *const data, uint16_t len) {
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
            if(dataFormat == _16BIT){
                uint16_t data = spi->DR;
                InternalBuffer::addByte(data >> 8);
                InternalBuffer::addByte(data & 0xFF);
            } else {
                InternalBuffer::addByte(spi->DR);
            }
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

bool SimpleSPI::send(uint8_t *const src, uint16_t srcLen) {
    nss = false;
    bool result = write(src, srcLen);
    nss = true;
    return result;
}

bool SimpleSPI::send(uint8_t value) {
    uint8_t container[1] = {value};
    return send(container, 1);
}

bool SimpleSPI::send(uint16_t *const src, uint16_t srcLen) {
    nss = false;
    bool result = write(src, srcLen);
    nss = true;
    return result;
}

bool SimpleSPI::send(uint16_t value) {
    uint16_t container[1] = {value};
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

bool SimpleSPI::sendAndReceive(uint8_t *const src, uint16_t srcLen, uint16_t readLen) {
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

bool SimpleSPI::sendAndReceive(uint16_t *const src, uint16_t srcLen, uint16_t readLen) {
    nss = false;
    bool result = write(src, srcLen);
    if(result){
        result = read(readLen);
    }
    nss = true;
    return result;
}

bool SimpleSPI::sendAndReceive(uint16_t value, uint16_t readLen) {
    uint16_t container[1] = {value};
    return sendAndReceive(container, 1, readLen);
}

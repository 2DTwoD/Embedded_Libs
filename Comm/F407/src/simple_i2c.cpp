#include "simple_i2c.h"

SimpleI2C::SimpleI2C(I2C_TypeDef *i2c, GPIO_Info sda, GPIO_Info scl, uint8_t busFreqMHz,
                     I2C_Mode mode, I2C_Duty duty, uint16_t bufferSize, uint32_t errorDelay):
        OnDelayCommon(errorDelay), InternalBuffer(bufferSize), i2c(i2c), sda(sda), scl(scl) {
    //Настройка тактирования (RCC)
    enableRCC(i2c);
    //Настройка GPIO
    adjustGPIO(sda);
    adjustGPIO(scl);
    //Сброс I2C
    resetI2C();
    //Сбросить лишнее в 0
    i2c->CR1 = 0;
    i2c->CR2 = 0;
    i2c->OAR1 = 0;
    i2c->OAR2 = 0;
    //Частота (такая же как на шине)
    busFreqMHz = limit(busFreqMHz, (uint8_t)2, (uint8_t)50);
    setRegValShift(i2c->CR2, I2C_CR2_FREQ_Msk, busFreqMHz);
    //Для расчета trise
    float tpclk = 1000.0f / busFreqMHz;
    uint16_t trise;
    //ccr
    uint16_t ccr;
    //Fast or Slow mode
    if(mode == FM){
        setBit(i2c->CCR, I2C_CCR_FS);
        trise = round(300 / tpclk) + 1;
        //Duty (Tlow/Thigh)
        if(duty == _16_9){
            setBit(i2c->CCR, I2C_CCR_DUTY);
            ccr = busFreqMHz / 10;
        } else {
            resetBit(i2c->CCR, I2C_CCR_DUTY);
            ccr = (busFreqMHz * 10) / 12;
        }
    } else {
        resetBit(i2c->CCR, I2C_CCR_FS);
        ccr = busFreqMHz * 5;
        trise = round(1000 / tpclk) + 1;
    }
    //установить рассчитанное ccr
    setRegValShift(i2c->CCR, I2C_CCR_CCR_Msk, ccr);
    //установить рассчитанное trice
    setRegValShift(i2c->TRISE, I2C_TRISE_TRISE_Msk, trise);
    //Запустить I2C
    setBit(i2c->CR1, I2C_CR1_PE);
}

void SimpleI2C::adjustGPIO(GPIO_Info gpioInfo) {
    GPIOconfig gpio(gpioInfo);
    gpio.start().setMODER(GPIO_MODER_AF).setOTYPER(GPIO_OTYPER_OPEN_DRAIN).setOSPEEDR(GPIO_OSPEEDR_VHIGH_SPEED)
    //.setPUPDR(GPIO_PUPDR_PULL_UP)
    .setAFR(GPIO_AFR4).fin();
}

void SimpleI2C::resetI2C() {
    setBit(i2c->CR1, I2C_CR1_SWRST);
    while(bitIsZero(i2c->CR1, I2C_CR1_SWRST));
    resetBit(i2c->CR1, I2C_CR1_SWRST);
    while(bitIsOne(i2c->CR1, I2C_CR1_SWRST));
}

void SimpleI2C::stopBus() {
    setBit(i2c->CR1, I2C_CR1_STOP);
}

void SimpleI2C::resetAF() {
    resetBit(i2c->SR1, I2C_SR1_AF);
}

void SimpleI2C::resetPE() {
    resetBit(i2c->CR1, I2C_CR1_PE);
    setBit(i2c->CR1, I2C_CR1_PE);
}

void SimpleI2C::solveBusy() {
    if(bitIsOneByPos(sda.gpio->IDR, sda.pin) && bitIsOneByPos(scl.gpio->IDR, scl.pin)){
        resetI2C();
    }
    if(bitIsOne(i2c->SR2, I2C_SR2_MSL)) {
        stopBus();
    }
    if(i2c->CR1 != 1) {
        resetPE();
    }
}

bool SimpleI2C::startBus(uint8_t address, I2C_RW rw) {
    //Ожидание пока шина освободится
    OnDelayCommon::again();
    while(bitIsOne(i2c->SR2, I2C_SR2_BUSY)){
        if(OnDelayCommon::get()){
            solveBusy();
            return false;
        }
    }
    //Старт шины
    resetBit(i2c->CR1, I2C_CR1_POS);
    setBit(i2c->CR1, I2C_CR1_START);
    //Ожидание start
    OnDelayCommon::again();
    while(bitIsZero(i2c->SR1, I2C_SR1_SB)){
        if(OnDelayCommon::get()){
            return false;
        }
    }
    //Сброс SB путем чтения SR1 и записи в DR адрес и команду read/write(1/0)
    i2c->SR1;
    i2c->DR = (address << 1) | rw;
    //Ожидание ответа от адреса
    OnDelayCommon::again();
    while(bitIsZero(i2c->SR1, I2C_SR1_AF) && bitIsZero(i2c->SR1, I2C_SR1_ADDR)){
        if(OnDelayCommon::get()) {
            return false;
        }
    }
    if(bitIsOne(i2c->SR1, I2C_SR1_ADDR)) {//Устройство отозвалось
        //Сброс ADDR
        i2c->SR1;
        i2c->SR2;
        return true;
    } else {//Устройство не отозвалось (AF = 1)
        stopBus();
        resetAF();
        return false;
    }
}

void SimpleI2C::update1ms() {
    OnDelayCommon::update();
}

bool SimpleI2C::write(uint8_t address, uint8_t *const src, uint16_t len) {
    bool result = true;
    for(uint8_t i = 0; i < len; i++){
        //Запись данных в DR
        i2c->DR = src[i];
        //Ожидание пока данные перейдут в регистр сдвига
        while(bitIsZero(i2c->SR1, I2C_SR1_TXE)){
            if(bitIsOne(i2c->SR1, I2C_SR1_AF)){
                result = false;
                resetAF();
                break;
            }
        }
    }
    stopBus();
    return result;
}

bool SimpleI2C::read(uint8_t address, uint16_t len) {
    for(uint8_t i = 0; i < len; i++){
        if(i < len - 1) {
            //Если не последний байт, то отправляем ACK
            setBit(i2c->CR1, I2C_CR1_ACK);
        } else {
            //Последний байт, выключаем подтверждение
            resetBit(i2c->CR1, I2C_CR1_ACK);
            //Стоп
            stopBus();
        }
        //Ожидание данных в сдвиговом регистре
        OnDelayCommon::again();
        while(bitIsZero(i2c->SR1, I2C_SR1_RXNE)){
            if(OnDelayCommon::get()) {
                stopBus();
                return false;
            }
        }
        //Считать данные в буфер
        InternalBuffer::addByte(i2c->DR);
    }
    return true;
}

bool SimpleI2C::slaveIsExist(uint8_t address) {
    bool result = startBus(address, WRITE);
    stopBus();
    resetAF();
    return result;
}

void SimpleI2C::getSlaves(uint8_t *const dst, uint8_t len) {
    uint8_t index = 0;
    fillArray(dst, len, (uint8_t)0);
    for(uint8_t i = 0; i < 128; i++){
        if(slaveIsExist(i)) {
            dst[index++] = i;
            if(index >= len) break;
        }
    }
}

bool SimpleI2C::send(uint8_t address, uint8_t *const src, uint16_t len) {
    if(startBus(address, WRITE)){
        return write(address, src, len);
    }
    return false;
}

bool SimpleI2C::send(uint8_t address, uint8_t value) {
    uint8_t container[1] = {value};
    return send(address, container, 1);
}

bool SimpleI2C::receive(uint8_t address, uint16_t len) {
    if(startBus(address, READ)){
        return read(address, len);
    }
    return false;
}

bool SimpleI2C::receive(uint8_t address) {
    return receive(address, 1);
}

bool SimpleI2C::sendAndReceive(uint8_t address, uint8_t *const writeSrc, uint16_t writeLen, uint16_t readLen) {
    if(send(address, writeSrc, writeLen)){
        return receive(address, readLen);
    }
    return false;
}

bool SimpleI2C::sendAndReceive(uint8_t address, uint16_t writeValue, uint16_t readLen) {
    if(send(address, writeValue)){
        return receive(address, readLen);
    }
    return false;
}
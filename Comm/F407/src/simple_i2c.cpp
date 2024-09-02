#include "simple_i2c.h"

SimpleI2C::SimpleI2C(I2C_TypeDef *i2c, GPIO_Info sda, GPIO_Info scl, uint8_t busFreqMHz,
                     I2C_Mode mode, I2C_Duty duty, uint16_t bufferSize, uint32_t errorDelay):
                     OnDelayCommon(errorDelay), Buffer(bufferSize), i2c(i2c), sda(sda), scl(scl) {
    if(i2c == I2C2){
        setBit(RCC->APB1ENR, RCC_APB1ENR_I2C2EN_Pos);
    } else if(i2c == I2C3){
        setBit(RCC->APB1ENR, RCC_APB1ENR_I2C3EN_Pos);
    } else {
        setBit(RCC->APB1ENR, RCC_APB1ENR_I2C1EN_Pos);
    }
    //GPIO
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
        setBit(i2c->CCR, I2C_CCR_FS_Pos);
        trise = round(300 / tpclk) + 1;
        //Duty (Tlow/Thigh)
        if(duty == _16_9){
            setBit(i2c->CCR, I2C_CCR_DUTY_Pos);
            ccr = busFreqMHz / 10;
        } else {
            resetBit(i2c->CCR, I2C_CCR_DUTY_Pos);
            ccr = (busFreqMHz * 10) / 12;
        }

    } else {
        resetBit(i2c->CCR, I2C_CCR_FS_Pos);
        ccr = busFreqMHz * 5;
        trise = round(1000 / tpclk) + 1;
    }
    //установить рассчитанное ccr
    setRegValShift(i2c->CCR, I2C_CCR_CCR_Msk, ccr);
    //установить рассчитанное trice
    setRegValShift(i2c->TRISE, I2C_TRISE_TRISE_Msk, trise);
    //Запустить I2C
    setBit(i2c->CR1, I2C_CR1_PE_Pos);
}

void SimpleI2C::adjustGPIO(GPIO_Info gpioInfo) {
    //Тактирование
    if(gpioInfo.gpio == GPIOA){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN_Pos);
    } else if(gpioInfo.gpio == GPIOB){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN_Pos);
    } else if(gpioInfo.gpio == GPIOC){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN_Pos);
    } else if(gpioInfo.gpio == GPIOD){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN_Pos);
    } else if(gpioInfo.gpio == GPIOE){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN_Pos);
    } else if(gpioInfo.gpio == GPIOF){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN_Pos);
    } else if(gpioInfo.gpio == GPIOG){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN_Pos);
    } else if(gpioInfo.gpio == GPIOH){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN_Pos);
    } else if(gpioInfo.gpio == GPIOI){
        setBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN_Pos);
    }
    //В режиме AF
    setRegValShift(gpioInfo.gpio->MODER, (0b11 << (gpioInfo.pin * 2)), 0b10);
    //Выход Open-drain
    setBit(gpioInfo.gpio->OTYPER, gpioInfo.pin);
    //Very high speed
    setRegValShift(gpioInfo.gpio->OSPEEDR, (0b11 << (gpioInfo.pin * 2)), 0b11);
    //Pull up
    //setRegValShift(gpioInfo.gpio->PUPDR, (0b11 << (gpioInfo.pin * 2)), 0b01);
    //AF4
    if(gpioInfo.pin < 8) {
        setRegValShift(gpioInfo.gpio->AFR[0], (0b1111 << (gpioInfo.pin * 4)), 0b100);
    } else {
        setRegValShift(gpioInfo.gpio->AFR[1], (0b1111 << ((gpioInfo.pin - 8) * 4)), 0b100);
    }
}

void SimpleI2C::resetI2C() {
    setBit(i2c->CR1, I2C_CR1_SWRST_Pos);
    while(bitIsZero(i2c->CR1, I2C_CR1_SWRST_Pos));
    resetBit(i2c->CR1, I2C_CR1_SWRST_Pos);
    while(bitIsOne(i2c->CR1, I2C_CR1_SWRST_Pos));
}

bool SimpleI2C::commonRW(uint8_t address, I2C_RW rw,
                         bool (*action)(SimpleI2C* ths, uint8_t *const, uint8_t), uint8_t *const src, uint8_t len) {
    if(bitIsOne(i2c->SR2, I2C_SR2_BUSY_Pos)){
        solveProblem();
        return false;
    }
    //ACK bit controls the (N)ACK of the current byte being received in the shift register.
    resetBit(i2c->CR1, I2C_CR1_POS_Pos);
    //Старт шины
    setBit(i2c->CR1, I2C_CR1_START_Pos);

    OnDelayCommon::again();
    //Ожидание start
    while(bitIsZero(i2c->SR1, I2C_SR1_SB_Pos)){
        if(OnDelayCommon::get()) {
            solveProblem();
            return false;
        }
    }
    //Сброс SB путем чтения SR1 и записи DR
    i2c->SR1;
    //Записать в DR адрес и команду read/write(1/0)
    i2c->DR = (address << 1) | rw;
    OnDelayCommon::again();
    //Ожидание ответа от адреса
    while(bitIsZero(i2c->SR1, I2C_SR1_AF_Pos) && bitIsZero(i2c->SR1, I2C_SR1_ADDR_Pos)){
        if(OnDelayCommon::get()) {
            solveProblem();
            return false;
        }
    }
    if(bitIsOne(i2c->SR1, I2C_SR1_ADDR_Pos)){//Устройство отозвалось
        //Сброс ADDR
        i2c->SR1;
        i2c->SR2;
        //Сделать что-нибудь
        if(action(this, src, len)){
            //Стоп
            setBit(i2c->CR1, I2C_CR1_STOP_Pos);
            return true;
        } else {
            solveProblem();
        }
    } else {//Устройство не отозвалось (AF = 1)
        //Стоп
        setBit(i2c->CR1, I2C_CR1_STOP_Pos);
        //Сброс AF
        resetBit(i2c->SR1, I2C_SR1_AF_Pos);
    }
    return false;
}

bool SimpleI2C::slaveIsExist(uint8_t address) {
    return commonRW(address, WRITE, [](SimpleI2C*, uint8_t *const, uint8_t) -> bool{ return true;},
                    nullptr, 0);
}

void SimpleI2C::getSlaves(uint8_t *const dst, uint8_t len) {
    uint8_t index = 0;
    fillArray(dst, len, (uint8_t )1);
    for(uint8_t i = 0; i < 128; i++){
        if(slaveIsExist(i)) {
            dst[index++] = i;
            if(index >= len) break;
        }
    }
}

void SimpleI2C::update1ms() {
    OnDelayCommon::update();
}

void SimpleI2C::solveProblem() {
    if(bitIsOne(sda.gpio->IDR, (1 << sda.pin)) && bitIsOne(scl.gpio->IDR, (1 << scl.pin))){
        resetI2C();
        return;
    }
    if(bitIsOne(i2c->SR2, I2C_SR2_MSL_Pos)) {
        setBit(i2c->CR1, I2C_CR1_STOP_Pos);
        return;
    }
    toggleBitTwice(i2c->CR1, I2C_CR1_PE_Pos);
}

void SimpleI2C::transmit(uint8_t address, uint8_t *const src, uint16_t len) {
    commonRW(address, WRITE, [](SimpleI2C* ths, uint8_t *const src, uint8_t len) -> bool{
        for(uint8_t i = 0; i < len; i++){
            ths->i2c->DR = src[i];
            //Ожидание пока данные перейдут в регистр сдвига
            while(bitIsZero(ths->i2c->SR1, I2C_SR1_TXE_Pos)){
                if(bitIsOne(ths->i2c->SR1, I2C_SR1_AF_Pos)){
                    //Стоп
                    setBit(ths->i2c->CR1, I2C_CR1_STOP_Pos);
                    //Сброс AF
                    resetBit(ths->i2c->SR1, I2C_SR1_AF_Pos);
                    return false;
                }
            }
        }
        return true;
        }, src, len);
}

void SimpleI2C::receive(uint8_t address, uint16_t len) {
    commonRW(address, READ, [](SimpleI2C* ths, uint8_t *const src, uint8_t len) -> bool{
        for(uint8_t i = 0; i < len; i++){
            if(i < len -1) {
                //Если не последний байт, то отправляем ACK
                setBit(ths->i2c->CR1, I2C_CR1_ACK_Pos);
            } else {
                //Последний байт, выключаем подтверждение
                resetBit(ths->i2c->CR1, I2C_CR1_ACK_Pos);
                //Стоп
                setBit(ths->i2c->CR1, I2C_CR1_STOP_Pos);
            }
            ths->again();
            //Ожидание данных в сдвиговом регистре
            while(bitIsZero(ths->i2c->SR1, I2C_SR1_RXNE_Pos)){
                if(ths->get()) return false;
            }
            //Считать данные в буфер
            ths->addByte(ths->i2c->DR);
        }
        return true;
    }, nullptr, len);
}

void SimpleI2C::transmit(uint8_t address, uint8_t value) {
    commonRW(address, WRITE, [](SimpleI2C* ths, uint8_t *const src, uint8_t len) -> bool{
            ths->i2c->DR = src[value];
            //Ожидание пока данные перейдут в регистр сдвига
            while(bitIsZero(ths->i2c->SR1, I2C_SR1_TXE_Pos)){
                if(bitIsOne(ths->i2c->SR1, I2C_SR1_AF_Pos)){
                    //Стоп
                    setBit(ths->i2c->CR1, I2C_CR1_STOP_Pos);
                    //Сброс AF
                    resetBit(ths->i2c->SR1, I2C_SR1_AF_Pos);
                    return false;
                }
            }
        return true;
    }, nullptr, 0);
}

void SimpleI2C::receive(uint8_t address) {
    commonRW(address, READ, [](SimpleI2C* ths, uint8_t *const src, uint8_t len) -> bool{
        //Последний байт, выключаем подтверждение
        resetBit(ths->i2c->CR1, I2C_CR1_ACK_Pos);
        //Стоп
        setBit(ths->i2c->CR1, I2C_CR1_STOP_Pos);
        ths->again();
        //Ожидание данных в сдвиговом регистре
        while(bitIsZero(ths->i2c->SR1, I2C_SR1_RXNE_Pos)){
            if(ths->get()) return false;
        }
        //Считать данные в буфер
        ths->addByte(ths->i2c->DR);
        return true;
    }, nullptr, 0);
}

bool SimpleI2C::startBus(uint8_t address, I2C_RW rw) {
    if(bitIsOne(i2c->SR2, I2C_SR2_BUSY_Pos)){
        solveProblem();
        return false;
    }
    //ACK bit controls the (N)ACK of the current byte being received in the shift register.
    resetBit(i2c->CR1, I2C_CR1_POS_Pos);
    //Старт шины
    setBit(i2c->CR1, I2C_CR1_START_Pos);

    OnDelayCommon::again();
    //Ожидание start
    while(bitIsZero(i2c->SR1, I2C_SR1_SB_Pos)){
        if(OnDelayCommon::get()) {
            solveProblem();
            return false;
        }
    }
    //Сброс SB путем чтения SR1 и записи DR
    i2c->SR1;
    //Записать в DR адрес и команду read/write(1/0)
    i2c->DR = (address << 1) | rw;
    OnDelayCommon::again();
    //Ожидание ответа от адреса
    while(bitIsZero(i2c->SR1, I2C_SR1_AF_Pos) && bitIsZero(i2c->SR1, I2C_SR1_ADDR_Pos)){
        if(OnDelayCommon::get()) {
            solveProblem();
            return false;
        }
    }
    if(bitIsOne(i2c->SR1, I2C_SR1_ADDR_Pos)){//Устройство отозвалось
        return true;
    } else {//Устройство не отозвалось (AF = 1)
        //Стоп
        setBit(i2c->CR1, I2C_CR1_STOP_Pos);
        //Сброс AF
        resetBit(i2c->SR1, I2C_SR1_AF_Pos);
        return false;
    }
}

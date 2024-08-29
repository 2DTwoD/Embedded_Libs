#include "simple_i2c.h"

SimpleI2C::SimpleI2C(I2C_TypeDef *i2c, GPIO_Info sda, GPIO_Info scl, uint8_t busFreqMHz,
                     I2C_Mode mode, I2C_Duty duty): i2c(i2c) {
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
    float tpclk = 1000.0 / busFreqMHz;
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
            ccr = (busFreqMHz * 10) / 100;
        } else {
            resetBit(i2c->CCR, I2C_CCR_DUTY_Pos);
            ccr = (busFreqMHz * 10) / 12;
        }

    } else {
        resetBit(i2c->CCR, I2C_CCR_FS_Pos);
        ccr = busFreqMHz * 5;
        trise = round(1000 / tpclk)+ 1;
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
    setBit(gpioInfo.gpio->OTYPER, (1 << gpioInfo.pin));
    //Very high speed
    setRegValShift(gpioInfo.gpio->OSPEEDR, (0b11 << (gpioInfo.pin * 2)), 0b11);
    //AF4
    if(gpioInfo.pin < 8) {
        setRegValShift(gpioInfo.gpio->AFR[0], (0b1111 << (gpioInfo.pin * 4)), 0b100);
    } else {
        setRegValShift(gpioInfo.gpio->AFR[1], (0b1111 << ((gpioInfo.pin - 8) * 4)), 0b100);
    }
}

void SimpleI2C::resetI2C() {
    i2c->CR1 |= I2C_CR1_SWRST_Pos;
    while(!(i2c->CR1 & I2C_CR1_SWRST_Pos));
    i2c->CR1 ^= I2C_CR1_SWRST_Pos;
    while(i2c->CR1 & I2C_CR1_SWRST_Pos);
}

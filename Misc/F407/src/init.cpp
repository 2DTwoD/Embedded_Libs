#include "init.h"
#include "coil.h"
#include "register.h"

extern Coil spi2Nss;

void commonInit(){
	NVIC_SetPriorityGrouping(4);
}

void rccInit(){
    Register<uint32_t> RCC_CR(RCC->CR);
    Register<uint32_t> RCC_PLLCFGR(RCC->PLLCFGR);
    Register<uint32_t> RCC_CFGR(RCC->CFGR);

    //Включаем HSE
    RCC_CR |= RCC_CR_HSEON_Pos;
    while(!RCC_CR.getBit(RCC_CR_HSERDY_Pos));

    //Устанавливаем divP mulN divM
    RCC_PLLCFGR = {RCC_PLLCFGR_PLLP_Msk, 0};
    RCC_PLLCFGR = {RCC_PLLCFGR_PLLN_Msk, 336};
    RCC_PLLCFGR = {RCC_PLLCFGR_PLLM_Msk, 25};

    //Источник будет HSE
    RCC_PLLCFGR |= RCC_PLLCFGR_PLLSRC_Pos;

    //Разрешаем PLL
    RCC_CR |= RCC_CR_PLLON_Pos;
    while(!RCC_CR.getBit(RCC_CR_PLLRDY_Pos));

    //These bits represent the ratio of the CPU clock period to the flash memory access time
    FLASH->ACR |= FLASH_ACR_LATENCY_5WS;

    //Делители для шин
    RCC_CFGR = {RCC_CFGR_HPRE_Msk, 0};
    RCC_CFGR = {RCC_CFGR_PPRE1_Msk, 0b101};
    RCC_CFGR = {RCC_CFGR_PPRE2_Msk, 0b100};

    //Выбор System clock
    RCC_CFGR = {RCC_CFGR_SW_Msk, 0b10};
    while(RCC_CFGR.getValue(RCC_CFGR_SWS_Msk) != 0b10);

    //Выключаем HSI
    RCC_CR ^= RCC_CR_HSION_Pos;
}

void tickInit(TIM_TypeDef* tim, uint16_t busFreqMHz){
    IRQn_Type irqType;
    if(tim == TIM1){
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
        irqType = TIM1_CC_IRQn;
    } else if(tim == TIM2){
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
        irqType = TIM2_IRQn;
    } else if(tim == TIM3){
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
        irqType = TIM3_IRQn;
    } else if(tim == TIM4){
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
        irqType = TIM4_IRQn;
    } else if(tim == TIM5){
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
        irqType = TIM5_IRQn;
    } else if(tim == TIM6){
        RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
        irqType = TIM6_DAC_IRQn;
    } else if(tim == TIM7){
        RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
        irqType = TIM7_IRQn;
    } else if(tim == TIM8){
        RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
        irqType = TIM8_CC_IRQn;
    } else if(tim == TIM9){
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
        irqType = TIM1_BRK_TIM9_IRQn;
    } else if(tim == TIM10){
        RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
        irqType = TIM1_UP_TIM10_IRQn;
    } else if(tim == TIM11){
        RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
        irqType = TIM1_TRG_COM_TIM11_IRQn;
    } else if(tim == TIM12){
        RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
        irqType = TIM8_BRK_TIM12_IRQn;
    } else if(tim == TIM13){
        RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
        irqType = TIM8_UP_TIM13_IRQn;
    } else if(tim == TIM14){
        RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
        irqType = TIM8_TRG_COM_TIM14_IRQn;
    } else {
        return;
    }
	tim->PSC = busFreqMHz;
	tim->ARR = 1000;
	tim->DIER |= TIM_DIER_UIE;
	tim->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(irqType);
	NVIC_SetPriority(irqType, 6);
}

void spiInit() {
    Register<uint32_t> RCC_AHB1ENR(RCC->AHB1ENR);
    Register<uint32_t> RCC_APB1ENR(RCC->APB1ENR);
    RCC_AHB1ENR.setBit(RCC_AHB1ENR_GPIOBEN_Pos);
    RCC_AHB1ENR.setBit(RCC_AHB1ENR_GPIOCEN_Pos);
    RCC_APB1ENR.setBit(RCC_APB1ENR_SPI2EN_Pos);
    //Тактирование разрешить SPI, GPIO
    /*RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;*/
    //RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    //Скорость пинов very high
    setRegisterWithAutoShift(&GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED10_Msk, 0b11);
    setRegisterWithAutoShift(&GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED2_Msk, 0b11);
    setRegisterWithAutoShift(&GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED3_Msk, 0b11);
    //Режим альтернативной функции AF5
    setRegisterWithAutoShift(&GPIOB->AFR[1], GPIO_AFRH_AFSEL10_Msk, 0b101);
    setRegisterWithAutoShift(&GPIOC->AFR[0], GPIO_AFRL_AFSEL2_Msk, 0b101);
    setRegisterWithAutoShift(&GPIOC->AFR[0], GPIO_AFRL_AFSEL3_Msk, 0b101);
    //Делитель частоты (/32)
    setRegisterWithAutoShift(&SPI2->CR1, SPI_CR1_BR_Msk, 0b100);
    //Полярность и фаза (0, 0)
    setRegister(&SPI2->CR1, SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk, 0);
    //Формат (8 бит)
    setRegisterWithAutoShift(&SPI2->CR1, SPI_CR1_DFF_Msk, 0);
    //MSB сначала
    setRegisterWithAutoShift(&SPI2->CR1, SPI_CR1_LSBFIRST_Msk, 0);
    //SSM SSI утсанавливаю, т.к. программно меняю NSS
    setRegisterWithAutoShift(&SPI2->CR1, SPI_CR1_SSM_Msk | SPI_CR1_SSI_Msk, 0b11);
    //MSTR и SPE установить при установленном NSS
    spi2Nss = true;
    SPI2->CR1 |= SPI_CR1_MSTR;
    SPI2->CR1 |= SPI_CR1_SPE;
    spi2Nss = false;
}

void i2cInit() {
    Register<uint32_t> RCC_AHB1ENR(RCC->AHB1ENR);
    Register<uint32_t> RCC_APB1ENR(RCC->APB1ENR);
    Register<uint32_t> GPIOB_MODER(GPIOB->MODER);
    Register<uint32_t> GPIOB_OTYPER(GPIOB->OTYPER);
    Register<uint32_t> GPIOB_OSPEEDR(GPIOB->OSPEEDR);
    Register<uint32_t> GPIOB_AFRH(GPIOB->AFR[1]);
    Register<uint32_t> I2C_CR1(I2C1->CR1);
    Register<uint32_t> I2C_CR2(I2C1->CR2);
    Register<uint32_t> I2C_CCR(I2C1->CCR);
    Register<uint32_t> I2C_TRISE(I2C1->TRISE);

    //Тактирование
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN_Pos;
    RCC_APB1ENR |= RCC_APB1ENR_I2C1EN_Pos;

    //В режиме AF
    GPIOB_MODER = {GPIO_MODER_MODE8_Msk, 0b10};
    GPIOB_MODER = {GPIO_MODER_MODE9_Msk, 0b10};

    //Выход Open-drain
    GPIOB_OTYPER |= GPIO_OTYPER_OT8_Pos;
    GPIOB_OTYPER |= GPIO_OTYPER_OT9_Pos;

    //Very high speed
    GPIOB_OSPEEDR = {GPIO_OSPEEDR_OSPEED8_Msk, 0b11};
    GPIOB_OSPEEDR = {GPIO_OSPEEDR_OSPEED9_Msk, 0b11};

    //AF4
    GPIOB_AFRH = {GPIO_AFRH_AFSEL8_Msk, 0b100};
    GPIOB_AFRH = {GPIO_AFRH_AFSEL9_Msk, 0b100};

    I2C_CR1 |= I2C_CR1_SWRST_Pos;
    while(I2C_CR1.isZero(I2C_CR1_SWRST_Pos));

    I2C_CR1 ^= I2C_CR1_SWRST_Pos;
    while(I2C_CR1.isOne(I2C_CR1_SWRST_Pos));

    I2C_CR1 = 0;
    I2C_CR2 = 0;
    I2C_CR2 = {I2C_CR2_FREQ_Msk, 42};
    I2C1->OAR1 = 0;
    I2C1->OAR2  = 0;

    //Tlow/Thigh = 2
    I2C_CCR ^= I2C_CCR_FS_Pos;
    I2C_CCR = {I2C_CCR_CCR_Msk, 210};
    I2C_TRISE = {I2C_TRISE_TRISE_Msk, 43};
    I2C_CR1 |= I2C_CR1_PE_Pos;
}
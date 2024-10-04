#include "init.h"
#include "register.h"

void commonInit(){
	NVIC_SetPriorityGrouping(4);
}

void rccInit(){
    Register<uint32_t> RCC_CR(RCC->CR);
    Register<uint32_t> RCC_PLLCFGR(RCC->PLLCFGR);
    Register<uint32_t> RCC_CFGR(RCC->CFGR);

    //Включаем HSE
    RCC_CR |= RCC_CR_HSEON_Pos;
    while(!RCC_CR.getBitByPos(RCC_CR_HSERDY_Pos));

    //Устанавливаем divP mulN divM
    RCC_PLLCFGR = {RCC_PLLCFGR_PLLP_Msk, 0};
    RCC_PLLCFGR = {RCC_PLLCFGR_PLLN_Msk, 336};
    RCC_PLLCFGR = {RCC_PLLCFGR_PLLM_Msk, 25};

    //Источник будет HSE
    RCC_PLLCFGR |= RCC_PLLCFGR_PLLSRC_Pos;

    //Разрешаем PLL
    RCC_CR |= RCC_CR_PLLON_Pos;
    while(!RCC_CR.getBitByPos(RCC_CR_PLLRDY_Pos));

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
    ResultV<IRQn_Type> result = getIRQ(tim);
    if(enableRCC(tim) != rOK || result != rOK){
        return;
    }
    IRQn_Type irqType = getIRQ(tim).getValue();
	tim->PSC = busFreqMHz;
	tim->ARR = 1000;
	tim->DIER |= TIM_DIER_UIE;
	tim->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(irqType);
	NVIC_SetPriority(irqType, 0);
}

//enableRCC
static Result setRegMask(volatile uint32_t& reg, uint32_t mask){
    setBit(reg, mask);
    if(getBit(reg, mask)){
        return ResultBuilder::getOK();
    }
    return ResultBuilder::getError("RCC bit not set");
}

Result enableRCC(RCC_Module module){
    volatile uint32_t* reg = nullptr;
    uint32_t mask;
    switch (module) {
        case CLOCK_GPIOA:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIOAEN;
            break;
        case CLOCK_GPIOB:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIOBEN;
            break;
        case CLOCK_GPIOC:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIOCEN;
            break;
        case CLOCK_GPIOD:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIODEN;
            break;
        case CLOCK_GPIOE:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIOEEN;
            break;
        case CLOCK_GPIOF:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIOFEN;
            break;
        case CLOCK_GPIOG:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIOGEN;
            break;
        case CLOCK_GPIOH:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIOHEN;
            break;
        case CLOCK_GPIOI:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_GPIOIEN;
            break;
        case CLOCK_CRC:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_CRCEN;
            break;
        case CLOCK_BKPSRAM:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_BKPSRAMEN;
            break;
        case CLOCK_CCMDATARAM:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_CCMDATARAMEN;
            break;
        case CLOCK_DMA1:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_DMA1EN;
            break;
        case CLOCK_DMA2:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_DMA2EN;
            break;
        case CLOCK_ETHMAC:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_ETHMACEN;
            break;
        case CLOCK_ETHMACTX:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_ETHMACTXEN;
            break;
        case CLOCK_ETHMACRX:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_ETHMACRXEN;
            break;
        case CLOCK_ETHMACPTP:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_ETHMACPTPEN;
            break;
        case CLOCK_OTGHS:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_OTGHSEN;
            break;
        case CLOCK_OTGHSULPI:
            reg = &RCC->AHB1ENR;
            mask = RCC_AHB1ENR_OTGHSULPIEN;
            break;
        case CLOCK_DCMI:
            reg = &RCC->AHB2ENR;
            mask = RCC_AHB2ENR_DCMIEN;
            break;
        case CLOCK_RNG:
            reg = &RCC->AHB2ENR;
            mask = RCC_AHB2ENR_RNGEN;
            break;
        case CLOCK_OTGFS:
            reg = &RCC->AHB2ENR;
            mask = RCC_AHB2ENR_OTGFSEN;
            break;
        case CLOCK_FSMC:
            reg = &RCC->AHB3ENR;
            mask = RCC_AHB3ENR_FSMCEN;
            break;
        case CLOCK_TIM2:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM2EN;
            break;
        case CLOCK_TIM3:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM3EN;
            break;
        case CLOCK_TIM4:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM4EN;
            break;
        case CLOCK_TIM5:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM5EN;
            break;
        case CLOCK_TIM6:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM6EN;
            break;
        case CLOCK_TIM7:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM7EN;
            break;
        case CLOCK_TIM12:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM12EN;
            break;
        case CLOCK_TIM13:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM13EN;
            break;
        case CLOCK_TIM14:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_TIM14EN;
            break;
        case CLOCK_WWDG:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_WWDGEN;
            break;
        case CLOCK_SPI2:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_SPI2EN;
            break;
        case CLOCK_SPI3:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_SPI3EN;
            break;
        case CLOCK_USART2:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_USART2EN;
            break;
        case CLOCK_USART3:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_USART3EN;
            break;
        case CLOCK_UART4:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_UART4EN;
            break;
        case CLOCK_UART5:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_UART5EN;
            break;
        case CLOCK_I2C1:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_I2C1EN;
            break;
        case CLOCK_I2C2:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_I2C2EN;
            break;
        case CLOCK_I2C3:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_I2C3EN;
            break;
        case CLOCK_CAN1:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_CAN1EN;
            break;
        case CLOCK_CAN2:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_CAN2EN;
            break;
        case CLOCK_PWR:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_PWREN;
            break;
        case CLOCK_DAC:
            reg = &RCC->APB1ENR;
            mask = RCC_APB1ENR_DACEN;
            break;
        case CLOCK_TIM1:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_TIM1EN;
            break;
        case CLOCK_TIM8:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_TIM8EN;
            break;
        case CLOCK_USART1:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_USART1EN;
            break;
        case CLOCK_USART6:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_USART6EN;
            break;
        case CLOCK_ADC1:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_ADC1EN;
            break;
        case CLOCK_ADC2:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_ADC2EN;
            break;
        case CLOCK_ADC3:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_ADC3EN;
            break;
        case CLOCK_SDIO:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_SDIOEN;
            break;
        case CLOCK_SPI1:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_SPI1EN;
            break;
        case CLOCK_SYSCFG:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_SYSCFGEN;
            break;
        case CLOCK_TIM9:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_TIM9EN;
            break;
        case CLOCK_TIM10:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_TIM10EN;
            break;
        case CLOCK_TIM11:
            reg = &RCC->APB2ENR;
            mask = RCC_APB2ENR_TIM11EN;
            break;
    }
    if(reg == nullptr) return ResultBuilder::getError("Wrong module");
    return setRegMask(*reg, mask);
}

Result enableRCC(volatile GPIO_TypeDef *gpio){
    volatile uint32_t& reg = RCC->AHB1ENR;
    uint32_t mask;
    if(gpio == GPIOA){
        mask = RCC_AHB1ENR_GPIOAEN;
    } else if(gpio == GPIOB){
        mask = RCC_AHB1ENR_GPIOBEN;
    } else if(gpio == GPIOC){
        mask = RCC_AHB1ENR_GPIOCEN;
    } else if(gpio == GPIOD){
        mask = RCC_AHB1ENR_GPIODEN;
    } else if(gpio == GPIOE){
        mask = RCC_AHB1ENR_GPIOEEN;
    } else if(gpio == GPIOF){
        mask = RCC_AHB1ENR_GPIOFEN;
    } else if(gpio == GPIOG){
        mask = RCC_AHB1ENR_GPIOGEN;
    } else if(gpio == GPIOH){
        mask = RCC_AHB1ENR_GPIOHEN;
    } else if(gpio == GPIOI){
        mask = RCC_AHB1ENR_GPIOIEN;
    } else {
        return ResultBuilder::getError("Wrong GPIO_Typedef");
    }
    return setRegMask(reg, mask);
}

Result enableRCC(volatile TIM_TypeDef *tim){
    volatile uint32_t* reg;
    uint32_t mask;
    if(tim == TIM1){
        reg = &RCC->APB2ENR;
        mask = RCC_APB2ENR_TIM1EN;
    } else if(tim == TIM2){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM2EN;
    } else if(tim == TIM3){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM3EN;
    } else if(tim == TIM4){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM4EN;
    } else if(tim == TIM5){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM5EN;
    } else if(tim == TIM6){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM6EN;
    } else if(tim == TIM7){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM7EN;
    } else if(tim == TIM8){
        reg = &RCC->APB2ENR;
        mask = RCC_APB2ENR_TIM8EN;
    } else if(tim == TIM9){
        reg = &RCC->APB2ENR;
        mask = RCC_APB2ENR_TIM9EN;
    } else if(tim == TIM10){
        reg = &RCC->APB2ENR;
        mask = RCC_APB2ENR_TIM10EN;
    } else if(tim == TIM11){
        reg = &RCC->APB2ENR;
        mask = RCC_APB2ENR_TIM11EN;
    } else if(tim == TIM12){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM12EN;
    } else if(tim == TIM13){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM13EN;
    } else if(tim == TIM14){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_TIM14EN;
    } else {
        return ResultBuilder::getError("Wrong TIM_Typedef");
    }
    return setRegMask(*reg, mask);
}

Result enableRCC(volatile USART_TypeDef *usart){
    volatile uint32_t* reg;
    uint32_t mask;
    if(usart == USART1){
        reg = &RCC->APB2ENR;
        mask = RCC_APB2ENR_USART1EN;
    } else if(usart == USART2){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_USART2EN;
    } else if(usart == USART3){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_USART3EN;
    } else if(usart == UART4){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_UART4EN;
    } else if(usart == UART5){
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_UART5EN;
    } else if(usart == USART6){
        reg = &RCC->APB2ENR;
        mask = RCC_APB2ENR_USART6EN;
    } else {
        return ResultBuilder::getError("Wrong USART_Typedef");
    }
    return setRegMask(*reg, mask);
}

Result enableRCC(volatile SPI_TypeDef *spi){
    volatile uint32_t* reg;
    uint32_t mask;
    if(spi == SPI1){
        reg = &RCC->APB2ENR;
        mask = RCC_APB2ENR_SPI1EN;
    } else if(spi == SPI2) {
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_SPI2EN;
    } else if(spi == SPI3) {
        reg = &RCC->APB1ENR;
        mask = RCC_APB1ENR_SPI3EN;
    } else {
        return ResultBuilder::getError("Wrong SPI_Typedef");
    }
    return setRegMask(*reg, mask);
}

Result enableRCC(volatile I2C_TypeDef *i2c){
    volatile uint32_t& reg = RCC->APB1ENR;
    uint32_t mask;
    if(i2c == I2C1){
        mask = RCC_APB1ENR_I2C1EN;
    } else if(i2c == I2C2){
        mask = RCC_APB1ENR_I2C2EN;
    } else if(i2c == I2C3){
        mask = RCC_APB1ENR_I2C3EN;
    } else {
        return ResultBuilder::getError("Wrong I2C_Typedef");
    }
    return setRegMask(reg, mask);
}

//getIRQ
ResultV<IRQn_Type> getIRQ(volatile TIM_TypeDef *tim){
    IRQn_Type irqType = TIM1_CC_IRQn;
    if(tim == TIM1){
        irqType = TIM1_CC_IRQn;
    } else if(tim == TIM2){
        irqType = TIM2_IRQn;
    } else if(tim == TIM3){
        irqType = TIM3_IRQn;
    } else if(tim == TIM4){
        irqType = TIM4_IRQn;
    } else if(tim == TIM5){
        irqType = TIM5_IRQn;
    } else if(tim == TIM6){
        irqType = TIM6_DAC_IRQn;
    } else if(tim == TIM7){
        irqType = TIM7_IRQn;
    } else if(tim == TIM8){
        irqType = TIM8_CC_IRQn;
    } else if(tim == TIM9){
        irqType = TIM1_BRK_TIM9_IRQn;
    } else if(tim == TIM10){
        irqType = TIM1_UP_TIM10_IRQn;
    } else if(tim == TIM11){
        irqType = TIM1_TRG_COM_TIM11_IRQn;
    } else if(tim == TIM12){
        irqType = TIM8_BRK_TIM12_IRQn;
    } else if(tim == TIM13){
        irqType = TIM8_UP_TIM13_IRQn;
    } else if(tim == TIM14){
        irqType = TIM8_TRG_COM_TIM14_IRQn;
    } else {
        return ResultBuilder::getErrorV<IRQn_Type>(irqType);
    }
    return ResultBuilder::getOKv<IRQn_Type>(irqType);
}

ResultV<IRQn_Type> getIRQ(volatile USART_TypeDef *usart){
    IRQn_Type irqType = USART1_IRQn;
    if(usart == USART1){
        irqType = USART1_IRQn;
    } else if(usart == USART2){
        irqType = USART2_IRQn;
    } else if(usart == USART3){
        irqType = USART3_IRQn;
    } else if(usart == UART4){
        irqType = UART4_IRQn;
    } else if(usart == UART5){
        irqType = UART5_IRQn;
    } else if(usart == USART6){
        irqType = USART6_IRQn;
    } else {
        return ResultBuilder::getErrorV<IRQn_Type>(irqType);
    }
    return ResultBuilder::getOKv<IRQn_Type>(irqType);
}
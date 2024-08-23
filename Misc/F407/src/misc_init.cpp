#include "misc_init.h"
#include "coil.h"
#include "register.h"

extern Coil spi2Nss;

void commonInit(){
	NVIC_SetPriorityGrouping(4);
}

void rccInit(){
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));

	setRegister(&RCC->PLLCFGR,
				RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLM,
				0 | 336 << RCC_PLLCFGR_PLLN_Pos | 25);

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;

	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;

	setRegister(&RCC->CFGR,
				RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2,
				0 | 0b101 << RCC_CFGR_PPRE1_Pos | 0b100 << RCC_CFGR_PPRE2_Pos);

	RCC->CFGR |= 0b10;
	while(!(RCC->CFGR & RCC_CFGR_SWS_1));
	RCC->CFGR &= ~RCC_CR_HSION;
}

void tickInit(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 84;
	TIM2->ARR = 1000;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 6);
}

void spiInit() {
    Register<__IO uint32_t> RCC_AHB1ENR(RCC->AHB1ENR);
    Register<__IO uint32_t> RCC_APB1ENR(RCC->APB1ENR);
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

    RCC_AHB1ENR.setBit(RCC_AHB1ENR_GPIOBEN_Pos);
    RCC_APB1ENR.setBit(RCC_APB1ENR_I2C1EN_Pos);

    GPIOB_MODER.setValue(GPIO_MODER_MODE8_Msk, 0b10);
    GPIOB_MODER.setValue(GPIO_MODER_MODE9_Msk, 0b10);

    GPIOB_OTYPER.setBit(GPIO_OTYPER_OT8_Pos);
    GPIOB_OTYPER.setBit(GPIO_OTYPER_OT9_Pos);

    GPIOB_OSPEEDR.setValue(GPIO_OSPEEDR_OSPEED8_Msk, 0b11);
    GPIOB_OSPEEDR.setValue(GPIO_OSPEEDR_OSPEED9_Msk, 0b11);

    GPIOB_AFRH.setValue(GPIO_AFRH_AFSEL8_Msk, 0b100);
    GPIOB_AFRH.setValue(GPIO_AFRH_AFSEL9_Msk, 0b100);

    I2C_CR1.setBit(I2C_CR1_SWRST_Pos);
    while(!I2C_CR1.getBit(I2C_CR1_SWRST_Pos));
    I2C_CR1.resetBit(I2C_CR1_SWRST_Pos);
    while(I2C_CR1.getBit(I2C_CR1_SWRST_Pos));
    I2C_CR1 = 0;
    I2C_CR2 = 0;
    I2C_CR2.setValue(I2C_CR2_FREQ_Msk, 42);
    I2C1->OAR1 = 0;
    I2C1->OAR2  = 0;

    //Tlow/Thigh = 2
    I2C_CCR.resetBit(I2C_CCR_FS_Pos);
    I2C_CCR.setValue(I2C_CCR_CCR_Msk, 210);
    I2C_TRISE.setValue(I2C_TRISE_TRISE_Msk, 43);
    I2C_CR1.setBit(I2C_CR1_PE_Pos);
}
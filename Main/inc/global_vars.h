#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include "coil_with_timer.h"
#include "scale.h"
#include "simple_input_delayed.h"
#include "pid.h"
#include "main.h"
#include "simple_uart.h"
#include "simple_i2c.h"
#include "simple_spi.h"
#include "simple_sdio.h"

CoilOffDelay led1({GPIOE, 13}, 1000);
Coil led2({GPIOE, 14});
SimpleInputDelayed button({GPIOE, 10}, 10);
OnDelay delay(1000);
PIDreg pid(20, 65.2);
SimpleUART uart(USART1,
                {GPIOA, 10},
                {GPIOA, 9},
                84,
                _9600,
                NO_PARITY,
                _1,
                128,
                10);
SimpleI2C i2c(I2C1,
              {GPIOB, 9},
              {GPIOB, 8},
              42,
              FM,
              _2_1,
              128,
              10);
Coil spiNSS({GPIOE, 3});
SimpleSPI spi(SPI2,
              {GPIOC, 3},
              {GPIOC, 2},
              {GPIOB, 10},
              2,
              42,
              POLPHA_0,
              POLPHA_0,
              _8BIT,
              MSB,
              spiNSS,
              128,
              10);

SimpleSDIO sdio(SDIO_1BIT,
                4,
                84000000,
                100);

IUpdated1ms *updateObjects[] = {
        &button,
        &led1,
        &delay,
        &uart,
        &i2c,
        &spi,
        &sdio
};

uint8_t updateObjectsSize = sizeof(updateObjects) / sizeof(*updateObjects);

#endif //GLOBAL_VARS_H
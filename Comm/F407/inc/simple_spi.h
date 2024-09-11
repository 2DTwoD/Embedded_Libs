#ifndef SIMPLE_SPI_H
#define SIMPLE_SPI_H
#include <cstdint>
#include "stm32f407xx.h"

#include "register.h"
#include "gpio_common.h"
#include "on_delay.h"
#include "buffer.h"
#include "coil.h"

enum POLPHA{
    POLPHA_0 = 0, POLPHA_1 = 1
};

enum SPI_data{
    _8BIT = 0, _16BIT = 1
};

enum SPI_frame{
    MSB = 0, LSB = 1
};

class SimpleSPI: private OnDelayCommon, public IUpdated1ms, public InternalBuffer{
private:
    SPI_TypeDef* spi;
    Coil& nss;
    SPI_data dataFormat;
    void adjustGPIO(GPIO_Info gpioInfo, uint8_t AFcode);
    bool write(uint8_t *const data, uint16_t len);
    bool write(uint16_t *const data, uint16_t len);
    bool read(uint16_t len);
public:
    explicit SimpleSPI(SPI_TypeDef *sp, GPIO_Info mosi, GPIO_Info miso, GPIO_Info clk,
                       uint16_t SPIfreqMHz, uint16_t busFreqMHz, POLPHA pol, POLPHA pha,
                       SPI_data dataFormat, SPI_frame frameFormat, Coil& nss, uint16_t bufferSize, uint32_t errorDelay);
    void update1ms() override;
    void newNss(Coil& newNss);
    bool send(uint8_t *const src, uint16_t srcLen);
    bool send(uint8_t value);
    bool send(uint16_t *const src, uint16_t srcLen);
    bool send(uint16_t value);
    bool receive(uint16_t readLen);
    bool receive();
    bool sendAndReceive(uint8_t *const src, uint16_t srcLen, uint16_t readLen);
    bool sendAndReceive(uint16_t *const src, uint16_t srcLen, uint16_t readLen);
    bool sendAndReceive(uint8_t value, uint16_t readLen);
    bool sendAndReceive(uint16_t value, uint16_t readLen);
};

#endif //SIMPLE_SPI_H

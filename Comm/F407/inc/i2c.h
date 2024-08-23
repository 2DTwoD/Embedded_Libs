#ifndef I2C_H
#define I2C_H
// you need initiaize i2c out of this class

#include "stm32f407xx.h"

class I2Cworker{
private:
    I2C_TypeDef i2c;
public:
    explicit I2Cworker(const I2C_TypeDef &i2C);
};

#endif //I2C_H

#ifndef PROGRAMM_COIL_H
#define PROGRAMM_COIL_H

#include "interfaces.h"

class ProgrammCoil: public ISwitch{
private:
    bool out;
public:
    virtual bool isActive() override;
    virtual void setValue(bool value);
    bool isNotActive();
    void set();
    void reset();
    void toggle();
    ProgrammCoil& operator=(bool value);
};

#endif //PROGRAMM_COIL_H

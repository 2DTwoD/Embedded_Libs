#include "programm_coil.h"

bool ProgrammCoil::isActive(){
    return out;
}
void ProgrammCoil::setValue(bool value){
    out = value;
}
bool ProgrammCoil::isNotActive(){
    return !isActive();
}
void ProgrammCoil::set(){
    setValue(true);
}
void ProgrammCoil::reset(){
    setValue(false);
}
void ProgrammCoil::toggle(){
    setValue(!isActive());
}
ProgrammCoil& ProgrammCoil::operator=(bool value){
    setValue(value);
    return *this;
}
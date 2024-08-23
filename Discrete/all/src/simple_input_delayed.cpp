#include "simple_input_delayed.h"

SimpleInputDelayed::SimpleInputDelayed(GPIO_Info gpioInfo, uint16_t delay): SimpleInput(gpioInfo), CommonTimer(delay){
}
void SimpleInputDelayed::update1ms(){
    CommonTimer::setStart(SimpleInput::isActive());
    CommonTimer::update();
}
bool SimpleInputDelayed::isActive(){
    return finished();
}
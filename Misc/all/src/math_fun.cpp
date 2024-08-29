#include "math_fun.h"

uint32_t round(float value){
    if((uint32_t)(value * 10.0) % 10 > 5) {
        return (uint32_t) value + 1;
    }
    return (uint32_t) value;
}
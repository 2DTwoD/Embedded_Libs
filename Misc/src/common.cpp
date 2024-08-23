#include "common.h"

void badTimeOut(uint32_t value){
	for(int i = 0; i < value; i++);
}

int8_t getPosition(uint32_t mask){
	for(int i = 0; i < 32; i++){
		if(mask & (1<<i)) return i;
	}
	return -1;
}

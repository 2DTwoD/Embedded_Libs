#ifndef MAIN_H
#define MAIN_H

#include "FreeRTOS.h"
#include "task.h"

#include "init.h"
#include "global_vars.h"
#include "all_tasks.h"

extern uint8_t updateObjectsSize;

extern "C"{
    //include this function name in startup_stm32x.s after Reset_Handler, for floating point operations in global code(line 62)
    void enableFPU(){
        SCB->CPACR |= ((3UL << 10 * 2)|(3UL << 11 * 2));
    }

	void vApplicationIdleHook ( void ){
	}

	void vApplicationTickHook ( void ){
	}

	void vApplicationMallocFailedHook ( void ){
		while(1);
	}

	void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName ){
		( void ) pcTaskName;
		( void ) xTask;
		while(1);
	}

	void TIM2_IRQHandler(void){
		TIM2->SR &= ~TIM_SR_UIF;
		for(int i = 0; i < updateObjectsSize; i++){
			updateObjects[i]->update1ms();
		}
	}

    void USART1_IRQHandler(void) {
        uart.IRQhandler();
    }
    /*void ADC1_2_IRQHandler(void){
        if(ADC1->SR & ADC_SR_EOC){
            uint16_t val = ADC1->DR;
        }
    }*/
}
#endif //MAIN_H

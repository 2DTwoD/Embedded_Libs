#include "main.h"
#include "array_list.h"
int main(void)
{
	rccInit();
	tickInit(TIM2, 84);
	commonInit();
	xTaskCreate(ledTask, "ledTask", configMINIMAL_STACK_SIZE, NULL,
                tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(pidTask, "pidTask", configMINIMAL_STACK_SIZE, NULL,
                tskIDLE_PRIORITY + 1, NULL);
	BaseType_t res = xTaskCreate(testTask, "testTask", configMINIMAL_STACK_SIZE * 10, NULL,
                tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
	while(1);
}

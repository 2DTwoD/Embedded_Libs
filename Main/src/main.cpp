#include "main.h"

int main(void)
{
	rccInit();
	tickInit();
	commonInit();
	xTaskCreate(ledTask, "ledTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(pidTask, "pidTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(testTask, "testTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
	while(1);
}

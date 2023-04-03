#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "timer_interrupts.h"
#include "semphr.h"

xSemaphoreHandle xSemaphore;

void LedBlink(void)
{
		if(xSemaphoreTakeFromISR(xSemaphore, portMAX_DELAY == pdTRUE))
		{
			Led_Toggle(0);
			xSemaphoreGiveFromISR(xSemaphore, (long *)portMAX_DELAY);
		}
}

int main(void)
{
	vSemaphoreCreateBinary( xSemaphore );
	LedInit();
	Timer1Interrupts_Init(500, &LedBlink);
	vTaskStartScheduler();
	while(1);
}

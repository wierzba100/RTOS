#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "timer_interrupts.h"
void LedBlink(void)
{
	Led_Toggle(0);
}
int main(void)
{
	LedInit();
	Timer1Interrupts_Init(500, &LedBlink);
	vTaskStartScheduler();
	while(1);
}

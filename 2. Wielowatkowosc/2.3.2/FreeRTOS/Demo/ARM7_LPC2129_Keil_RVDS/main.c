#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

void Delay(unsigned int uiMiliSec) {
	unsigned int uiLoopCtr, uiDelayLoopCount;
	uiDelayLoopCount = uiMiliSec*12000;
	for(uiLoopCtr=0;uiLoopCtr<uiDelayLoopCount;uiLoopCtr++) {}
}

void Led0Blink( void *pvParameters ){
	while(1)
	{
		LedOn(0);
		Delay(1000);
		LedOff(0);
		Delay(1000);
	}
}

void Led1Blink( void *pvParameters ){
	while(1)
	{
		Led_Toggle(1);
		vTaskDelay(50);
	}
}

int main(void){
	LedInit();
	xTaskCreate(Led0Blink, NULL , 100 , NULL, 2 , NULL );
	xTaskCreate(Led1Blink, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}




#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

void LedBlink( void *pvParameters )
{ 
	unsigned char* ucFreq = (unsigned char*)pvParameters;
	while(1)
	{ 
		Led_Toggle(0);
		vTaskDelay((1000/ *ucFreq)/2);
	} 
}

void LedBlinkControl( void *pvParameters )
{
	TaskHandle_t* myHandle  = (TaskHandle_t*)pvParameters;
	while(1)
	{
		vTaskResume(*myHandle);
		vTaskDelay(500);
		vTaskSuspend(*myHandle);
		vTaskDelay(500);
	}
}



int main( void ) 
{ 
	unsigned char ucBlinkingFreq = 10;
	TaskHandle_t xMyHandle = NULL;
	
	LedInit();
	xTaskCreate(LedBlinkControl, NULL , 100 , &xMyHandle, 2 , NULL );
	xTaskCreate(LedBlink, NULL , 100 , &ucBlinkingFreq, 1 , &xMyHandle );
	vTaskStartScheduler(); 
	while(1); 
}




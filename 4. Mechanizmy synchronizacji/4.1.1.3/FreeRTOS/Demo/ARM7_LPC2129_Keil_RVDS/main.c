#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"

void PulseTrigger( void *pvParameters )
{ 
	xSemaphoreHandle* xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1)
	{ 
		xSemaphoreGive(*xSemaphore);
		vTaskDelay(500);
	} 
}

void Pulse_Led0( void *pvParameters )
{
	xSemaphoreHandle* xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1)
	{
		if(xSemaphoreTake(*xSemaphore,portMAX_DELAY) ==  pdTRUE)
		{
			LedOn(0);
			vTaskDelay(50);
			LedOff(0);
		}
	}
}

void Pulse_Led1( void *pvParameters )
{
	xSemaphoreHandle* xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1)
	{
		if(xSemaphoreTake(*xSemaphore,portMAX_DELAY) ==  pdTRUE)
		{
			LedOn(1);
			vTaskDelay(50);
			LedOff(1);
		}
	}
	
}



int main( void ) 
{
	xSemaphoreHandle xSemaphore;
	vSemaphoreCreateBinary( xSemaphore );
	
	LedInit();
	xTaskCreate(PulseTrigger, NULL , 100 , &xSemaphore, 2 , NULL );
	xTaskCreate(Pulse_Led0, NULL , 100 , &xSemaphore, 1 , NULL );
	xTaskCreate(Pulse_Led1, NULL , 100 , &xSemaphore, 1 , NULL );
	vTaskStartScheduler(); 
	while(1); 
}




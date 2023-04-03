#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"

typedef struct LedControl
{
	xSemaphoreHandle xSemaphore;
	unsigned char LedNr;
} LedControl;

void PulseTrigger( void *pvParameters )
{ 
	xSemaphoreHandle* xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1)
	{ 
		xSemaphoreGive(*xSemaphore);
		vTaskDelay(500);
	} 
}

void Pulse_Led( void *pvParameters )
{
	LedControl* LedControlptr = (LedControl*)pvParameters;
	while(1)
	{
		if(xSemaphoreTake(LedControlptr->xSemaphore,portMAX_DELAY) ==  pdTRUE)
		{
			LedOn(LedControlptr->LedNr);
			vTaskDelay(50);
			LedOff(LedControlptr->LedNr);
		}
	}
}

xSemaphoreHandle xSemaphore;
LedControl LedControl_1;
LedControl LedControl_2;

int main( void ) 
{
	vSemaphoreCreateBinary( xSemaphore );
	
	LedControl_1.xSemaphore = xSemaphore;
	LedControl_2.xSemaphore = xSemaphore;
	
	LedControl_1.LedNr = 0;
	LedControl_2.LedNr = 1;
	
	LedInit();
	xTaskCreate(PulseTrigger, NULL , 100 , &xSemaphore, 2 , NULL );
	xTaskCreate(Pulse_Led, NULL , 100 , &LedControl_1, 1 , NULL );
	xTaskCreate(Pulse_Led, NULL , 100 , &LedControl_2, 1 , NULL );
	vTaskStartScheduler(); 
	while(1); 
}




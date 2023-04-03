#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

typedef struct LedControl
{
	unsigned char ucFreq;
	unsigned char ucLedIndeks;
}LedControl;

void LedBlink( void *pvParameters )
{ 
	LedControl* LedControlptr = (LedControl*)pvParameters;
	while(1)
	{ 
		Led_Toggle(LedControlptr->ucLedIndeks % 4);
		vTaskDelay((1000/ LedControlptr->ucFreq)/2);
	} 
}

void LedCtrl( void *pvParameters )
{
	while(1)
	{
		LedControl* LedControlptr = (LedControl*)pvParameters;
		LedControlptr->ucFreq = LedControlptr->ucFreq + 1;
		vTaskDelay(500);
		LedControlptr->ucFreq = LedControlptr->ucFreq + 1;
		vTaskDelay(500);
		LedControlptr->ucLedIndeks = LedControlptr->ucLedIndeks + 1;
		LedControlptr->ucFreq = LedControlptr->ucFreq + 1;
		vTaskDelay(500);
	}
}



int main( void ) 
{ 
	LedControl LedControl;
	LedControl.ucFreq = 1;
	LedControl.ucLedIndeks = 0;
	LedInit(); 
	xTaskCreate(LedBlink, NULL , 100 , &LedControl, 1 , NULL );
	xTaskCreate(LedCtrl, NULL , 100 , &LedControl, 2 , NULL );
	vTaskStartScheduler(); 
	while(1); 
}




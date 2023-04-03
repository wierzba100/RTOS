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

void LedCtrl( void *pvParameters )
{ 
	unsigned char* ucFreq = (unsigned char*)pvParameters;
	while(1)
	{ 
		*ucFreq = *ucFreq + 1;
		vTaskDelay(500);
	} 
}


int main( void ) 
{ 
	unsigned char ucBlinkingFreq = 1;
	LedInit(); 
	xTaskCreate(LedBlink, NULL , 100 , &ucBlinkingFreq, 1 , NULL );
	xTaskCreate(LedCtrl, NULL , 100 , &ucBlinkingFreq, 2 , NULL );
	vTaskStartScheduler(); 
	while(1); 
}




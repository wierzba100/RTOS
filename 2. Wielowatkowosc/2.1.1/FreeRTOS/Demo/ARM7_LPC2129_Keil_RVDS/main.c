#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

void Delay(unsigned int uiMiliSec) {
	unsigned int uiLoopCtr, uiDelayLoopCount;
	uiDelayLoopCount = uiMiliSec*12000;
	for(uiLoopCtr=0;uiLoopCtr<uiDelayLoopCount;uiLoopCtr++) {}
}

int main( void )
{
	LedInit();
	LedOff(0);
	while(1)
	{
		Led_Toggle(0);
		Led_Toggle(1);
		Delay(125);
		Led_Toggle(1);
		Delay(125);
		Led_Toggle(1);
		Delay(125);
		Led_Toggle(1);
		Delay(125);
	} 
}




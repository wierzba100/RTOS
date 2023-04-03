#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "keyboard.h"
#include "watch.h"

/*void DetectButton( void *pvParameters )
{
	while(1)
	{
		if(eKeyboard_Read() == BUTTON_0)
		{
			Led_Toggle(0);
		}else if(eKeyboard_Read() == BUTTON_1)
		{
			Led_Toggle(1);
		}else if(eKeyboard_Read() == BUTTON_2)
		{
			Led_Toggle(2);
		}else if(eKeyboard_Read() == BUTTON_3)
		{
			Led_Toggle(3);
		}else
		{
			
		}
		vTaskDelay(250);
	}
}*/



void DetectTime( void *pvParameters )
{
	while(1)
	{
		if(ElapsedSeconds()%2 == 0)
		{
			LedOn(0);
		}else
		{
			LedOff(0);
		}
		if(ElapsedMinutes()%2 == 0)
		{
			LedOn(1);
		}else
		{
			LedOff(1);
		}
	}
}


int main( void )
{
	LedInit();
	WatchInit();
	//KeyboardInit();
	//UART_InitWithInt(9600);
	xTaskCreate( DetectTime, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}

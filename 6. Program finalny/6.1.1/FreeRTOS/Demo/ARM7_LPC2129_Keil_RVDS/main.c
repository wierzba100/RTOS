#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "keyboard.h"

void DetectButton( void *pvParameters )
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
}


int main( void )
{
	LedInit();
	KeyboardInit();
	//UART_InitWithInt(9600);
	xTaskCreate( DetectButton, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}

#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"

void KeyboardTx( void *pvParameters )
{
	while(1)
	{
		if(ReadKeyboardState() != RELASED)
		{
			Transmiter_SendString("-Keyboard-\n");
			while (eReciever_GetStatus()!=FREE){};
		}
		vTaskDelay(300);
	}
	
}


void LettersTx (void *pvParameters)
{
	while(1)
	{
		Transmiter_SendString("-ABCDEEFGH-\n");
		while (eReciever_GetStatus()!=FREE){};
		vTaskDelay(300);
	}
}

int main( void )
{
	KeyboardInit();
	UART_InitWithInt(300);
	xTaskCreate(LettersTx, NULL, 128, NULL, 2, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}

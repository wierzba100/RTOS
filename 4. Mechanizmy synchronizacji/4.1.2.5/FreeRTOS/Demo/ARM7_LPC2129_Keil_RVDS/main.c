#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"

void Rtos_Transmiter_SendString(char String[])
{
	Transmiter_SendString(String);
	while(eReciever_GetStatus()!=FREE){};
	while(Transmiter_GetStatus()!=FREE){};
}

void KeyboardTx( void *pvParameters )
{
	xSemaphoreHandle* xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1)
	{
		if(ReadKeyboardState() != RELASED)
		{
			if(xSemaphoreTake(*xSemaphore,portMAX_DELAY) ==  pdTRUE)
			{
				Rtos_Transmiter_SendString("-Keyboard-\n");
				xSemaphoreGive(*xSemaphore);
			}
			vTaskDelay(300);
		}
	}
}


void LettersTx (void *pvParameters)
{
	xSemaphoreHandle* xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1)
	{
		if(xSemaphoreTake(*xSemaphore,portMAX_DELAY) ==  pdTRUE)
		{
			Rtos_Transmiter_SendString("-ABCDEEFGH-\n");
			xSemaphoreGive(*xSemaphore);
		}
		vTaskDelay(300);
	}
}

xSemaphoreHandle xSemaphore;

int main( void )
{
	vSemaphoreCreateBinary( xSemaphore );
	KeyboardInit();
	UART_InitWithInt(300);
	xTaskCreate(LettersTx, NULL, 128, &xSemaphore, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, &xSemaphore, 1, NULL );
	vTaskStartScheduler();
	while(1);
}

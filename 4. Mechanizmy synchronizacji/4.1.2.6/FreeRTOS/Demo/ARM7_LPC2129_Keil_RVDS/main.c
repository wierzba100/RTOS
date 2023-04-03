#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"

xSemaphoreHandle xSemaphore;

void Rtos_Transmiter_SendString(xSemaphoreHandle xSemaphore, char cString[])
{
	xSemaphoreTake(xSemaphore, portMAX_DELAY );
	Transmiter_SendString(cString);
	while(Transmiter_GetStatus()!=FREE){};
	xSemaphoreGive(xSemaphore);
}

void KeyboardTx( void *pvParameters )
{
	while(1)
	{
		if(ReadKeyboardState() != RELASED)
		{
			Rtos_Transmiter_SendString(xSemaphore, "-Keyboard-\n");
		}
			vTaskDelay(300);
	}
}


void LettersTx (void *pvParameters)
{
	unsigned int uiStart, uiStop, uiTickCount;
	while(1)
	{
		char cString[20] = "-ABCDEEFGH-:\n";
		uiStart = xTaskGetTickCount();
		Rtos_Transmiter_SendString(xSemaphore, cString);
		cString[0] = '\0';
		uiStop = xTaskGetTickCount();
		uiTickCount = uiStop - uiStart;
		AppendUIntToString(uiTickCount, cString);
		AppendString("\n", cString);
		Rtos_Transmiter_SendString(xSemaphore, cString);
		
		vTaskDelay(300);
	}
}


int main( void )
{
	vSemaphoreCreateBinary( xSemaphore );
	KeyboardInit();
	UART_InitWithInt(300);
	
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}

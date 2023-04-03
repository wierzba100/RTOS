#include <lpc21xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"
#include "queue.h"

QueueHandle_t xQueue;

void Rtos_Transmiter_SendString(void *pvParameters)
{
	char *ptrStringToSend;
	while(1)
	{
		if( xQueueReceive( xQueue,&( ptrStringToSend ),( TickType_t ) 10 ) == pdPASS )
		{
			Transmiter_SendString(ptrStringToSend);
			while(eReciever_GetStatus()!=FREE){};
			while(Transmiter_GetStatus()!=FREE){};
		}
		vTaskDelay(300);
	}
}

void KeyboardTx( void *pvParameters )
{
	char StringToSend[] = "-Keyboard-\n";
	char *ptrStringToSend = StringToSend;
	while(1)
	{
		if(ReadKeyboardState() != RELASED)
		{
			if(xQueueSend( xQueue, &ptrStringToSend, portMAX_DELAY ) != pdPASS)
			{
				Led_Toggle(0);
			}
			vTaskDelay(300);
		}
	}
}


void LettersTx (void *pvParameters)
{
	unsigned int uiStart, uiStop, uiTickCount;
	while(1)
	{
		char StringToSend[20] = "-ABCDEEFGH-:\n";
		char *ptrStringToSend = StringToSend;
		uiStart = xTaskGetTickCount();
		if(xQueueSend( xQueue, &ptrStringToSend, portMAX_DELAY) != pdPASS )
		{
			Led_Toggle(0);
		}
		uiStop = xTaskGetTickCount();
		uiTickCount = uiStop - uiStart;
		StringToSend[0] = '\0';
		AppendUIntToString(uiTickCount, StringToSend);
		AppendString("\n", StringToSend);
		if(xQueueSend( xQueue, &ptrStringToSend, portMAX_DELAY ) != pdPASS)
		{
			Led_Toggle(0);
		}
		vTaskDelay(300);
	}
}


int main( void )
{
	xQueue = xQueueCreate( 6, sizeof( char[20] ) );
	KeyboardInit();
	UART_InitWithInt(300);
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(Rtos_Transmiter_SendString, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}

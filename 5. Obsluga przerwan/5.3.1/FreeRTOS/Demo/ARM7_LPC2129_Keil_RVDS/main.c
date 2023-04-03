#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"

void UartRx( void *pvParameters )
{
	char acBuffer[RECEIVER_SIZE];
	acBuffer[0] = '\0';
	while(1)
	{
		Uart_GetString(acBuffer);
		Uart_PutString(acBuffer);
		vTaskDelay(100);
	}
}


int main( void )
{
	LedInit();
	UART_InitWithInt(9600);
	xTaskCreate( UartRx, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}

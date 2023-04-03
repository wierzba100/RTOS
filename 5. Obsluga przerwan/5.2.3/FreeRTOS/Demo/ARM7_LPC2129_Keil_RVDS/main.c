#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"

void UartRx( void *pvParameters )
{
	char acBuffer[RECEIVER_SIZE];
	acBuffer[0] = '\0';
	while(1)
	{
		cUart_GetString(acBuffer);
		Transmiter_SendString(acBuffer);
		acBuffer[0] = acBuffer[0] - '0';
		Led_Toggle(acBuffer[0]);
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

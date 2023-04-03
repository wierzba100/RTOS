#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"

void UartRx( void *pvParameters )
{
	//char acBuffer[RECEIVER_SIZE];
	char odebranyznak;
	while(1)
	{
		odebranyznak = cUart_GetChar();
		odebranyznak = odebranyznak - '0';
		Led_Toggle(odebranyznak);
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

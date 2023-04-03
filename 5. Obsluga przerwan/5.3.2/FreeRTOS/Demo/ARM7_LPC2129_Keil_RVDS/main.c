#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"

void UartRx( void *pvParameters )
{
	char acBuffer[RECEIVER_SIZE] = "0123456789\n";
	while(1)
	{
		//A
		/*vTaskDelay(500);
		Uart_PutString(acBuffer);
		Uart_PutString(acBuffer);
		Uart_PutString(acBuffer);*/
		//B
		vTaskDelay(500);
		Uart_PutString(acBuffer);
		vTaskDelay(10);
		Uart_PutString(acBuffer);
		vTaskDelay(10);
		Uart_PutString(acBuffer);

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

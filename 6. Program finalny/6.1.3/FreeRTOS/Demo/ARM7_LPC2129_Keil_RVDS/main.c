#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "keyboard.h"
#include "watch.h"

void UART_PutButtonSecMin(char Buffer1[])
{
	char seconds;
	char minutes;
	char Buffer2[RECEIVER_SIZE]; 
	seconds = ElapsedSeconds();
	UIntToHexStr(seconds, Buffer2);
	AppendString("sek, ", Buffer2);
	AppendString(Buffer2, Buffer1);
	minutes = ElapsedMinutes();
	UIntToHexStr(minutes, Buffer2);
	AppendString("min ", Buffer2);
	AppendString(Buffer2, Buffer1);
	Uart_PutString(Buffer1);
}


void DetectButton( void *pvParameters )
{
	while(1)
	{
		if(eKeyboard_Read() == BUTTON_0)
		{
			char Buffer1[TRANSMITER_SIZE] = "0x0000 button, ";
			UART_PutButtonSecMin(Buffer1);
		}else if(eKeyboard_Read() == BUTTON_1)
		{
			char Buffer1[TRANSMITER_SIZE] = "0x0001 button, ";
			UART_PutButtonSecMin(Buffer1);
		}else if(eKeyboard_Read() == BUTTON_2)
		{
			char Buffer1[TRANSMITER_SIZE] = "0x0002 button, ";
			UART_PutButtonSecMin(Buffer1);
		}else if(eKeyboard_Read() == BUTTON_3)
		{
			char Buffer1[TRANSMITER_SIZE] = "0x0003 button, ";
			UART_PutButtonSecMin(Buffer1);
		}else
		{
			
		}
		vTaskDelay(250);
	}
}



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
	KeyboardInit();
	UART_InitWithInt(9600);
	xTaskCreate( DetectButton, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}

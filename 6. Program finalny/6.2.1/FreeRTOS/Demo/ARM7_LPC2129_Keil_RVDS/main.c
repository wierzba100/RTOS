#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "keyboard.h"
#include "servo.h"
#include "queue.h"

xQueueHandle xQueueEvents;

void DetectButton( void *pvParameters )
{
	while(1)
	{
		if(eKeyboard_Read() == BUTTON_0)
		{
			xQueueSend(xQueueEvents, "button 0x0000", portMAX_DELAY);
			//ServoCallib();
		}else if(eKeyboard_Read() == BUTTON_1)
		{
			xQueueSend(xQueueEvents, "button 0x0001", portMAX_DELAY);
			//ServoGoTo(12);
		}else if(eKeyboard_Read() == BUTTON_2)
		{
			xQueueSend(xQueueEvents, "button 0x0002", portMAX_DELAY);
			//ServoGoTo(24);
		}else if(eKeyboard_Read() == BUTTON_3)
		{
			xQueueSend(xQueueEvents, "button 0x0003", portMAX_DELAY);
			//ServoGoTo(36);
		}else
		{
			
		}
		vTaskDelay(100);
	}
}

void DetectUART(void *pvParameters)
{
	char acBuffer[20];
	acBuffer[0] = '\0';
	while(1)
	{
		Uart_GetString(acBuffer);
		xQueueSend(xQueueEvents, acBuffer, portMAX_DELAY);
		vTaskDelay(100);
	}
	
}

void Executor(void *pvParameters)
{
	char buffer[20];
	while(1)
	{
		if(xQueueReceive( xQueueEvents, &buffer, 10) == pdPASS)
		{
			if(eCompareString(buffer,"button 0x0000") == EQUAL)
			{
				ServoCallib();
			}else if(eCompareString(buffer,"button 0x0001") == EQUAL)
			{
				ServoGoTo(12);
			}else if(eCompareString(buffer,"button 0x0002") == EQUAL)
			{
				ServoGoTo(24);
			}else if(eCompareString(buffer,"button 0x0003") == EQUAL)
			{
				ServoGoTo(36);
			}else if(eCompareString(buffer,"callib") == EQUAL)
			{
				ServoCallib();
				Uart_PutString("ok");
			}else if((buffer[0] == 'g') && (buffer[1] == 'o') && (buffer[2] == 't') && (buffer[3] == 'o'))
			{
				char hexbuffer[7];
				unsigned int puiValue;
				int i;
				for(i = 0; i < 7;i++)
				{
					hexbuffer[i] = buffer[4+i];
				}
				
				if(eHexStringToUInt(hexbuffer, &puiValue) == OK)
				{
					ServoGoTo(puiValue);
					Uart_PutString("ok");
				}
			}else
			{
				
			}
		}else
		{
			
		}
			
	}
	
}



int main( void )
{
	unsigned int uiServoFrequency = 200;
	LedInit();
	KeyboardInit();
	ServoInit(&uiServoFrequency);
	UART_InitWithInt(9600);
	xQueueEvents = xQueueCreate(6, sizeof (char[20]));
	xTaskCreate( DetectButton, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( DetectUART, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( Executor, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}

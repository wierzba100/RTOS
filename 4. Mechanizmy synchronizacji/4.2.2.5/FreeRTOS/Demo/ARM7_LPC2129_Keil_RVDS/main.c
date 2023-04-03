#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "keyboard.h"
#include "queue.h"
#include "servo.h"


void Keyboard (void *pvParameters)
{
	enum KeyboardState eButton;
	while(1)
	{	
		eButton = eKeyboardDetector();
	
		switch(eButton)
		{
			case BUTTON_0:
				ServoCallib();
				break;
			case BUTTON_1:
				ServoGoTo(50);
				break;
			case BUTTON_2:
				ServoGoTo(100);
				break;	
			case BUTTON_3:
				ServoGoTo(12);
				ServoGoTo(0);
				ServoGoTo(24);
				ServoGoTo(0);
				ServoGoTo(36);
				ServoGoTo(0);
				break;
			default:
				break;
		}
		vTaskDelay(100);
	}
}


int main( void)
{
	unsigned int uiServoFrequency = 200;
	KeyboardInit();
	ServoInit(&uiServoFrequency);
	
	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL );
	
	vTaskStartScheduler();
	while(1);
}

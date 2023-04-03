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
				ServoGoTo(150);				
				break;
			default:
				break;
		}
		vTaskDelay(100);
	}
}


int main( void)
{
	DetectorInit();
	KeyboardInit();
	LedInit();
	ServoInit(100);
	
	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL );
	
	vTaskStartScheduler();
	while(1);
}

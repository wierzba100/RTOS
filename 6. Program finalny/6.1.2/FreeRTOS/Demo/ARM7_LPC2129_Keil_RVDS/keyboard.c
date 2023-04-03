#include <LPC21xx.H>
#include "keyboard.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#define BUTTON0_bm (1<<4)
#define BUTTON1_bm (1<<6)
#define BUTTON2_bm (1<<5)
#define BUTTON3_bm (1<<7)

enum KeyboardState;
unsigned char ucPressedFlag;
enum KeyboardState eButton;

xQueueHandle xKeyboardQueue;


enum KeyboardState eReadButtons(void){
	

	if ((IO0PIN & BUTTON0_bm) == 0){
		return BUTTON_0;
	}
	else if((IO0PIN & BUTTON1_bm) == 0){
		return BUTTON_1;
	}
	else if ((IO0PIN & BUTTON2_bm) == 0 ){
		return BUTTON_2;
	}
	else if ((IO0PIN & BUTTON3_bm) == 0 ){
		return BUTTON_3;
	}
	else{
		return RELEASED;
	}
}

enum KeyboardState eKeyboard_Read(void)
{
	enum KeyboardState PressedButton;
	if(xQueueReceive( xKeyboardQueue, &PressedButton, 10) == pdPASS)
	{
		return PressedButton;
	}else
	{
		return RELEASED;
	}
	
}

enum KeyboardState eKeyboardDetector(void){

	eButton = eReadButtons();
	
	if ((eButton!=RELEASED)&&(ucPressedFlag == 0)){
		
		ucPressedFlag = 1;
		return eReadButtons();
	}else if (eButton==RELEASED) {
			ucPressedFlag =0;
			return RELEASED;
	}else{
		return RELEASED;
	}
		
}

void KeyboardThread( void *pvParameters )
{
	enum KeyboardState PressedButton;
	while(1)
	{
		PressedButton = eReadButtons();
		if(PressedButton != RELEASED)
		{
			xQueueSend(xKeyboardQueue, &PressedButton, portMAX_DELAY);
		}
	}
}

void KeyboardInit(void)
{
	xKeyboardQueue = xQueueCreate(1, sizeof(eButton));
	IO0DIR = IO0DIR &(~(BUTTON0_bm | BUTTON1_bm | BUTTON2_bm | BUTTON3_bm));
	xTaskCreate( KeyboardThread, NULL , 100 , NULL, 1 , NULL );
}



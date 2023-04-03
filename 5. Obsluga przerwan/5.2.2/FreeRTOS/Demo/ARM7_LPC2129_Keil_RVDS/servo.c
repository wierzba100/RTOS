#include "Servo.h"
#include <LPC21xx.H>
#include "task.h"
#include "timer.h"
#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"
#include "queue.h"

#define DETECTOR_bm	(1<<10)


struct Servo sServo;

xQueueHandle xQueue;

enum DetectorState eReadDetector(){
	if((IO0PIN & DETECTOR_bm) == 0){
		return ACTIVE;
	}
	else{
		return INACTIVE;
	}
}

void DetectorInit(){
	IO0DIR = (IO0DIR & (~DETECTOR_bm));

}


void Automat(void *pvParameters)
{
	unsigned int* uiServoFregPtr = (unsigned int*)pvParameters;
	unsigned int uiDesiredPosition = 0;
	sServo.uiServoSpeed = 1000/(*uiServoFregPtr);
	while(1){
		
		switch(sServo.eState){
				case CALLIB:
					if(eReadDetector() == ACTIVE){
						sServo.uiCurrentPosition = 0;
						uiDesiredPosition = 0;
						sServo.eState = IDLE;
						}
					else{
						LedStepRight();}
						break;	
				case IDLE:	
					if(xQueueReceive( xQueue, &uiDesiredPosition, 10) == pdPASS)
					{
						if(sServo.uiCurrentPosition == uiDesiredPosition){
							sServo.eState = IDLE;}
						else{
							sServo.eState = IN_PROGRESS;}
					}
					break;
				case IN_PROGRESS:
					if(sServo.uiCurrentPosition < uiDesiredPosition){	
						LedStepRight();
						sServo.uiCurrentPosition++;}					
					else if(sServo.uiCurrentPosition > uiDesiredPosition){
						LedStepLeft();
						sServo.uiCurrentPosition--;}					
					else{
						sServo.eState = IDLE;}
						break;
				
			}
			
			vTaskDelay(sServo.uiServoSpeed);
	}
}

void ServoCallib(){	
	sServo.eState = CALLIB;

}

void ServoInit(unsigned int * uiServoFrequency){
	LedInit();
	DetectorInit();
	xQueue = xQueueCreate(6, sizeof (int));
	xTaskCreate(Automat, NULL, 128, uiServoFrequency , 1, NULL );
}

void ServoGoTo(unsigned int uiPosition){
	xQueueSend(xQueue, &uiPosition, portMAX_DELAY);
}

void ServoWait(TickType_t tDelay)
{	
	vTaskDelay(tDelay);
}

void ServoSpeed(unsigned int uiSpeed)
{
	sServo.uiServoSpeed = uiSpeed;
}



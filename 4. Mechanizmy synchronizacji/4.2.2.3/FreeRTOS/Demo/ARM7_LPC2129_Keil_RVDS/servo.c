#include <LPC21xx.H>
#include "FreeRTOS.h"
#include "task.h"
#include "servo.h"
#include "timer_interrupts.h"
#include "led.h"

#define   DETECTOR_bm (1<<10)

struct Servo sServo;

void DetectorInit(void){
	
	IO0DIR = IO0DIR &(~(DETECTOR_bm)); 
}

enum DetectorState {ACTIVE, INACTIVE};

enum DetectorState eReadDetector(void){
	if ((IO0PIN & DETECTOR_bm) == 0){
		return ACTIVE;
  }
	else {
		return INACTIVE;
	}
}

void ServoCallib(void){
	
	sServo.eState=CALLIB;

	
} 

void ServoGoTo(unsigned int uiPosition){
	
	sServo.eState = IN_PROGRESS;
	sServo.uiDesiredPosition = uiPosition;
}

void Automat(void *pvParameters)
{
		while(1)
		{
			switch(sServo.eState){
				case IDLE:
					if(sServo.uiCurrentPosition != sServo.uiDesiredPosition){
						sServo.eState = IN_PROGRESS;
					}
					else{
						sServo.eState = IDLE;
					}
					break;
				case IN_PROGRESS:
					if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
						sServo.uiCurrentPosition--;
						sServo.eState = IN_PROGRESS;
						
						LedStepLeft();
					}
					else if(sServo.uiCurrentPosition<sServo.uiDesiredPosition){
						sServo.uiCurrentPosition++;
						sServo.eState = IN_PROGRESS;
						
						LedStepRight();
					}
					else {
						sServo.eState = IDLE;
					}
					break;
				
				case CALLIB:
					if(eReadDetector()==INACTIVE){
						LedStepRight();
					}else{
						sServo.eState=IDLE;
						sServo.uiCurrentPosition=0;
						sServo.uiDesiredPosition=0;
					}
					break;
				default:
					break;
			}
			vTaskDelay(10);
		}
}

void ServoInit(unsigned int* uiServoFrequency)
{
	xTaskCreate(Automat, NULL, 128, uiServoFrequency, 1, NULL );
	ServoCallib();
	LedInit();
	DetectorInit();
} 

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
xQueueHandle xStatusQueue;

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
		struct Servo* pxMessage = &sServo;
		switch(sServo.eState){
				case CALLIB:
					if(eReadDetector() == ACTIVE){
						sServo.uiCurrentPosition = 0;
						uiDesiredPosition = 0;
						sServo.eState = IDLE;
						}
					else{
						LedStepRight();}
					xQueueOverwrite( xStatusQueue, &pxMessage);
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
					xQueueOverwrite( xStatusQueue, &pxMessage);
						break;
				 
			}
			vTaskDelay(sServo.uiServoSpeed);
	}
}

void ServoCallib(){	
	sServo.eState = CALLIB;

}

void ServoInit(unsigned int * uiServoFrequency)
{
	struct Servo* pxMessage = &sServo;
	LedInit();
	DetectorInit();
	xQueue = xQueueCreate(6, sizeof (int));
	xStatusQueue = xQueueCreate(1, sizeof(struct Servo*));
  xQueueSend( xStatusQueue , &pxMessage, ( TickType_t ) 0 );
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

struct Servo ServoStatus(void)
{
	return sServo;
}

struct Servo Servo_State(void)
{
	struct Servo sServoStatus;
	struct Servo* pxMessage;
	
	xQueuePeek(xStatusQueue, &pxMessage, portMAX_DELAY);
	
	sServoStatus = *pxMessage;
	
	return sServoStatus;
}



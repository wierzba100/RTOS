#include <LPC21xx.H>
#include "led.h"

#define LED0_bm 0x10000
#define LED1_bm 0x20000
#define LED2_bm 0x40000
#define LED3_bm 0x80000

enum eDirrection {LEFT, RIGHT};

void LedInit(){ //ustwia piny odpowiedzialne za ledy na wyjsciowe (ustawia jedynki tylko i wylacznie na tych bitach)
	
	IO1DIR = IO1DIR | (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	IO1SET = LED0_bm;

}

void LedOn(unsigned char ucLedIndeks){
	
		IO1CLR = (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	
	switch(ucLedIndeks){
		case(0):
			IO1SET = LED0_bm;
			break;
		case(1):
			IO1SET = LED1_bm;
			break;
		case(2):
			IO1SET = LED2_bm;
			break;
		case(3):
			IO1SET = LED3_bm;
			break;
		default:
			break;}
}

void LedStep(enum eDirrection Dirrection){
	
	static unsigned int uiCounter;
	switch(Dirrection){
		case(LEFT):
		uiCounter++;
		break;
		case(RIGHT):
		uiCounter--;
		break;}

	LedOn(uiCounter % 4);
	
}

void LedStepRight(){
	LedStep(RIGHT);
}

void LedStepLeft(){
	LedStep(LEFT);
}


void LedOff(unsigned char ucLedIndeks){
	
	switch(ucLedIndeks){
		case(0):
			IO1CLR = LED0_bm;
			break;
		case(1):
			IO1CLR = LED1_bm;
			break;
		case(2):
			IO1CLR = LED2_bm;
			break;
		case(3):
			IO1CLR = LED3_bm;
			break;
		default:
			break;}
}

void Led_Toggle(unsigned char ucLedIndeks){
	
	unsigned int state = 1;

	
	switch(ucLedIndeks){
		case(0):
			state = IO1PIN & LED0_bm;
			if(state){
				IO1CLR = LED0_bm;
			}
			else{
				IO1SET = LED0_bm;
			}
			break;
		case(1):
			state = IO1PIN & LED1_bm;
			if(state){
				IO1CLR = LED1_bm;
			}
			else{
				IO1SET = LED1_bm;
			}
			break;
		case(2):
			state = IO1PIN & LED2_bm;
			if(state){
				IO1CLR = LED2_bm;
			}
			else{
				IO1SET = LED2_bm;
			}
			break;
		case(3):
			state = IO1PIN & LED3_bm;
			if(state){
				IO1CLR = LED3_bm;
			}
			else{
				IO1SET = LED3_bm;
			}
			break;
		default:
			break;}
}

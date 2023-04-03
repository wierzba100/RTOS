#include <LPC21xx.H>
#include "keyboard.h"

#define BUT0_bm 0x10
#define BUT1_bm 0x40
#define BUT2_bm 0x20
#define BUT3_bm 0x80


enum KeyboardState ReadKeyboardState(){
	
		if((IO0PIN & BUT0_bm) == 0){
			return BUTTON_0;}
		else if((IO0PIN & BUT1_bm) == 0){
			return BUTTON_1;}
		else if((IO0PIN & BUT2_bm) == 0){
			return BUTTON_2;}
		else if((IO0PIN & BUT3_bm) == 0){
			return BUTTON_3;}
		else{
			return RELASED;}
}

void KeyboardInit(){ //ustawia bity odpowiedzialne za przyciski na wejsciowe (ustawia zera tylko na tych bitach)
	IO0DIR = IO0DIR & ~(BUT0_bm | BUT1_bm | BUT2_bm | BUT3_bm);
}	


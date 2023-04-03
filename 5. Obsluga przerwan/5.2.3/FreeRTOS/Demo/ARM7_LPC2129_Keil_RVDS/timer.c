#include <LPC21xx.H>
#include "timer.h"
#define TIMER_ENABLE_bm 1<<0
#define TIMER_RESET_bm 1<<1

#define MR0_RESET_bm 1<<1
#define MR0_INTERRUPT_bm 1<<0


void InitTimer0(void){
	T0TCR = T0TCR | TIMER_ENABLE_bm;
}

void WaitOnTimer0(unsigned int uiTime){
	
	T0TCR = T0TCR | TIMER_RESET_bm;
	T0TCR = T0TCR & ~(TIMER_RESET_bm);
	while(T0TC < (uiTime*15)){}

}
void InitTimer0Match0(unsigned int uiDeleyTime){
	
	T0MR0 = (uiDeleyTime*15);
	T0MCR = T0MCR | MR0_RESET_bm;
	T0MCR = T0MCR | MR0_INTERRUPT_bm;
	T0TCR = T0TCR | TIMER_ENABLE_bm;	
	T0TCR = T0TCR | TIMER_RESET_bm;
	T0TCR = T0TCR & ~(TIMER_RESET_bm);
	
}

void WaitOnTimer0Match0(){
	
	while((T0IR & MR0_INTERRUPT_bm)== 0){}
	T0IR = MR0_INTERRUPT_bm;
		
}

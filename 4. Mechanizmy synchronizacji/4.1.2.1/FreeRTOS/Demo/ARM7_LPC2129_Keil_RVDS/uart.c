#include <LPC210X.H>
#include "uart.h"
#include "string.h"

#define NULL '\0'
#define RECIEVER_SIZE 12
#define TERMINATOR '\r'
#define ON 1
#define OFF 0

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

#define RxD_ENABLE 0x4
#define TxD_ENABLE 0x1

////////////// Zmienne globalne ////////////


//---------------------- ODBIERANIE ZNAKOW --------------------------
//-------------------------------------------------------------------

char cOdebranyZnak;

struct RecieverBuffer{
	
	char cData[RECIEVER_SIZE];
	unsigned int ucCharCtr;
	enum eRecieverStatus eStatus;

};

struct RecieverBuffer eRecieverBuffer;

void Reciever_PutCharacterToBuffer(char cCharacter){
					
				if(eRecieverBuffer.ucCharCtr == RECIEVER_SIZE){
					eRecieverBuffer.eStatus = OVERFLOW;
				}
				else if(cCharacter == TERMINATOR){
					eRecieverBuffer.cData[eRecieverBuffer.ucCharCtr] = NULL;
					eRecieverBuffer.eStatus = READY;
					eRecieverBuffer.ucCharCtr++;
				}
				else{
					eRecieverBuffer.cData[eRecieverBuffer.ucCharCtr] = cCharacter;
					eRecieverBuffer.ucCharCtr++;
					eRecieverBuffer.eStatus = EMPTY;
				}

}

enum eRecieverStatus eReciever_GetStatus(void){
	return eRecieverBuffer.eStatus;
}

void Reciever_GetStringCopy(char *ucDestination){
	CopyString(eRecieverBuffer.cData, ucDestination);
	
	eRecieverBuffer.ucCharCtr = 0;
	eRecieverBuffer.eStatus = EMPTY;
}

//---------------------- WYSYLANIE ZNAKOW --------------------------
//------------------------------------------------------------------

struct TransmiterBuffer sTransmiterBuffer;

char Transmiter_GetCharacterFromBuffer(){
	
	if(sTransmiterBuffer.fLastCharacter == ON){
		sTransmiterBuffer.eStatus = FREE;
		return NULL;}
	else if(sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr] == NULL){
		sTransmiterBuffer.fLastCharacter = ON;
		//sTransmiterBuffer.eStatus = BUSY;
		return '\r';}
	else{
		//sTransmiterBuffer.eStatus = BUSY;
		return sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr++];
		}
	
}

void Transmiter_SendString(char cString[]){
	
	CopyString(cString, sTransmiterBuffer.cData);
	U0THR = sTransmiterBuffer.cData[0];
	sTransmiterBuffer.fLastCharacter = OFF;
	sTransmiterBuffer.eStatus = BUSY;
	sTransmiterBuffer.cCharCtr = 1;

}

enum eTransmiterStatus Transmiter_GetStatus(void){
	return sTransmiterBuffer.eStatus;
}

///////////////////////////////////////////
__irq void UART0_Interrupt (void){
   // jesli przerwanie z odbiornika (Rx)
	
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
			Reciever_PutCharacterToBuffer(U0RBR);
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
		
		unsigned char CharacterFromBuffer = Transmiter_GetCharacterFromBuffer();
		if(CharacterFromBuffer != NULL){
			U0THR = CharacterFromBuffer; //Wysylamy znak
		}
	}

   VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){

   // UART0
   PINSEL0 = PINSEL0 | RxD_ENABLE | TxD_ENABLE ;                 // ustawic pina na wysylanie uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = (((15000000)/16)/uiBaudRate);                      // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= mTHRE_INTERRUPT_ENABLE | mRX_DATA_AVALIABLE_INTERRUPT_ENABLE;;               // Inicjalizacja status lini przerwaD Rx??? co tu robinmy

   // INT
   VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
   VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
}

#include <LPC21xx.H>
#include "uart.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
/************ UART ************/
#define mP0_1_RxD_UART0_MODE  										 0x00000004
#define mP0_0_TxD_UART0_MODE											 0x00000001
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


#define NULL 0
////////////// Zmienne globalne ////////////
char cOdebranyZnak;
char cCharToSend;

/*struct ReceiverBuffer
{
	char cData[RECEIVER_SIZE];
	unsigned char ucCharCtr;
	enum eReceiverStatus eStatus;
};*/

struct ReceiverBuffer
{
	char cData[RECEIVER_SIZE];
	unsigned char ucCharCtr;
};

char Buffertemp[RECEIVER_SIZE];

xQueueHandle xReceiverCharQueue;
xQueueHandle xReceiverStringQueue;
xQueueHandle xTransmitterStringQueue;

struct ReceiverBuffer sReceiverBuffer;

char cUart_GetChar(void)
{
	char cOdebranyznak;
	if(xQueueReceive( xReceiverCharQueue, &cOdebranyznak, 10) == pdPASS)
	{
		return cOdebranyznak;
	}
	
	return NULL;
}



void Uart_GetString(char* String)
{	
	char cOdebranyznak;
	char cDatatemp[RECEIVER_SIZE];
	unsigned int i;
	if(xQueueReceive( xReceiverStringQueue, &cDatatemp, 10) == pdPASS)
	{
		for(i = 0;i < RECEIVER_SIZE; i++)
		{
			String[i] = cDatatemp[i];
		}
	}else
	{
		
	}
		
	if(xQueueReceive( xReceiverCharQueue, &cOdebranyznak, 10) == pdPASS)
	{
		if((sReceiverBuffer.ucCharCtr) >= RECEIVER_SIZE)
		{
			sReceiverBuffer.ucCharCtr = 0;
			sReceiverBuffer.cData[0] = '\0';
		}else if(cOdebranyznak == TERMINATOR)
		{
			sReceiverBuffer.cData[sReceiverBuffer.ucCharCtr] = NULL;
			xQueueSend(xReceiverStringQueue, &sReceiverBuffer.cData, portMAX_DELAY);
			sReceiverBuffer.ucCharCtr = 0;
		}else
		{
			sReceiverBuffer.cData[sReceiverBuffer.ucCharCtr] = cOdebranyznak;
			sReceiverBuffer.ucCharCtr++;
		}
	}
}



struct TransmiterBuffer{
	char cData[TRANSMITER_SIZE];
	enum eTransmiterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char cCharCtr;
};
struct TransmiterBuffer sTransmiterBuffer;

char cTransmiter_GetCharacterFromBuffer(){
	
	char cCurrentChar;
	cCurrentChar = sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr];
	if(cCurrentChar == NULL){
		
		if(sTransmiterBuffer.fLastCharacter == 0){
			
			sTransmiterBuffer.fLastCharacter = 1;
			return TERMINATOR;
		}
		else{
			sTransmiterBuffer.fLastCharacter = 0;
			sTransmiterBuffer.eStatus = FREE;
			return NULL;
		}
	}
	else{
		
		sTransmiterBuffer.cCharCtr++;
		return cCurrentChar;
	}
}

void Transmiter_SendString(char cString[]){
	
	sTransmiterBuffer.eStatus=BUSY;
	sTransmiterBuffer.cCharCtr=0;
	CopyString(cString, sTransmiterBuffer.cData );
	U0THR = cTransmiter_GetCharacterFromBuffer();
	
}

void SendStringFromQueue(void)
{
	char cDatatemp[TRANSMITER_SIZE];
	if(xQueueReceive( xTransmitterStringQueue, &cDatatemp, 10) == pdPASS)
	{
		Transmiter_SendString(cDatatemp);
	}else
	{
		
	}
}

void Uart_PutString(char *String)
{
	xQueueSend(xTransmitterStringQueue, String, portMAX_DELAY);
	SendStringFromQueue();
}


///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
      cOdebranyZnak = U0RBR;
			xQueueSend(xReceiverCharQueue, &cOdebranyZnak, portMAX_DELAY);
   }
	 
	 if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
      if(sTransmiterBuffer.eStatus==BUSY){
				U0THR = cTransmiter_GetCharacterFromBuffer();
			}
				// narazie nic nie wysylamy
   }
	 
   VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){
	
	unsigned long ulDivisor, ulWantedClock;
	ulWantedClock=uiBaudRate*16;
	ulDivisor=15000000/ulWantedClock;
	// UART
	PINSEL0 = PINSEL0 | 0x55;                                     // ustawic piny uar0 odbiornik nadajnik
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // d�ugosc s�owa, DLAB = 1
	U0DLL = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	ulDivisor >>= 8;
	U0DLM = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
	U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE ;               

	// INT
	VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
	
	sReceiverBuffer.ucCharCtr = 0;
	
	xReceiverCharQueue = xQueueCreate(RECEIVER_SIZE, sizeof (char));
	xReceiverStringQueue = xQueueCreate(RECEIVER_SIZE, sizeof (char[RECEIVER_SIZE]));
	xTransmitterStringQueue = xQueueCreate(RECEIVER_SIZE, sizeof (char[TRANSMITER_SIZE]));
}

#define TRANSMITER_SIZE 15

enum eRecieverStatus {EMPTY, READY, OVERFLOW};

void UART_InitWithInt(unsigned int);
void Reciever_PutCharacterToBuffer(char);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char *);

enum eTransmiterStatus {FREE, BUSY};

char Transmiter_GetCharacterFromBuffer(void);

struct TransmiterBuffer{
	
	char cData[TRANSMITER_SIZE];
	enum eTransmiterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char cCharCtr;
	
};

void Transmiter_SendString(char cString[]);
enum eTransmiterStatus Transmiter_GetStatus(void);

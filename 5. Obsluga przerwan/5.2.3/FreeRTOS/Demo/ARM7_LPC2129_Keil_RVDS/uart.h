#define TERMINATOR '\r' //13
#define RECEIVER_SIZE 12
#define TRANSMITER_SIZE 100

enum eReceiverStatus { EMPTY, READY, OVERFLOW};
enum eTransmiterStatus {FREE, BUSY};

void UART_InitWithInt(unsigned int uiBaudRate);
void Receiver_PutCharacterToBuffer(void);
enum eReceiverStatus eReceiver_GetStatus(void);
void Receiver_GetStringCopy(char *ucDestination);
void Transmiter_SendString(char cString[]);
enum eTransmiterStatus Transmiter_GetStatus(void);
char cUart_GetChar(void);
void cUart_GetString(char String[]);

#include "FreeRTOS.h"


enum DetectorState{ACTIVE, INACTIVE};
enum ServoSate {CALLIB, IDLE, IN_PROGRESS};

struct Servo{
	enum ServoSate eState;
	unsigned int uiCurrentPosition;
	//unsigned int uiDesiredPosition;
};

void ServoCallib(void);
void ServoInit(unsigned int *);
void ServoGoTo(unsigned int);



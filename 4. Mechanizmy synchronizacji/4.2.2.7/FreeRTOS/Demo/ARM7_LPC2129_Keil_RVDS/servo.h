#include "FreeRTOS.h"

enum DetectorState{ACTIVE, INACTIVE};
enum ServoSate {CALLIB, IDLE, IN_PROGRESS};

struct Servo{
	enum ServoSate eState;
	unsigned int uiCurrentPosition;
	unsigned int uiServoSpeed;
	//unsigned int uiDesiredPosition;
};

void ServoCallib(void);
void ServoInit(unsigned int *);
void ServoGoTo(unsigned int);
void ServoWait(TickType_t);
void ServoSpeed(unsigned int);



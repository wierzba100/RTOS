void DetectorInit(void);
void ServoInit(unsigned int uiServoFrequency);
void ServoGoTo(unsigned int uiPosition);
void ServoCallib(void);

enum ServoState {CALLIB, IDLE, IN_PROGRESS};

struct Servo
{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};



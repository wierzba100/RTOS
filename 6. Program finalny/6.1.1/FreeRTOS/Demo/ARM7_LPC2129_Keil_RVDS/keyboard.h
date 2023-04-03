enum KeyboardState {BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3, RELEASED};

void KeyboardInit(void);
void KeyboardThread( void *pvParameters );
enum KeyboardState eReadButtons(void);
enum KeyboardState eKeyboardDetector(void);
enum KeyboardState eKeyboard_Read(void);

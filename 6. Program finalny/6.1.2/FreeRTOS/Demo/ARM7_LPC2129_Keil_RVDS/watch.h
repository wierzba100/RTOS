enum TimeUnit {SECCONDS, MINUTES};

struct WatchEvent {
	enum TimeUnit eTimeUnit;
	char TimeValue;
};

void WatchInit(void);
struct WatchEvent sWatch_Read_Seconds(void);
struct WatchEvent sWatch_Read_Minutes(void);
char ElapsedSeconds(void);
char ElapsedMinutes(void);

#include <LPC21xx.H>
#include "watch.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "led.h"

struct WatchEvent Seconds;
struct WatchEvent Minutes;

struct WatchEvent sWatch_Read_Seconds(void)
{
	return Seconds;
}

char ElapsedMinutes(void)
{
	return Minutes.TimeValue;
}

char ElapsedSeconds(void)
{
	return Seconds.TimeValue;
}

struct WatchEvent sWatch_Read_Minutes(void)
{
	return Minutes;
}

void Timer (void *pvParameters)
{
	while(1)
	{
		if(Seconds.TimeValue > 59)
		{
			Seconds.TimeValue=0;
			Minutes.TimeValue++;
		}else
		{
			Seconds.TimeValue++;
		}
		vTaskDelay(1000);
	}
}

void WatchInit(void)
{
	Seconds.eTimeUnit = SECCONDS;
	Minutes.eTimeUnit = MINUTES;
	Seconds.TimeValue = 0;
	Minutes.TimeValue = 0;
	xTaskCreate( Timer, NULL , 100 , NULL, 1 , NULL );
}

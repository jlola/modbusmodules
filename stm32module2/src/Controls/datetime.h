#ifndef DATETIME
#define DATETIME

#include <stdint.h> 

typedef enum 
{
	second,
	min,
	hour
} ETimeUnit;

typedef struct {	
	int8_t seconds;
	int8_t minutes;
	int8_t hours;
	int8_t days;
	int8_t month;
	int16_t year;
} DateTime;

extern "C" uint8_t GetTimeValue(ETimeUnit timeUnit);

extern "C" void IncrementHours(int8_t hours);
extern "C" void IncrementSeconds(int8_t seconds);
extern "C" void IncrementMinutes(int8_t minutes);

extern "C" void IncrementYear(int8_t year);
extern "C" void IncrementDays(int8_t days);
extern "C" void IncrementMonth(int8_t month);

extern "C" void DecrementTime(DateTime *datetime);

extern "C" void IncrementTime(DateTime *datetime);
//return count of days in month
extern "C" int8_t GetDaysPerMonth(int8_t month,int8_t year);

//set time
extern "C" void SetTime(int8_t pHours,
			int8_t pMinutes,
			int8_t pSeconds,
			int8_t pDays,
			int8_t pMonth,
			int16_t pYear);

//incremet time 1 second
extern "C" void ProcessTime();

extern "C" DateTime *GetSystemTime();

extern "C" void GetSysTimeStr(char strTime[]);
//return act time in string
extern "C" void GetTimeStr(char strTime[],DateTime *datetime);

extern "C" void GetDateStr(char strDate[],DateTime *datetime);

extern "C" void GetSysDateStr(char strDate[]);
#endif

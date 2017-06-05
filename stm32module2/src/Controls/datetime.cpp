#include "datetime.h"
#include "string.h"
#include <stdint.h> 
#include <stdio.h>
#ifdef _AVR_
#include <avr/pgmspace.h>
#endif
//systemovy cas
DateTime systemtime;

void SetTime(int8_t pHours,
			int8_t pMinutes,
			int8_t pSeconds,
			int8_t pDays,
			int8_t pMonth,
			int16_t pYear)
{
	systemtime.seconds = pSeconds;
	systemtime.minutes = pMinutes;
	systemtime.hours = pHours;
	systemtime.days = pDays;
	systemtime.month = pMonth;
	systemtime.year = pYear;
}

uint8_t GetTimeValue(ETimeUnit timeUnit)
{	
	switch(timeUnit)
	{
		case second:
			return GetSystemTime()->seconds;
		break;
		case min:
			return GetSystemTime()->minutes;	
		break;
		case hour:
			return GetSystemTime()->hours;
		break;
	}
	return 0;
}

DateTime *GetSystemTime()
{
	return &systemtime;	
}

void IncrementHours(int8_t hours)
{
	systemtime.hours += hours;
	
	if (systemtime.hours > 23 && hours == 10) systemtime.hours = 0;
	if (systemtime.hours > 23 && hours == 1) systemtime.hours = 20;
	if (systemtime.hours < 0) systemtime.hours = 0;
}
void IncrementSeconds(int8_t seconds)
{
	systemtime.seconds += seconds;
	if (systemtime.seconds > 59 || systemtime.seconds < 0) systemtime.seconds = 0;
}
void IncrementMinutes(int8_t minutes)
{
	systemtime.minutes += minutes;
	if (systemtime.minutes > 59 || systemtime.minutes < 0) systemtime.minutes = 0;
}

void IncrementDays(int8_t days)
{
	systemtime.days+=days;
	if (systemtime.days>GetDaysPerMonth(systemtime.month,systemtime.year))
		systemtime.days = 1;
	if (systemtime.days<1) systemtime.days=GetDaysPerMonth(systemtime.month,systemtime.year);
}

void IncrementMonth(int8_t month)
{
	systemtime.month+=month;
	if (systemtime.month>12) systemtime.month=1;
	if (systemtime.month<1) systemtime.month=12;
	if (systemtime.days>GetDaysPerMonth(systemtime.month,systemtime.year))
		systemtime.days=GetDaysPerMonth(systemtime.month,systemtime.year);	
}

void IncrementYear(int8_t year)
{
	systemtime.year+=year;
}

void ProcessTime()
{
	IncrementTime(&systemtime);
}

void DecrementTime(DateTime *datetime)
{
	datetime->seconds--;
	if (datetime->seconds < 0) 
	{
		datetime->seconds = 59;
		datetime->minutes--;
	}
	if (datetime->minutes < 0)
	{
		datetime->minutes = 59;
		datetime->hours--;
	}
	if (datetime->hours < 0)
	{
		datetime->hours = 23;
		datetime->days--;
	}
	if (datetime->days < 0)
	{
		datetime->days = GetDaysPerMonth(datetime->month,datetime->year);
		datetime->month--;
	}
	if (datetime->month <= 0)
	{
		datetime->month=12;
		datetime->year--;
	}
}

//incremet time 1 second
void IncrementTime(DateTime *datetime)
{
	datetime->seconds++;
	if (datetime->seconds > 59) 
	{
		datetime->seconds = 0;
		datetime->minutes++;
	}
	if (datetime->minutes > 59)
	{
		datetime->minutes = 0;
		datetime->hours++;
	}
	if (datetime->hours > 23)
	{
		datetime->hours = 0;
		datetime->days++;
	}
	if (datetime->days > GetDaysPerMonth(datetime->month,datetime->year))
	{
		datetime->days = 1;
		datetime->month++;
	}
	if (datetime->month > 12)
	{
		datetime->month=1;
		datetime->year++;
	}
}

int16_t GetDays(DateTime *startTime,DateTime *stopTime)
{
	return 0;
}

int8_t GetDaysPerMonth(int8_t month,int8_t year)
{
	switch(month)
	{
		//leden
		case 1: return 31;
		//unor
		case 2:
			if ((year % 4)==0) return 29;  
			else return 28;
		//brezen
		case 3: return 31;
		//duben
		case 4: return 30;
		//kveten
		case 5: return 31;
		//cerven
		case 6: return 30;
		//cervenec
		case 7: return 31;
		//srpen
		case 8: return 31;
		//zari
		case 9: return 30;
		//rijen
		case 10: return 31;
		//listopad
		case 11: return 30;
		//prosinec
		case 12: return 31;
	}
	return 0;
}

#ifdef _AVR_
extern const char strPTime[] PROGMEM;
#endif
const char strPTime[] = "%02d:%02d:%02d";
//return act time in string
void GetTimeStr(char strTime[],DateTime *datetime)
{
#ifdef _AVR_
	snprintf_P(strTime,9,strPTime,datetime->hours,datetime->minutes,datetime->seconds);
#endif
}

#ifdef _AVR_
extern const char strPDate[] PROGMEM;
#endif

const char strPDate[] = "%02d.%02d.%04d";

void GetDateStr(char strDate[],DateTime *datetime)
{
#ifdef _AVR_
	snprintf_P(strDate,11,strPDate,datetime->days,datetime->month,datetime->year);
#endif
}

void GetSysDateStr(char strDate[])
{
	GetDateStr(strDate,&systemtime);
}

void GetSysTimeStr(char strTime[])
{
	GetTimeStr(strTime,&systemtime);
}


#include "Windows/WindowsPlatformTime/WindowsPlatformTime.h"

#include <mmsystem.h>



static LARGE_INTEGER Frequency;
static double SecondsPerCycle = -1;




double FWindowsPlatformTime::GetSecondsPerCycle()
{
	if( SecondsPerCycle < 0 )
	{
		QueryPerformanceFrequency(&Frequency);
		SecondsPerCycle = 1.0 / Frequency.QuadPart;
	}

	return SecondsPerCycle;
}

uint64 FWindowsPlatformTime::GetQPCFrequency()
{
	if( SecondsPerCycle < 0 )
	{
		QueryPerformanceFrequency(&Frequency);
		SecondsPerCycle = 1.0 / Frequency.QuadPart;
	}

	return Frequency.QuadPart;
}





void FWindowsPlatformTime::Sleep(uint32 Milliseconds)
{
	static const UINT periodMin = []
	{
		TIMECAPS tc;
		timeGetDevCaps(&tc, sizeof(TIMECAPS));
		return tc.wPeriodMin;
	}();

	// Set the timer resolution to the minimum for the Sleep call
	timeBeginPeriod(periodMin);

	// Wait...
	::Sleep(static_cast<DWORD>(Milliseconds));

	// Reset the timer resolution back to the system default
	timeEndPeriod(periodMin);
}

void FWindowsPlatformTime::SystemTime(int32& Year, int32& Month, int32& DayOfWeek, int32& Day, int32& Hour, int32& Min, int32& Sec, int32& MSec)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	Year = st.wYear;
	Month = st.wMonth;
	DayOfWeek = st.wDayOfWeek;
	Day = st.wDay;
	Hour = st.wHour;
	Min = st.wMinute;
	Sec = st.wSecond;
	MSec = st.wMilliseconds;
}

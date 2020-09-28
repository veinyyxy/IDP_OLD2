// JulianTime.cpp: implementation of the CJulianTime class.
//
//////////////////////////////////////////////////////////////////////

#include "JulianTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJulianTime::
CJulianTime(int dsec)
{
	LTC_dsec = dsec;
	UTC_date = LTC_date = 0;
	UTC_time = LTC_time = 0;
}

CJulianTime::
~CJulianTime()
{

}

void CJulianTime::
set_time(unsigned short dates, int seconds, int tc)
{
	LTC_date = UTC_date = dates;
	LTC_time = UTC_time = seconds;

	if(tc == UTC)
	{
		LTC_time += LTC_dsec;
		if( LTC_time < 0 )
		{
			--LTC_date;
			LTC_time += 86400;   //24*3600
		}
		else if( LTC_time >= 86400 )
		{
			++LTC_date;
			LTC_time -= 86400;   //24*3600
		}
	}

	else
	{
		UTC_time -= LTC_dsec;
		if( UTC_time < 0 ) 
		{
			--UTC_date;
			UTC_time += 86400;   //24*3600
		}
		else if(UTC_time >= 86400 )
		{
			++UTC_date;
			UTC_time -= 86400;   //24*3600
		}
	}

	return ;
}

void CJulianTime::
set_time(short yr, short mon, short day, \
	     short hr, short minute, short second, int tc)
{
	// unsigned short dates = DateToJulian(yr, mon, day);
    unsigned short dates = CJulMsGMT::Greg2Jul(yr, mon, day);
	int seconds = hr*3600+minute*60+second;

	set_time(dates, seconds, tc);

	return;
}


void CJulianTime::
set_eps_time(short yr, short mon, short day, \
			 short hr, short minute, short second,\
			 int eps_time, int tc)
{
	// unsigned short dates = DateToJulian(yr, mon, day);
    unsigned short dates = CJulMsGMT::Greg2Jul(yr, mon, day);
	int seconds = hr*3600+minute*60+second;

	set_eps_time(dates, seconds, eps_time, tc);

	return ;
}

void CJulianTime::
set_eps_time(unsigned short dates, int seconds, int eps_time, int tc)
{
	unsigned short eps_dates = dates;
	int eps_seconds = ((seconds+eps_time/2)/eps_time)*eps_time;

	if( eps_seconds >= 86400 )
	{
		++eps_dates;
		eps_seconds -= 86400;   //24*3600
	}
	
	set_time(eps_dates, eps_seconds, tc);

}

void CJulianTime::
get_time(unsigned short &dates, int &seconds, int tc)
{
	if(tc == UTC)  // get UTC
	{
		dates = UTC_date;
		seconds = UTC_time;
	}
	else          // get LTC
	{
		dates = LTC_date;
		seconds = LTC_time;
	}
}


void CJulianTime::
get_time(short &year, short &month, short &day, \
	     short &hour, short &minute, short &second, int tc)
{
	unsigned short dates;
	int seconds;

	get_time(dates,seconds, tc);
	// JulianToDate(dates, year, month, day);
    gInt32 y,m,d;
    CJulMsGMT::Jul2Greg(dates, y, m, d);
    year = (short)y;month = (short)m;day = (short)d;
	SecondsToTime(seconds, hour, minute, second);

	return;
}

void CJulianTime::
get_local_time(int span_seconds, unsigned short &dates,
			   int &seconds, int tc)
{
	get_time(dates,seconds, tc);
	seconds += span_seconds;

	if( seconds < 0 ) 
	{
		--dates;
		seconds += 86400;   //24*3600
	}
	else if( seconds >= 86400 )
	{
		++dates;
		seconds -= 86400;   //24*3600
	}

	return;
}

void CJulianTime::
get_local_time(int span_seconds, short &year, short &month, short &day, \
			   short &hour, short &minute, short &second, int tc)
{
	unsigned short dates;
	int seconds;

	get_local_time(span_seconds, dates, seconds, tc);

	// JulianToDate(dates, year, month, day);
    gInt32 y,m,d;
    CJulMsGMT::Jul2Greg(dates, y, m ,d);
    year = y;month = m; day = d;
	SecondsToTime(seconds, hour, minute, second);

	return;
}

//----------------------------------------------------------  CJulianTime -----------------
//
//////////////////// 由为年、月、日转换JulianDate///////////////////////////// 
int CJulianTime::
DateToJulian(int year, int mon, int day)
{
	int JulianDate = day-32075 + 1461*(year+4800+(mon-14)/12)/4  \
		+ 367*(mon-2-(mon-14)/12*12)/12 - 3*((year+4900+(mon-14)/12)/100)/4;
	
	return (JulianDate-Julian1970) ;
}

//----------------------------------------------------------  CJulianTime -----------------
//
//////////////////// 由Seconds转换为时、分、秒///////////////////////////// 
void CJulianTime::
JulianToDate(unsigned short Julian, short& year, short& month, short& day)
{
	int JLDAYN = Julian + Julian1970;
	int L = JLDAYN + 68569 ;
	int N = 4*L/146097 ;
		L = L - (146097*N+3)/4 ;
	int I = 4000*(L+1)/1461001 ;
		L = L-1461*I/4+31 ;
	int J = 80*L/2447 ;

	day = L-2447*J/80 ;
	L = J/11 ;
	month = J+2-12*L ;
	year = 100*(N-49)+I+L ;

	return ;
}

//----------------------------------------------------------  CJulianTime -----------------
//
//////////////////// 由Seconds转换为时、分、秒///////////////////////////// 
void CJulianTime::
SecondsToTime(int ulSeconds, short& hour, short& minute, short& second)
{
	hour   = (short)(ulSeconds/3600);
	minute = (short)((ulSeconds-hour*3600)/60);
	second = (short)(ulSeconds-hour*3600-minute*60);
}

//
// ============================================== bottom line ============================
//
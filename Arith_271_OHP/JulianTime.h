// JulianTime.h: interface for the CJulianTime class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __JULIANTIME_H_
#define __JULIANTIME_H_

#include "CJulMsGMT.h"

enum {UTC,LTC};
enum {UTC_seconds = 0, BTC_seconds = 28800};
const int Julian1970 = 2440587;

class CJulianTime
{
private:
	unsigned short UTC_date, LTC_date;
	int   UTC_time, LTC_time;
	int   LTC_dsec;

protected:
	int  DateToJulian(int year, int mon, int day);
	void JulianToDate(unsigned short Julian, short& year, short& month, short& day);
	void SecondsToTime(int ulSeconds, short& hour, short& minute, short& second);

public:
	CJulianTime(int dsec);
	virtual ~CJulianTime();

	void set_time(unsigned short dates, int seconds, int tc);
	void set_time(short yr, short mon, short day, \
				  short hr, short minute, short second, int tc);

	void set_eps_time(unsigned short dates, int seconds, int eps_time, int tc);

	void set_eps_time(short yr, short mon, short day, \
					  short hr, short minute, short second,\
					  int eps_time, int tc);

	void get_time(unsigned short &dates, int &seconds, int tc);
	void get_time(short &year, short &month, short &day, \
				  short &hour, short &minute, short &second, int tc);
	void get_local_time(int span_seconds, unsigned short &dates,
						int &seconds, int tc);
	void get_local_time(int span_seconds, short &year, short &month, short &day, \
						short &hour, short &minute, short &second, int tc);
};

#endif // !defined(AFX_JULIANTIME_H__13A895CA_E9ED_41AF_8272_3A6D70D2C16D__INCLUDED_)

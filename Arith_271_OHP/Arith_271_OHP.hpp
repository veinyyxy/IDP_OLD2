#ifndef _ARITH_271_OHP_HPP_
#define _ARITH_271_OHP_HPP_

#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include <math.h>
#include "Product_271_OHP.hpp"
#include "JulianTime.h"

struct tagDateLTC : public CJulianTime
{
	gInt16 year;
	gInt16 mon;
	gInt16 day;
	gInt16 hour;
	gInt16 min;
	gInt16 sec;

	gUint16 tar_date;
	gInt32  tar_time;

	gUint16 start_date;
	gInt32  start_time;

	gUint16 s_date_sub[2];
	gInt32  s_time_sub[2];
	gUint16 t_date_sub[2];
	gInt32  t_time_sub[2];	

	tagDateLTC() : CJulianTime(BTC_seconds)
	{};
	~tagDateLTC()
	{
	};

	int set(gInt16 y, 
		gInt16 mo,
		gInt16 d,
		gInt16 h,
		gInt16 mi,
		gInt16 sc)
	{
		year = y;
		mon = mo;
		day = d;
		hour = h;
		min = mi;
		sec = sc;

		set_time(year,mon,day,hour,min,sec,LTC);
		get_time(tar_date, tar_time, LTC);

		const int QPE_sec = 3600;    
		const int eps_time = 300;
		set_eps_time(tar_date, tar_time, eps_time, LTC);
		get_time(tar_date, tar_time, LTC);
		get_local_time(-QPE_sec, start_date, start_time, LTC);

		return 0;
	};

	gInt32 set_boundary_symmetry_sub_area(gUint16 left_size,
		gUint16 right_size)
	{
		set_time(start_date, start_time, LTC);
		get_local_time(left_size*-1, s_date_sub[0], s_time_sub[0], LTC);
		get_local_time(left_size*1, s_date_sub[1], s_time_sub[1], LTC);
		set_time(tar_date, tar_time, LTC);
		get_local_time(right_size*-1, t_date_sub[0], t_time_sub[0], LTC);
		get_local_time(right_size*1, t_date_sub[1], t_time_sub[1], LTC);

		return 0;
	};
};

typedef tagDateLTC DATE_LTC;

class Arith_271_OHP : public IArith
{
public:
	Arith_271_OHP();
	~Arith_271_OHP();

	SYSMETHOD(Initialize)();
	SYSMETHOD(LoadData)(void* pdata, ReadParam * Paramter);
	SYSMETHOD(OutPutData)(void* pdata);
	SYSMETHOD(Execute());
	SYSMETHOD(UnInitialize());

private:

	GHRESULT FilterQPRData(DATE_LTC& date);
	void ParseScantime(char *ScanDateTime,gInt32& year,gInt32& month,gInt32& day,gInt32& hour,gInt32& min,gInt32& sec);
	/*int  stringToInt(char *s);*/  //md by 20080827
	GXList<GenerationData *>* m_pInputList;
	GXList<GenerationData>* m_pOutputList;
	ReadParam *       m_fnParam;
};

#endif

 /////////////////////////////////////////////////////// 
 // CJulMsGMT.cpp 
 // created by chenxiang 20080407
 // for时间日期处理
 /////////////////////////////////////////////////////// 

#include "CJulMsGMT.h"
#include  <time.h> 
#include  <string.h>

bool CJulMsGMT::Jul2Greg( gUint16 JulDay,gInt32& Year,gInt32& Month,gInt32& Day ) 
	{
		 // 转换Julian天数为阳历日期（Gregorian）
		 // 检测儒略日
		if( JulDay < 1 || JulDay > 65533 )			//MS by chenxiang for 1to19700101 20080428
		{
			return false;							// 如果儒略日小于零或大于65533则返回false
		}
		gInt32 t1 = gInt32( JulDay ) + 2509157 -1;	//MS by chenxiang for 1to19700101 20080428
		gInt32 t2 = 4 * t1 / 146097;
		t1 -= ( 146097 * t2 + 3 ) / 4;
		gInt32 yy = 4000 * ( t1 + 1 ) / 1461001;
		t1 = t1 - 1461 * yy / 4 + 31;
		gInt32 mm = 80 * t1 / 2447;
		Day = gInt32( t1 - 2447 * mm / 80 );
		t1 = mm / 11;
		Month = gInt32( mm + 2 - 12 * t1 );
		Year = gInt32( 100 * ( t2 - 49 ) + yy + t1 );
		return true;
	}

gUint16 CJulMsGMT::Greg2Jul( gInt32 Year,gInt32 Month,gInt32 Day ) 
	{
		 // 转换阳历日期（Gregorian）为完整的儒略日( Julian )   // 此函数同时具有日期检查功能 如果日期不合法如2月30日则返回值为65535 // 依赖Jul2Greg()
		 // 检测日期
		if( Year < 1970 || Year > 2148 )  
		{
			return 65535;						// 年份小于1970或大于2148则返回65535
		}
		if( Month < 1 || Month > 12 )  
		{
			return 65535;						// 月份不在1至12月之间则返回65535
		}
		if( Day < 0 || Day > 31 )  
		{
			return 65535;						// 日期不超出范围，这里先给最大的范围 后面还有判断
		}
		gInt32 y = Year;						// 简化公式 下同
		gInt32 m = Month;
		gInt32 d = Day;	
		gUint16 JulDay = gUint16 ( d + 1461 *( y + ( m - 14 ) / 12 ) / 4
						   + 367 *(m - 2 - ( m - 14 ) / 12 * 12 ) / 12
						   - 3 *(( y + 100 + ( m - 14 ) / 12 ) / 100 ) / 4 - 719499 + 1 ); // 日期转儒略日//MS by chenxiang for 19700101to1 and clear up warning 20080428
		gInt32 y1 = y;							// 定义临时年月日 用于比较
		gInt32 m1 = m;
		gInt32 d1 = d;
		if( !Jul2Greg( JulDay,y1,m1,d1 ) )		// 把得到的儒略日转回日期做比较用,检测儒略日是否合法
		{
			return 65535;
		}
		if( y1 != y || m1 != m || d1 != d )		// 如果转回来的日期不等于原来日期则说明日期不合法如4月31日会变成5月1日说明日期4月31日不合法，返回65535
		{
			return 65535;
		}
		return JulDay;
	 }

gInt32 CJulMsGMT::Time2MilliSecond( gInt32 T_Hour,gInt32 T_Minute,gInt32 T_Second,gInt32 T_MilliSecond ) 
	{
		 // 时间转毫秒  // 此函数同时具有时间检查功能 如果时间不合法如25点61分则返回值为 - 1
		 // 检查时间是否合法
		if( T_Hour < 0 || T_Hour >= 24 )  
		{
			return -1;
		}
		if( T_Minute < 0 || T_Minute >= 60 )  
		{
			return -1;
		}
		if( T_Second < 0 || T_Second >= 60 ) 
		{
			return -1;
		}
		if( T_MilliSecond < 0 || T_MilliSecond >= 1000 )  
		{
			return -1;
		}
		return T_Hour * 3600 * 1000 + T_Minute * 60 * 1000 + T_Second * 1000 + T_MilliSecond;
	}

bool CJulMsGMT::MilliSecond2Time( gInt32 MilliSecond,gInt32& T_Hour,gInt32& T_Minute,gInt32& T_Second,gInt32& T_MilliSecond ) 
	{
		 // 毫秒转时间
		if( MilliSecond < 0 || MilliSecond >= ( 24 * 3600 * 1000 ) ) 
		{
			return false;						// 毫秒时间范围
		}
		T_Hour = gInt32( MilliSecond / 3600 / 1000 );
		MilliSecond -= T_Hour * 3600 * 1000;
		T_Minute = gInt32( MilliSecond / 60 / 1000 );
		MilliSecond -= T_Minute * 60 * 1000;
		T_Second = gInt32( MilliSecond / 1000 );
		T_MilliSecond = gInt32( MilliSecond - T_Second * 1000 );	
		return true;
	}

bool CJulMsGMT::BeijingTime2GMT( gInt32& Year,gInt32& Month,gInt32& Day,gInt32& T_Hour ) 
	{
		 // 北京时间转格林时间  // 依赖Jul2Greg()和Greg2Jul()
		if( T_Hour < 0 || T_Hour >= 24 )  
		{
			return false;						// 检查小时是否合法
		}
		if( Greg2Jul( Year,Month,Day ) == 65535 )  
		{
			return false;						// 用Greg2Jul函数来检查日期是否合法
		}
		if( T_Hour >= 8 ) 
		{
			T_Hour -= 8;						// 小时直接减8
		}
		else
		{
			if( gInt32( Greg2Jul( Year,Month,Day ) ) - 1 < 0 ) 
			{
				return false;					 // 如果北京时间转为格林时间后的日期早于1970年1月1日则返回false
			}
			Jul2Greg( Greg2Jul( Year,Month,Day ) - 1,Year,Month,Day ); // 把天数减一 先把日期转为儒略日再减一再把减一后的儒略日转回日期
			T_Hour = T_Hour + 24 - 8;			// 小时加24减8
		}
		return true;
	}

bool CJulMsGMT::GMT2BeijingTime( gInt32& Year,gInt32& Month,gInt32& Day,gInt32& T_Hour ) 
	{
		 // 格林时间转北京时间  // 依赖Jul2Greg() 和Greg2Jul() 
		if( T_Hour < 0 || T_Hour >= 24 )  
		{
			return false;						// 检查小时是否合法
		}
		if( Greg2Jul( Year,Month,Day ) == 65535 )  
		{
			return false;						// 用Greg2Jul函数来检查日期是否合法
		}
		if( T_Hour < 16 ) 
		{
			T_Hour += 8;						// 小时直接加8
		}
		else
		{
			T_Hour = T_Hour + 8 - 24;			// 小时加8减24
			Jul2Greg( Greg2Jul( Year,Month,Day ) + 1, Year, Month, Day ); // 把天数加一 先把日期转为儒略日再加一再把加一后的儒略日转回日期 // 儒略日加一不会超出65535
		}
		return true;
	}

bool CJulMsGMT::JulMsBJT2GMT( gUint16& JulDay,gInt32& MilliSecond ) 
	{
		 // 北京儒略日毫秒转格林儒略日毫秒
		if( JulDay < 1 || JulDay > 65533 ) 
		{
			return false;						// 儒略日小于零或大于65533 // 无符号不会小于0为了保险起见
		}
		if( MilliSecond < 0 || MilliSecond >= 24 * 3600 * 1000 ) 
		{
			return false;						// 毫秒范围限制
		}
		if( MilliSecond >= 8 * 3600 * 1000 ) 
		{
			MilliSecond -= 8 * 3600 * 1000;		// 大于八小时直接减八小时对应的毫秒
		}
		else
		{
			--JulDay;
			if( JulDay < 1 || JulDay > 65533 ) 
			{
				return false;					// 转换后格林时间儒略日小于1或大于65533则返回false
			}
			MilliSecond += 16 * 3600 * 1000;	// 小时加24减8等于加16小时对应的毫秒数
		}	
		return true;
	}
bool CJulMsGMT::JulMsGMT2BJT( gUint16& JulDay,gInt32& MilliSecond ) 
	{
		 // 格林儒略日毫秒转北京儒略日毫秒
		if( JulDay < 1 || JulDay > 65533 ) 
		{
			return false;						// 儒略日小于1或大于65533
		}
		if( MilliSecond < 0 || MilliSecond >= 24 * 3600 * 1000 ) 
		{
			return false;						// 毫秒范围限制
		}
		if( MilliSecond < 16 * 3600 * 1000 ) 
		{
			MilliSecond += 8 * 3600 * 1000;		// 小于八小时直接加八小时对应的毫秒
		}
		else
		{
			JulDay++;							// 儒略日增一 // 上面已经限制 所以不会超出65535
			MilliSecond -= 16 * 3600 * 1000;	// 小时加8减24等于减16小时对应的毫秒数
		}	
		return true;
	}
bool CJulMsGMT::GetLocalJulMs( gUint16& JulDay,gInt32& MilliSecond ) 
{
		// 获取当前儒略日毫秒函数 精度"秒" 没有精确到毫秒 毫秒归零 // 依赖Greg2Jul() 和Time2Millisecond() 
		time_t NowTime;
        struct tm TimeStruct;
        memset( &TimeStruct,0,sizeof( struct tm ) ); // 将tmstrTmp结构体里内容全赋'0'
        NowTime = time( NULL );

		#if defined ( WIN32 ) || defined ( WIN64 )
			localtime_s( &TimeStruct,&NowTime ); // 将获取的日期时间等信息传入TimeStruct中(里面存整型数据)
		#else
			TimeStruct = *localtime( &NowTime );
		#endif

		JulDay = Greg2Jul( TimeStruct.tm_year + 1900, TimeStruct.tm_mon + 1, TimeStruct.tm_mday ); // 将获取到的当前年月日转为儒略日
		MilliSecond = Time2MilliSecond( TimeStruct.tm_hour,TimeStruct.tm_min,TimeStruct.tm_sec,0 ); // 将获取到的当前时间转为毫秒计时
		return true;
}

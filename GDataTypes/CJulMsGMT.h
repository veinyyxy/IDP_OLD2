///////////////////////////////////////////////////////
//CJulMsGMT.h by chenxiang 
//created by chenxiang 20080407
//for日期时间处理
//儒略日和标准日期互相转换
//毫秒和标准时间互相转换
//标准日期时间的格林时间和北京时间互相转换
//儒略日毫秒的格林时间和北京时间的互相转换
//获取当前儒略日毫秒
///////////////////////////////////////////////////////

#pragma once

#ifndef		CJULMSGMT_H
#define		CJULMSGMT_H

#include	"GDef.h"
class DLLEXP_GSYS CJulMsGMT
{
public:
	static bool Jul2Greg(gUint16 JulDay,gInt32& Year,gInt32& Month,gInt32& Day);		//转换Julian天数为阳历日期（Gregorian）
	static gUint16 Greg2Jul(gInt32 Year,gInt32 Month,gInt32 Day);						//转换阳历日期（Gregorian）为完整的儒略日(Julian)//依赖Jul2Greg()
	static gInt32 Time2MilliSecond(gInt32 T_Hour,gInt32 T_Minute,gInt32 T_Second,gInt32 T_MilliSecond);//时间转毫秒
	static bool MilliSecond2Time(gInt32 MilliSecond,gInt32& T_Hour,gInt32& T_Minute,gInt32& T_Second,gInt32& T_MilliSecond);//毫秒转时间
	static bool BeijingTime2GMT(gInt32& Year,gInt32& Month,gInt32& Day,gInt32& T_Hour);	//北京时间转格林时间 由于北京转格林过程中小时以下的时间不会改变因此不提供分钟秒等参数//依赖Jul2Greg()和Greg2Jul()
	static bool GMT2BeijingTime(gInt32& Year,gInt32& Month,gInt32& Day,gInt32& T_Hour);	//格林时间转北京时间 //依赖Jul2Greg()和Greg2Jul()
	static bool JulMsBJT2GMT(gUint16& JulDay,gInt32& MilliSecond);						//北京儒略日毫秒转格林儒略日毫秒
	static bool JulMsGMT2BJT(gUint16& JulDay,gInt32& MilliSecond);						//格林儒略日毫秒转北京儒略日毫秒
	static bool GetLocalJulMs(gUint16& JulDay,gInt32& MilliSecond);						//获取当前儒略日毫秒 精度"秒"//依赖Greg2Jul()和Time2Millisecond()

private:
	
};

#endif
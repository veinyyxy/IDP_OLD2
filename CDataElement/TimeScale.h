#pragma once
/*============================================================ 
项目名： 人机交互平台
作 者： 韩猛
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：TimeScale.h  TimeScale.cpp
类 名： CTimeScale
父 类： 无
子 类： 无 
功能说明： 时间尺度
调用说明： 无
============================================================*/
#include "cdataelement_global.h"
#include <QString>
#include <QDateTime>
#include <QHash>


/************************************************************************/
/* 时间尺度 ：yyyy-MM-dd hh:mi:ss   
/* 1:yyyy  unit:year   display: 0001-00-00 00:00:00
/* 2:MM    unit:month  display: 0000-01-00 00:00:00
/* 3:dd    unit:day    display: 0000-00-01 00:00:00 
/* 4:hh    unit:hour   display: 0000-00-00 01:00:00 
/* 5:mi	   unit:minute display: 0000-00-00 00:01:00
/* 6:ss    unit:second display: 0000-00-00 00:00:01                                                         */
/************************************************************************/

class CDATAELEMENT_EXPORT CTimeScale
{
public:
	enum Enum_TimeUnit
	{
		Year = 1, 
		Month = 2,
		Day = 3, 
		Hour = 4,
		Munite = 5, 
		Second = 6,
		Season = 7, // 季度
		TenDays = 8, // 旬
		FiveDays = 9, // 侯
		FiveDaysOfChina = 10, // 中国侯
		Week = 11, 
		Decade = 12,// 年代
		Century = 13 //世纪

	};

	CTimeScale();
	CTimeScale(int value, int unit);
	CTimeScale(QString name);
	CTimeScale(const CTimeScale &ts);
	CTimeScale& operator =(const CTimeScale &ts);
	QString Description(); // eg vlaue=6, unit = hour , return 6 hour

	QString GetUnitDescription();
	int GetValue() const;
	int GetUnit() const;
	bool Valid() const;
private:
	int m_value; //值
	int m_unit; //单位 1:year, 2: month, 3:day, 4:hour, 5:minute, 6:second, 7:season  (季度), 8:ten_days(旬), 9:five_days（侯), 10: 中国侯， 11:week(周), 12:decade, 13 century

};



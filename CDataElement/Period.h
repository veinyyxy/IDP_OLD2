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
文件名：Period.h  Period.cpp
类 名： CPeriod
父 类： 无
子 类： 无 
功能说明： 时间范围
调用说明： 无
============================================================*/
#include "cdataelement_global.h"
#include "TimeScale.h"
	
class CDATAELEMENT_EXPORT CPeriod
{
public:
	CPeriod();
	CPeriod(const CPeriod &p);
	CPeriod& operator = (const CPeriod &p);
	~CPeriod();
	long Index(const QDateTime& dateTime ) const;
	QDateTime Value(long index);
	//时间尺度
	CTimeScale& GetTimeScale();
	void SetTimeScale(const CTimeScale& ts);
	void ResetCount();

	QDateTime m_begin; // 用户选择的开始时间
	QDateTime m_end;   //用户选择的结束时间
	long m_count;      // 用户选择的时间总数
	CTimeScale m_timeScale; //时间尺度
	QHash<long, QDateTime> m_hashDate; //实际时间范围
};

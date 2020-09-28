#pragma once
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QHash>
#include "TimeScale.h"


/*============================================================ 
项目名： 人机交互平台
作 者： 韩猛
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年5月17日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：basic.h
类 名： 无
父 类： 无
子 类： 无 
功能说明： 基本枚举类型定义
调用说明： 无
============================================================*/
#include "cdataelement_global.h"
#include "basic.h"

/************************************************************************
 功能说明：nc多维数据转置
 参    数：数据源 ，转置后数据，下标1最大值，下标二最大值，下标三最大值，数据排列顺序  
 作      者：陈志学
 完成日期：2010年4月28日
 更      新：陈志学
 更新日期：
 更新说明：

************************************************************************/

bool CDATAELEMENT_EXPORT DestData( float *sourData , float* destData,const int rangeA,const int rangeB,const int rangeC, Enum_DataOrder dataorder);
bool CDATAELEMENT_EXPORT GetDateInfoFromUnit(QDateTime& dateTime, CTimeScale &timeScale, const QString& time);
//bool CDATAELEMENT_EXPORT SetFirstDateTime(QDateTime& begin, const QDateTime& base, const CTimeScale &timeScale, long delt);
bool CDATAELEMENT_EXPORT SetDateTime(QDateTime& dateTime, const QDateTime& begin, const CTimeScale &timeScale, long delt);
bool CDATAELEMENT_EXPORT GetSteps(long &delt, const QDateTime& dateTime, const QDateTime& begin, const CTimeScale &timeScale);

/************************************************************************
 功能说明：将时间转换为对应的时间尺度下的字符串， 并按从小到大排序
 参    数：日期、时间尺度  
 作      者：韩猛
 完成日期：2010年6月13日
 更新日期：
 更新说明：

************************************************************************/
QStringList CDATAELEMENT_EXPORT GetDateStringList(const QHash<long, QDateTime> &dates, const CTimeScale &ts);
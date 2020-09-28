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
文件名：Station.h  Station.cpp
类 名： CStation
父 类： 无
子 类： 无 
功能说明： 站点
调用说明： 无
============================================================*/
#include "cdataelement_global.h"
#include <QString>
#include <QObject>

class CDATAELEMENT_EXPORT CStation 
{
public:
	CStation();
	CStation(const CStation& station);
	QString  c0; //Id;
	QString  ch_name; //chinese name
	QString v01000; // 区号	数字（以下要素均为实型数）当为船舶站时此字段为马士顿号
	double v05001; //纬度	单位:度 
	double v06001;  //经度	单位：度
	double v07001; //测站高度	单位：M
	double v07033;
	QString v02001; //台站类型	见编码表02001
	QString ch_area_province;
	QString ch_area_city;
	QString ch_area_county;
	QString by_status;
	QString ch_obs_type; //
	QString ch_business_type;
	QString ch_start_year;
	QString ch_start_month;
	QString ch_end_year;
	QString ch_end_month;
	QString rowid; // do not used
	CStation& operator =(const CStation & station);
};


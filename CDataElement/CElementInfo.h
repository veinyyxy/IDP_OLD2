/*================================================================================== 
项目名： 人机交互平台
作 者： 杨潇
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：CElementInfo.h CElementInfo.cpp
类 名： CElementInfo
父 类： NULL
子 类： 无 
功能说明： CIPAS系统数据需求规格说明书 3.2.13.1 NCEP再分析资料
调用说明： 

-------------------------------------版本更新-----------------------------------------
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：杨潇
完成日期：2011年05月09日
=====================================================================================*/
#pragma once
#include "cdataelement_global.h"

#include <QtCore/QString>
#include <QtCore/qlist.h>
#include <QtCore/qdatetime.h>
#include <QtCore/QPair.h>
#include <QtCore/qhash.h>

class CDATAELEMENT_EXPORT CElementInfo
{
public:
	CElementInfo();
	CElementInfo(QString time,QString type,QString elefile,QString en_name,QString ch_name,QString units,float offset,float rule,QString level,QString hr,QString vr,QString ranges,QString defaults,QString full_elefile);
	CElementInfo(const CElementInfo& ts);
	CElementInfo& operator =(const CElementInfo& ts);
	QString GetChineseName();              //获得要素中文名
private:
	QString m_time;
	QString m_en_name;
	QString m_type;
	QString m_elefile;
	QString m_ch_name;
	QString m_units;
	float m_offset;
	float m_rule;
	QString m_level;
	QString m_horizontal_resolution;
	QString m_vertical_resolution;
	QString m_ranges;
	QString m_default;
	QString m_full_elefile;
};

class CDATAELEMENT_EXPORT CElementFactory
{
public:
	static CElementFactory* GetInstance();                      //创建CElementFactory
	~CElementFactory();
	void InitAllElements();                                     //初始化
	CElementInfo* GetElementInfo(QString time, QString name);   //获得要素信息
	QHash<QPair<QString, QString>, CElementInfo*> GetElementData(){return m_elements;};
private:
	QHash<QPair<QString, QString>, CElementInfo*> m_elements;  

	CElementFactory();
	static CElementFactory* pFactory;
};
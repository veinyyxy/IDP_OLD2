#pragma once
/*================================================================================== 
项目名： 人机交互平台
作 者： 赵高攀
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：SingleStationElement.h SingleStationElement.cpp
类 名： SingleStationElement
父 类： NULL
子 类： 无 
功能说明：单站点要素数据信息，（用于绘制站点折线图）
调用说明： 

-------------------------------------版本更新----------------------------------------- 
V 1.0 
原作者 ：赵高攀
完成日期：2011年04月08日
=====================================================================================*/
#include "cdataelement_global.h"
#include "SingleStationElementData.h"
#include <QString>
#include <QVector>
#include <QHash>

class CDATAELEMENT_EXPORT SingleStationElement
{
public:
	SingleStationElement(void);
	~SingleStationElement(void);

	void SetElementName(QString &ElementName);
	QString GetElementName();

	void SetStationInfo(QString &StaionName,int ncount,double *px,double *py);

	QVector<QString>* GetStaionNameList();
	SingleStationElementData* GetStationData(QString &StationName);

private:
	QString m_ElementName;
	QVector<QString> m_StationNameList;
	QHash<QString,SingleStationElementData*> m_StationDataHash;
};


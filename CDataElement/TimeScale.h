#pragma once
/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ����
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����TimeScale.h  TimeScale.cpp
�� ���� CTimeScale
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ʱ��߶�
����˵���� ��
============================================================*/
#include "cdataelement_global.h"
#include <QString>
#include <QDateTime>
#include <QHash>


/************************************************************************/
/* ʱ��߶� ��yyyy-MM-dd hh:mi:ss   
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
		Season = 7, // ����
		TenDays = 8, // Ѯ
		FiveDays = 9, // ��
		FiveDaysOfChina = 10, // �й���
		Week = 11, 
		Decade = 12,// ���
		Century = 13 //����

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
	int m_value; //ֵ
	int m_unit; //��λ 1:year, 2: month, 3:day, 4:hour, 5:minute, 6:second, 7:season  (����), 8:ten_days(Ѯ), 9:five_days����), 10: �й�� 11:week(��), 12:decade, 13 century

};



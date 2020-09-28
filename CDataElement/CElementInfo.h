/*================================================================================== 
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
�ļ�����CElementInfo.h CElementInfo.cpp
�� ���� CElementInfo
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� CIPASϵͳ����������˵���� 3.2.13.1 NCEP�ٷ�������
����˵���� 

-------------------------------------�汾����-----------------------------------------
V 1.2 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ������
������ڣ�2011��05��09��
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
	QString GetChineseName();              //���Ҫ��������
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
	static CElementFactory* GetInstance();                      //����CElementFactory
	~CElementFactory();
	void InitAllElements();                                     //��ʼ��
	CElementInfo* GetElementInfo(QString time, QString name);   //���Ҫ����Ϣ
	QHash<QPair<QString, QString>, CElementInfo*> GetElementData(){return m_elements;};
private:
	QHash<QPair<QString, QString>, CElementInfo*> m_elements;  

	CElementFactory();
	static CElementFactory* pFactory;
};
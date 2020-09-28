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
�ļ�����Period.h  Period.cpp
�� ���� CPeriod
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ʱ�䷶Χ
����˵���� ��
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
	//ʱ��߶�
	CTimeScale& GetTimeScale();
	void SetTimeScale(const CTimeScale& ts);
	void ResetCount();

	QDateTime m_begin; // �û�ѡ��Ŀ�ʼʱ��
	QDateTime m_end;   //�û�ѡ��Ľ���ʱ��
	long m_count;      // �û�ѡ���ʱ������
	CTimeScale m_timeScale; //ʱ��߶�
	QHash<long, QDateTime> m_hashDate; //ʵ��ʱ�䷶Χ
};

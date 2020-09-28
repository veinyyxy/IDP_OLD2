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
�ļ�����Level.h
�� ���� CLevel
�� �ࣺ ��
�� �ࣺ �� 
����˵���� 
����˵���� ��
============================================================*/
#include "cdataelement_global.h"

#include <QString>
#include <QDateTime>
#include <QHash>	


class CDATAELEMENT_EXPORT CLevel
{

public:
	CLevel();
	CLevel(const CLevel &level);
	CLevel& operator=(const CLevel &level);
	~CLevel();
	typedef QPair<float , float > LevelRange;
	LevelRange m_range;
	QHash<int, float > m_valus;
	QHash<int, float > m_selected;
	long m_count;
	long m_selectedCount;
	QString m_units;
};

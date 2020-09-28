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
�ļ�����LonLatRange.h  LongLatRange.cpp
�� ���� CLonLatrange
�� �ࣺ ��
�� �ࣺ �� 
����˵���� 
����˵���� ��
============================================================*/
#include "cdataelement_global.h"
#include <QString>
#include <QDateTime>
#include <QHash>	

class CDATAELEMENT_EXPORT CLonLatRange
{
public:
	typedef float  ValueType;
	CLonLatRange();
	CLonLatRange(ValueType begin, ValueType end, ValueType delt, long int count, QString units, ValueType precision = 0.01);
	CLonLatRange(const CLonLatRange&range);
	CLonLatRange& operator = (const CLonLatRange& range);
	bool Valid() const; // return (end - begin) == delt * (count -1);
	long Index(const ValueType value) const;
	ValueType Value(const long Index) const;
	void ResetCount();
	bool operator == (const CLonLatRange & other); //�ж�2�����Ȼ�ά���Ƿ���ͬ
	bool operator != (const CLonLatRange & other); //�ж�2�����Ȼ�ά���Ƿ���ͬ

	ValueType m_begin;
	ValueType m_end;
	ValueType m_delt;
	long int m_count;
	ValueType m_precision;
	QString m_units;
};

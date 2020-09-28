#pragma once
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QHash>
#include "TimeScale.h"


/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ����
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��5��17��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����basic.h
�� ���� ��
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ����ö�����Ͷ���
����˵���� ��
============================================================*/
#include "cdataelement_global.h"
#include "basic.h"

/************************************************************************
 ����˵����nc��ά����ת��
 ��    ��������Դ ��ת�ú����ݣ��±�1���ֵ���±�����ֵ���±������ֵ����������˳��  
 ��      �ߣ���־ѧ
 ������ڣ�2010��4��28��
 ��      �£���־ѧ
 �������ڣ�
 ����˵����

************************************************************************/

bool CDATAELEMENT_EXPORT DestData( float *sourData , float* destData,const int rangeA,const int rangeB,const int rangeC, Enum_DataOrder dataorder);
bool CDATAELEMENT_EXPORT GetDateInfoFromUnit(QDateTime& dateTime, CTimeScale &timeScale, const QString& time);
//bool CDATAELEMENT_EXPORT SetFirstDateTime(QDateTime& begin, const QDateTime& base, const CTimeScale &timeScale, long delt);
bool CDATAELEMENT_EXPORT SetDateTime(QDateTime& dateTime, const QDateTime& begin, const CTimeScale &timeScale, long delt);
bool CDATAELEMENT_EXPORT GetSteps(long &delt, const QDateTime& dateTime, const QDateTime& begin, const CTimeScale &timeScale);

/************************************************************************
 ����˵������ʱ��ת��Ϊ��Ӧ��ʱ��߶��µ��ַ����� ������С��������
 ��    �������ڡ�ʱ��߶�  
 ��      �ߣ�����
 ������ڣ�2010��6��13��
 �������ڣ�
 ����˵����

************************************************************************/
QStringList CDATAELEMENT_EXPORT GetDateStringList(const QHash<long, QDateTime> &dates, const CTimeScale &ts);
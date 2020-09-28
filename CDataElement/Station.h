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
�ļ�����Station.h  Station.cpp
�� ���� CStation
�� �ࣺ ��
�� �ࣺ �� 
����˵���� վ��
����˵���� ��
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
	QString v01000; // ����	���֣�����Ҫ�ؾ�Ϊʵ��������Ϊ����վʱ���ֶ�Ϊ��ʿ�ٺ�
	double v05001; //γ��	��λ:�� 
	double v06001;  //����	��λ����
	double v07001; //��վ�߶�	��λ��M
	double v07033;
	QString v02001; //̨վ����	�������02001
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


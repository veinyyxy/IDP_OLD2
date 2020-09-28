#pragma once
/*================================================================================== 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� �Ը���
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����SingleStationElement.h SingleStationElement.cpp
�� ���� SingleStationElement
�� �ࣺ NULL
�� �ࣺ �� 
����˵������վ��Ҫ��������Ϣ�������ڻ���վ������ͼ��
����˵���� 

-------------------------------------�汾����----------------------------------------- 
V 1.0 
ԭ���� ���Ը���
������ڣ�2011��04��08��
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


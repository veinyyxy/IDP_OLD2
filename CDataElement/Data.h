#pragma once
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
#include <QList>
#include "BasicData.h"

//class CBasicData;

//����ָ��
class CDATAELEMENT_EXPORT CData
{
public:
	CData();
	CData(int size);
	CData(long *pSize, long iDims, float *pArray = 0);
	CData(QList<long> dimensions, float *pArray = 0);
	CData(const CBasicData& basicData);
	CData(const CData &data);
	CData& operator = (const CData &data);
	~CData();
	int DataBlocksCount() const; // �������ݿ�����
	bool RemoveDataBlock(int blockIndex);
	void AddData(const CBasicData& basicData);

	//ĳ�����ݿ��ά��
	long DataBlockDimensions(int blockIndex) const;
	// ĳ�����ݿ�ĳά�Ĵ�С
	long DataBlockDimensionSize(int blockIndex, long dimensionIndex) const;

	bool GetData(float &retVal, int blockIndex, long *pSize, long size);
	bool SetData(float value,   int blockIndex, long *pSize, long size);
	bool GetData(float &retVal, int blockIndex, QList<long> indexes) const;
	bool SetData(float value, int blockIndex, QList<long> indexes) const;
	
	float* GetDataArray(int blockIndex) const;
	CBasicData GetBasicData(int index);
private:
	void FreeData();
	
	
private:
	QList<CBasicData> m_basicDatas;
};


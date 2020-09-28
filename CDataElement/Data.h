#pragma once
/*============================================================ 
项目名： 人机交互平台
作 者： 韩猛
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年5月17日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：basic.h
类 名： 无
父 类： 无
子 类： 无 
功能说明： 基本枚举类型定义
调用说明： 无
============================================================*/
#include "cdataelement_global.h"
#include <QList>
#include "BasicData.h"

//class CBasicData;

//数据指针
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
	int DataBlocksCount() const; // 基础数据块数量
	bool RemoveDataBlock(int blockIndex);
	void AddData(const CBasicData& basicData);

	//某个数据块的维数
	long DataBlockDimensions(int blockIndex) const;
	// 某个数据块某维的大小
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


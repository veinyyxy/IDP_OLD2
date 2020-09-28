#pragma once
#include "cdataelement_global.h"
#include <QList>
#include "Reference.h"


//数据指针
class CDATAELEMENT_EXPORT CBasicData
{
public:
	CBasicData();
	CBasicData(long *pSize, long iDims, float *pArray = 0);
	CBasicData(QList<long> dimensions, float *pArray = 0);
	CBasicData(const CBasicData &data);
	CBasicData& operator = (const CBasicData &data);
	~CBasicData();
	long Dimensions() const;
	long DimensionSize(long index) const;
	bool GetData(float &retVal, long *pSize, long size) const;
	bool SetData(float value, long *pSize, long size) const;
	bool GetData(float &retVal, QList<long> indexes) const;
	bool SetData(float value, QList<long> indexes) const;
	float *GetDataArray() const;
	long TotalSize()  const;
private:
	bool Valid() const;
	bool InitData(float value) const;
	
private:
	float *m_pArray; //数据指针
	QList<long> m_dimensions;
	CReference m_ref; 
	int m_id; 
	int count;
};


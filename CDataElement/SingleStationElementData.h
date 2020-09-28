#pragma once
#include "cdataelement_global.h"
class CDATAELEMENT_EXPORT SingleStationElementData
{
public:
	SingleStationElementData();
	SingleStationElementData(int nCount,double *pftime,double *pfdata);
	~SingleStationElementData();

	void SetData(int nCount,double *pftime,double *pfdata);

	int GetCount();
	double *GetTimeArray();
	double *GetDataArray();
private:
	SingleStationElementData(const SingleStationElementData &data);
	SingleStationElementData& operator = (const SingleStationElementData &data);
private:
	double *m_ftimeArray;
	double *m_fdataArray;
	int m_nCount;
};


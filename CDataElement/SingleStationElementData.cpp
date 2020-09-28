#include "SingleStationElementData.h"


SingleStationElementData::SingleStationElementData(void)
{
	m_nCount = 0;
	m_fdataArray = NULL;
	m_ftimeArray = NULL;
}

SingleStationElementData::SingleStationElementData(int nCount,double *pftime,double *pfdata)
{
	m_nCount = nCount;
	m_fdataArray = new double[m_nCount];
	m_ftimeArray = new double[m_nCount];
	for (int i = 0;i<m_nCount;i++)
	{
		m_fdataArray[i] = pfdata[i];
		m_ftimeArray[i] = pftime[i];
	}
}

SingleStationElementData::~SingleStationElementData(void)
{
	m_nCount = 0;
	delete []m_ftimeArray;
	delete []m_fdataArray;
	m_fdataArray = NULL;
	m_ftimeArray = NULL;
}

void SingleStationElementData::SetData(int nCount,double *pftime,double *pfdata)
{
	m_nCount = nCount;
	delete []m_ftimeArray;
	delete []m_fdataArray;
	m_fdataArray = NULL;
	m_ftimeArray = NULL;
	m_fdataArray = new double[m_nCount];
	m_ftimeArray = new double[m_nCount];
	for (int i = 0;i<m_nCount;i++)
	{
		m_fdataArray[i] = pfdata[i];
		m_ftimeArray[i] = pftime[i];
	}
}

int SingleStationElementData::GetCount()
{
	return m_nCount;
}
double* SingleStationElementData::GetTimeArray()
{
	return m_ftimeArray;
}
double* SingleStationElementData::GetDataArray()
{
	return m_fdataArray;
}

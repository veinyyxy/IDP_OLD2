#include "Data.h"
#include "BasicData.h"

//////////////////////////////////// CData 
/************************************************************************/
/* 函数功能： 构造函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 05 月 17 日										*/
/************************************************************************/
CData::CData()
{
}

CData::CData(int size)
{
	for(int i = 0; i < size; ++i)
	{
		m_basicDatas.append(CBasicData());		
	}
}

CData::CData(long *pSize, long iDims, float *pArray)
{
	m_basicDatas.append(CBasicData(pSize, iDims, pArray));
}

CData::CData(QList<long> dimensions, float *pArray)
{
	m_basicDatas.append(CBasicData(dimensions, pArray));
}

CData::CData(const CBasicData& basicData)
{
	//CBasicData pData = basicData;
	m_basicDatas.append(CBasicData(basicData));
}

CData::CData(const CData &data)
{
	for(int i= 0; i < data.m_basicDatas.size(); ++i)
	{
		//CBasicData (data.m_basicDatas.at(i));
		m_basicDatas.append(data.m_basicDatas.at(i));
	}
}

CData& CData::operator = (const CData &data)
{
	if(this == &data)
	{
		return *this;
	}

	m_basicDatas.clear();
	// free memory
	//FreeData();
	for(int i = 0; i < data.m_basicDatas.size(); ++i)
	{
		//CBasicData *pData = data.m_basicDatas.at(i);
		m_basicDatas.append(data.m_basicDatas.at(i));
	}

	return *this;
}
/************************************************************************/
/* 函数功能： 析构函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CData::~CData()
{
	FreeData();
}

/************************************************************************/
/* 函数功能： 基础数据块数量           			                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_basicDatas.size()                                       */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
int CData::DataBlocksCount() const
{
	return m_basicDatas.size();
}

/************************************************************************/
/* 函数功能： 删除某个数据块数据       			                        */
/* 参数说明:  NULL														*/
/* 返 回 值： bool				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
bool CData::RemoveDataBlock(int index) 
{
	if(index >=0 && index < m_basicDatas.size())
	{
		m_basicDatas.takeAt(index);
		return true;
	}

	return false;
}

void CData::AddData(const CBasicData& basicData)
{
	m_basicDatas.append(basicData);
}

/************************************************************************/
/* 函数功能： 某个数据块的维数       			                        */
/* 参数说明:  index														*/
/* 返 回 值： pData->Dimensions()                                       */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
long CData::DataBlockDimensions(int index) const
{
	// CBasicData* pData = GetBasicData(index);
	// if(pData)
	// {
	//	 return pData->Dimensions();
	// }

	//return 0;
	if(index <0 || index >= m_basicDatas.size())
	{
		return 0;
	}

	CBasicData Data = m_basicDatas.at(index);
	return Data.Dimensions();

}
/************************************************************************/
/* 函数功能： 某个数据块的大小       			                        */
/* 参数说明:  blockIndex dimensionIndex									*/
/* 返 回 值： pData->DimensionSize(dimensionIndex)                      */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
long CData::DataBlockDimensionSize(int blockIndex, long dimensionIndex) const
{
	if(blockIndex <0 || blockIndex >= m_basicDatas.size())
	{
		return 0;
	}

	CBasicData data = m_basicDatas.at(blockIndex);

	//CBasicData* pData = GetBasicData(blockIndex);
	//if(pData)
	//{
	//	 return pData->DimensionSize(dimensionIndex);
	//}
	return data.DimensionSize(dimensionIndex);
	//return 0;
}

bool CData::GetData(float &retVal, int blockIndex, long *pSize, long size)
{
	if(blockIndex <0 || blockIndex >= m_basicDatas.size())
	{
		return false;
	}

	//CBasicData* pData = m_basicDatas.at(blockIndex);
	//if(pData)
	//{
	//	return pData->GetData(retVal, pSize, size);
	//}

	CBasicData data = m_basicDatas.at(blockIndex);
	data.GetData(retVal, pSize, size);
	return true;
}

bool CData::SetData(float value, int blockIndex, long *pSize, long size)
{
	if(blockIndex <0 || blockIndex >= m_basicDatas.size())
	{
		return false;
	}

	//CBasicData* pData = m_basicDatas.at(blockIndex);
	//if(pData)
	//{
	//	return pData->SetData(value, pSize, size);
	//}

	CBasicData data = m_basicDatas.at(blockIndex);
	data.SetData(value, pSize, size);

	return false;
}

bool  CData::GetData(float &retVal, int blockIndex, QList<long> indexes) const
{
	CBasicData data = m_basicDatas.at(blockIndex);
	return data.GetData(retVal, indexes);
}
bool  CData::SetData(float value, int blockIndex, QList<long> indexes) const
{
	CBasicData data = m_basicDatas.at(blockIndex);
	return data.SetData(value, indexes);
}

void CData::FreeData()
{
	while(!m_basicDatas.isEmpty())
	{
		 m_basicDatas.takeFirst();
	}
}

CBasicData CData:: GetBasicData(int index)
{
	if(index <0 || m_basicDatas.isEmpty() || index >= m_basicDatas.size())
	{
		NULL;
	}

	CBasicData pData = m_basicDatas.value(index); 
	return  pData;

	//QList<CBasicData*>::Iterator it = m_basicDatas.begin();
	//for(int i = 0; i <= index && it != m_basicDatas.end(); ++i, ++it)
	//{
	//	return (*it);
	//}
	//
	//return NULL;
}

float* CData::GetDataArray(int blockIndex) const
{
	if(blockIndex <0 || m_basicDatas.isEmpty() || blockIndex >= m_basicDatas.size())
	{
		return NULL;
	}

	CBasicData pBasicData = m_basicDatas.at(blockIndex);
	//if(!pBasicData)
	//{
	//	return NULL;
	//}

	return pBasicData.GetDataArray();
}
//////////////////////////////////// end CData 
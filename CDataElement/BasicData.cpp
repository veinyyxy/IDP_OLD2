#include "BasicData.h"
#include "Reference.h"
#include <QDebug>
//#include <vld.h>

//int CBasicData::count = 0;
static int id = 0;

//检验数据是否合法
bool Valid(QList<long> dimensions) 
{
	long iDims = dimensions.size();
	if(iDims <=0 )
	{
		return false;
	}

	long size = 1;
	for(long i = 0; i <iDims; ++i )
	{
		size *= dimensions.at(i);
	}

	if(size == 0)
	{
		return false;
	}

	return true;
}


CBasicData::CBasicData():m_pArray(new float(0))
{
	m_id = ++id;
	++count;
}

CBasicData::CBasicData( long *pSize, long iDims, float *pArray)
{
	m_id = ++id;
	++count;
	if(iDims <=0 || !pSize )
	{
		m_pArray = new float(0);
		return;
	}

	long size = 1;
	for(long i = 0; i <iDims; ++i )
	{
		m_dimensions << pSize[i];
		size *= pSize[i];
	}

	if(size == 0)
	{
		m_pArray = new float(0);
		m_dimensions.clear();
		return;
	}
	m_pArray = new float[size];
	
	if(!m_pArray)
	{
		m_dimensions.clear();
		return;
	}

	if(pArray)
	{
		for(long i = 0; i <size; i++)
		{
			m_pArray[i] = pArray[i];
		}
	}
	else
	{
		InitData(0);
	}	

}

CBasicData::CBasicData(QList<long> dimensions, float *pArray)
{
	m_id = ++id;
	++count;
	long size = 1;
	for(long i = 0; i <dimensions.size(); ++i )
	{
		m_dimensions << dimensions.at(i);
		size *= dimensions.at(i);
	}

	if(size == 0)
	{
		m_pArray = new float(0);
		m_dimensions.clear();
		return;
	}
	m_pArray = new float[size];

	if(!m_pArray)
	{
		m_dimensions.clear();
		return;
	}

	if(pArray)
	{
		for(long i = 0; i <size; i++)
		{
			m_pArray[i] = pArray[i];
		}
	}
	else
	{
		InitData(0);
	}	
}

CBasicData::CBasicData(const CBasicData &data)
{	
	
	// check data valid
	if(data.Valid())
	{
		m_pArray = data.m_pArray;
		m_dimensions = data.m_dimensions;
		m_ref = data.m_ref;
		m_id = data.m_id;
	}
	else
	{
		m_pArray = new float(0);
		m_id = ++id;
		++count;
	}
}




CBasicData& CBasicData::operator = (const CBasicData &data)
{
	// check data valid
	if(data.Valid())
	{
		//qDebug()<<" in function operator=, id:" << m_id << " ref=" << m_ref.GetRefCount();
		if(m_ref.Reattach(data.m_ref))
		{
			//qDebug() << "in function operator = , delete Array, id:" << m_id << m_dimensions;
			delete []m_pArray;
			--count;
		}

		m_pArray = data.m_pArray;
		m_dimensions.clear();
		m_dimensions = data.m_dimensions;
		m_ref = data.m_ref;
	}

	return *this;
}

CBasicData::~CBasicData()
{	

	//qDebug()<<" in function destructor, id:" << m_id << " ref=" << this->m_ref.GetRefCount();
	if(m_ref.Only())
	{
		//qDebug() << "in function destructor , delete Array, id:" << m_id << m_dimensions;
		delete []m_pArray;
		--count;
	}
	m_dimensions.clear();
	//qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Count:" << count;
}

long CBasicData::Dimensions() const
{
	return m_dimensions.size();
}

long CBasicData::DimensionSize(long index) const
{
	if(index >= 0 && index < m_dimensions.size())
	{
		return m_dimensions.at(index);
	}

	return 0;

}

bool CBasicData::GetData(float &retVal, long *pSize, long size)const
{
	if(size <= 0 || m_dimensions.isEmpty() || size > m_dimensions.size())
	{
		return false;
	}

	long index = 0;
	long subIndex = 0;
	long  formula = 1;
	for(long i = size-1; i>=0; --i)
	{
		if(pSize[i] < 0 || pSize[i] >= m_dimensions.at(i))
		{
			return false;
		}
		subIndex = pSize[i] * formula;
		index += subIndex;
		formula *= m_dimensions.at(i);
	}

	retVal = m_pArray[index];
	return true;
}

bool CBasicData::SetData(float value, long *pSize, long size)const
{
	if(size <= 0 || m_dimensions.isEmpty() || size > m_dimensions.size())
	{
		return false;
	}

	long index = 0;
	long subIndex = 0;
	long  formula = 1;
	for(long i = size-1; i>=0; --i)
	{
		if(pSize[i] < 0  || pSize[i] >= m_dimensions.at(i))
		{
			return false;
		}
		subIndex = pSize[i] * formula;
		index += subIndex;
		formula *= m_dimensions.at(i);
	}

	m_pArray[index] = value;
	return true;
}

bool CBasicData::GetData(float &retVal, QList<long> indexes)const
{
	if(indexes.isEmpty()|| m_dimensions.isEmpty() || indexes.size() > m_dimensions.size())
	{
		return false;
	}

	long index = 0;
	long subIndex = 0;
	long  formula = 1;
	for(long i = indexes.size() -1; i>=0; --i)
	{
		if(indexes.at(i) <= 0 || indexes.at(i) >= m_dimensions.at(i))
		{
			return false;
		}
		subIndex = indexes.at(i) * formula;
		index += subIndex;
		formula *= m_dimensions.at(i);
	}

	retVal = m_pArray[index];
	return true;
}
bool CBasicData::SetData(float value, QList<long> indexes)const
{
	if(indexes.isEmpty()|| m_dimensions.isEmpty() || indexes.size() > m_dimensions.size())
	{
		return false;
	}

	long index = 0;
	long subIndex = 0;
	long  formula = 1;
	for(long i = indexes.size() -1; i>=0; --i)
	{
		if(indexes.at(i) <= 0 || indexes.at(i) >= m_dimensions.at(i))
		{
			return false;
		}
		subIndex = indexes.at(i) * formula;
		index += subIndex;
		formula *= m_dimensions.at(i);
	}

	m_pArray[index] = value;
	return true;
}

float* CBasicData::GetDataArray() const
{
	return m_pArray;
}

//检验数据是否合法
bool CBasicData::Valid() const
{
	long iDims = m_dimensions.size();
	if(iDims <=0 )
	{
		return false;
	}

	long size = 1;
	for(long i = 0; i <iDims; ++i )
	{
		size *= m_dimensions.at(i);
	}

	if(size == 0)
	{
		return false;
	}

	return true;
}

//初始化数据
bool  CBasicData::InitData(float value) const
{
	int i = 0;
	for(; i < TotalSize(); ++i)
	{
		m_pArray[i] = value;
	}


	return i ? true : false;
}


long CBasicData::TotalSize()  const
{
	long iDims = m_dimensions.size();
	if(iDims <=0 )
	{
		return 0;
	}

	long size = 1;
	for(long i = 0; i <iDims; ++i )
	{
		size *= m_dimensions.at(i);
	}

	return size;
}
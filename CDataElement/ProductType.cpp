#include "ProductType.h"


CProductType::CProductType():m_key(-1)
{
}

CProductType::CProductType(int key, QString chName):m_key(key), m_chName(chName)
{
}
CProductType::CProductType(const CProductType& other):m_key(other.m_key), m_chName(other.m_chName)
{
}

CProductType& CProductType:: operator =(const CProductType& other)
{
	if(this == &other)
	{
		return *this;
	}
	
	m_key = other.m_key;
	m_chName = other.m_chName;

	return *this;
}

CProductType::~CProductType(void)
{
}


int CProductType::Key() const
{
	return m_key;
}

QString CProductType::ChineseName() const
{
	return m_chName;
}

bool CProductType::Valid() const
{
	return m_key > 0;
}

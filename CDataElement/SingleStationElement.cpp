#include "SingleStationElement.h"


SingleStationElement::SingleStationElement(void)
{
}


SingleStationElement::~SingleStationElement(void)
{
	m_StationNameList.clear();
	QHash<QString,SingleStationElementData*> :: iterator it = m_StationDataHash.begin();
	while(it != m_StationDataHash.end())
	{
		SingleStationElementData* pTemp = it.value();
		delete pTemp;
		pTemp = NULL;
		it = m_StationDataHash.erase(it);
	}
}

void SingleStationElement::SetElementName(QString &ElementName)
{
	m_ElementName = ElementName;
}
QString SingleStationElement::GetElementName()
{
	return m_ElementName;
}

void SingleStationElement::SetStationInfo(QString &StaionName,int ncount,double *px,double *py)
{
	m_StationNameList.append(StaionName);
	SingleStationElementData *pTemp = new SingleStationElementData(ncount,px,py);
	m_StationDataHash.insert(StaionName,pTemp);
	px = NULL;
	py = NULL;
}

QVector<QString>* SingleStationElement::GetStaionNameList()
{
	return &m_StationNameList;
}
SingleStationElementData* SingleStationElement::GetStationData(QString &StationName)
{
	if (m_StationDataHash.contains(StationName))
	{
		return m_StationDataHash.value(StationName);
	}else
		return NULL;
}


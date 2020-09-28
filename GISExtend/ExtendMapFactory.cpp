#include "ExtendMapFactory.h"
#include "ExtendMap.h"

ExtendMapFactory* ExtendMapFactory::m_pMe = 0;

ExtendMapFactory::ExtendMapFactory(void)
{
}

ExtendMapFactory::ExtendMapFactory( ExtendMapFactory& )
{

}

ExtendMapFactory::~ExtendMapFactory(void)
{
}

Map* ExtendMapFactory::CreateMap()
{
	ExtendMap* pMap = new ExtendMap();
	return pMap;
}

MapFactory* ExtendMapFactory::Instantiate()
{
	if(m_pMe == 0) 
	{
		m_pMe = new ExtendMapFactory;
		return m_pMe;
	}
	else
		return m_pMe;
}

void ExtendMapFactory::operator=( ExtendMapFactory& )
{

}

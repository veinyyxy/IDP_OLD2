#include "DefaultMapFactory.h"

GOTO_NAMESPACE_START

MapFactory* DefaultMapFactory::m_pMe = 0;

DefaultMapFactory::DefaultMapFactory(void)
{
}

DefaultMapFactory::DefaultMapFactory( DefaultMapFactory& )
{

}

DefaultMapFactory::~DefaultMapFactory(void)
{
}

Map* DefaultMapFactory::CreateMap()
{
	return new Map();
}

MapFactory* DefaultMapFactory::Instantiate()
{
	if(m_pMe == 0) 
	{
		m_pMe = new DefaultMapFactory;
		return m_pMe;
	}
	else
		return m_pMe;
}

void DefaultMapFactory::operator=( DefaultMapFactory& )
{

}

GOTO_NAMESPACE_END
#pragma once
#include "goConfig.h"
#include "mapfactory.h"
#include "Map.h"

GOTO_NAMESPACE_START

class DefaultMapFactory :
	public MapFactory
{
public:
	virtual ~DefaultMapFactory(void);

	virtual goto_gis::Map* CreateMap();
	static MapFactory* Instantiate();
private:
	DefaultMapFactory(void);
	DefaultMapFactory(DefaultMapFactory&);
	void operator=(DefaultMapFactory&);
	static MapFactory* m_pMe;
};

GOTO_NAMESPACE_END
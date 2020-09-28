#pragma once
#include "goConfig.h"
#include "Map.h"


class MapFactory
{
public:
	virtual goto_gis::Map* CreateMap() = 0;
};

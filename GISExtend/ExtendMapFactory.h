#pragma once
#include "goConfig.h"
#include <osgEarth/ImageUtils>
#include <osgEarthUtil/SkyNode>
#include <osgEarth/MapNode>
#include <OsgEarth/ModelLayer>

#include "Map.h"
#include "MapFactory.h"

using namespace goto_gis;

class ExtendMapFactory : public MapFactory
{
public:
	enum MAP_TYPE {BASE_MAP, EXTEN_MAP};
	
	~ExtendMapFactory(void);

	virtual Map* CreateMap();
	static MapFactory* Instantiate();
private:
	ExtendMapFactory(void);
	ExtendMapFactory(ExtendMapFactory&);
	void operator=(ExtendMapFactory&);
	static ExtendMapFactory* m_pMe;
};

#pragma once
#include "goConfig.h"
#include "VectorLayer.h"
#include "Layer.h"
//#include "RadarVectorLayer.h"
#include "CoordinateTransform.h"
#include "PluginsManager.h"
#include "MapFactory.h"

class RectResult
{
public:
	virtual void FinishDraw(const osg::Vec3Array* pVec3Array) = 0;
};



GOTO_NAMESPACE_START

class GEOSONTHEOSG_EXPORT GeosOnTheOsg
{
public:
	virtual ~GeosOnTheOsg(void);
	static GeosOnTheOsg* Instantiate();
	static void Uninstantiate();

	int LoadProviderPlugins(const std::string& strConfigName);
	int LoadGisExtendPlugins(const std::string& strConfigName);
	void OpenDataAsLayer(const std::string& strDataName , Layer* layer);
	int DynamicLoadPugin(const std::string& strPlName);
	/*É¾³ýµÄº¯Êý*/Layer* OpenRadarDataAsLayer(const std::string& strDataName , Map* map = 0, CoordinateTransform* coords = 0);
	MapFactory* GetMapFactory();

private:
	GeosOnTheOsg(void);
	GeosOnTheOsg(GeosOnTheOsg&){};
	void operator=(GeosOnTheOsg&){};
	

private:
	static GeosOnTheOsg* m_sGeosOnTheOsg;
	PluginsManager* m_pPluginManager;
	PluginsManager* m_pGisExtendPluginsManager;
};

GOTO_NAMESPACE_END
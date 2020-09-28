#pragma once
#include "goConfig.h"
#include "pluginsmanager.h"

class MapFactory;

class GisExtendPluginsManager :
	public PluginsManager
{
public:
	typedef MapFactory* (*GetGisExtendFactory)();

	GisExtendPluginsManager(void);
	virtual ~GisExtendPluginsManager(void);

	virtual int LoadPluginsFromConfigFile( const std::string& strConfigFile );

	virtual int LoadPlugin( const std::string& strPluginName );

	virtual void* GetDataProviderPluginFromID( int i );

	virtual void* GetMineFunctionFromID( int i );

	virtual int GetDataProviderPluginCount();
private:
	MapFactory* m_pGisExtendFactory;
};


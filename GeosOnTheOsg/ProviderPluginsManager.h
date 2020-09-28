#pragma once
#include <string>
#include <map>

#include "goConfig.h"
#include "PluginsManager.h"
#include "DataProvider.h"

GOTO_NAMESPACE_START

class ProviderPluginsManager : public PluginsManager
{
public:
	typedef DataProvider* (*DataProviderPluginMain)();
	typedef std::map<int, DataProviderPluginMain> PROVIDER_CREATER_MAP, *P_PROVIDER_CREATER_MAP;
	typedef std::pair<int, DataProviderPluginMain> PROVIDER_CREATER_PAIR, *P_PROVIDER_CREATER_PAIR;
	typedef bool (*MineData)(const std::string& strConfigFile);
	typedef std::map<int, MineData> MINEDATA_CREATER_MAP, *P_MINEDATA_CREATER_MAP;
	typedef std::pair<int, MineData> MINEDATA_CREATER_PAIR, *P_MINEDATA_CREATER_PAIR;

	ProviderPluginsManager(void);
	virtual ~ProviderPluginsManager(void);

	int LoadPluginsFromConfigFile(const std::string& strConfigFile);
	int LoadPlugin(const std::string& strPluginName);
	void* GetDataProviderPluginFromID(int i);
	void* GetMineFunctionFromID(int i);
	int GetDataProviderPluginCount();
private:
	PROVIDER_CREATER_MAP m_ProviderCreatorMap;
	MINEDATA_CREATER_MAP m_MineDataMap;
};

GOTO_NAMESPACE_END

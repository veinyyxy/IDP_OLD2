#pragma once
#include <string>

class PluginsManager
{
public:
	virtual int LoadPluginsFromConfigFile(const std::string& strConfigFile) = 0;
	virtual int LoadPlugin(const std::string& strPluginName) = 0;
	virtual void* GetDataProviderPluginFromID(int i) = 0;
	virtual void* GetMineFunctionFromID(int i) = 0;
	virtual int GetDataProviderPluginCount() = 0;
};


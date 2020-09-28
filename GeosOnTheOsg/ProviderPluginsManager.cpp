#include "ProviderPluginsManager.h"

#include <Windows.h>
#include <fstream>

GOTO_NAMESPACE_START

ProviderPluginsManager::ProviderPluginsManager(void)
{
}


ProviderPluginsManager::~ProviderPluginsManager(void)
{
}

int ProviderPluginsManager::LoadPluginsFromConfigFile(const std::string& strConfigFile )
{
	char strbuff[1024] = {0};

	std::ifstream fs(strConfigFile, std::ios::in/* | ios::out | ios::trunc*/);

	if (!fs.bad())
	{
		while(!fs.eof())
		{
			fs.getline(strbuff, 1024);

			char* cC = "<:>CheckPlugin";

			size_t len = strlen(strbuff);
			size_t chLen = strlen("<:>CheckPlugin");

			if(len + chLen + 2 < 1024)
			{
				strcpy((strbuff + len + 1), cC);
			}

			HMODULE hm = LoadLibraryA(strbuff);

			if(hm == NULL) continue;

			DataProviderPluginMain proMain = (DataProviderPluginMain)GetProcAddress(hm, "DataProviderPluginMain");
			MineData isMyData = (MineData)GetProcAddress(hm, "MineData");

			size_t uSize = m_ProviderCreatorMap.size();
			m_ProviderCreatorMap.insert(PROVIDER_CREATER_PAIR(uSize, proMain));
			m_MineDataMap.insert(MINEDATA_CREATER_PAIR(uSize, isMyData));
		}
		fs.close();
	}

	return 1;
}

int ProviderPluginsManager::LoadPlugin(const std::string& strPluginName )
{
	HMODULE hm = LoadLibraryA(strPluginName.c_str());
	typedef DataProvider* (*DataProviderPluginMain)();
	DataProvider* pProvider = 0;

	DataProviderPluginMain proMain = (DataProviderPluginMain)GetProcAddress(hm, "DataProviderPluginMain");
	
	size_t uSize = m_ProviderCreatorMap.size();
	m_ProviderCreatorMap.insert(PROVIDER_CREATER_PAIR(uSize, proMain));
	return uSize + 1;
}

void* ProviderPluginsManager::GetDataProviderPluginFromID( int i )
{
	PROVIDER_CREATER_MAP::iterator finded = m_ProviderCreatorMap.find(i);
	if(finded != m_ProviderCreatorMap.end())
	{
		return finded->second;
	}
	else
	{
		return 0;
	}
}

int ProviderPluginsManager::GetDataProviderPluginCount()
{
	return m_ProviderCreatorMap.size();
}

void* ProviderPluginsManager::GetMineFunctionFromID( int i )
{
	MINEDATA_CREATER_MAP::iterator finded = m_MineDataMap.find(i);
	if(finded != m_MineDataMap.end())
	{
		return finded->second;
	}
	else
	{
		return 0;
	}
}

GOTO_NAMESPACE_END
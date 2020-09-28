#include <Windows.h>
#include <string>
#include <map>
#include <fstream>

#include "GisExtendPluginsManager.h"


GisExtendPluginsManager::GisExtendPluginsManager(void)
{
	m_pGisExtendFactory = 0;
}


GisExtendPluginsManager::~GisExtendPluginsManager(void)
{
}

int GisExtendPluginsManager::LoadPluginsFromConfigFile( const std::string& strConfigFile )
{
	char strbuff[1024] = {0};

	std::ifstream fs(strConfigFile, std::ios::in);

	if (!fs.bad())
	{
		fs.getline(strbuff, 1024);

		/*char* cC = "<:>CheckPlugin";

		size_t len = strlen(strbuff);
		size_t chLen = strlen("<:>CheckPlugin");

		if(len + chLen + 2 < 1024)
		{
		strcpy((strbuff + len + 1), cC);
		}*/

		HMODULE hm = LoadLibraryA(strbuff);

		if(hm == NULL) return -1;

		GetGisExtendFactory proMain = (GetGisExtendFactory)GetProcAddress(hm, "GetGisExtendFactory");
		//MineData isMyData = (MineData)GetProcAddress(hm, "MineData");

		m_pGisExtendFactory = proMain();
		//size_t uSize = m_ProviderCreatorMap.size();
		//m_ProviderCreatorMap.insert(PROVIDER_CREATER_PAIR(uSize, proMain));
		//m_MineDataMap.insert(MINEDATA_CREATER_PAIR(uSize, isMyData));
		fs.close();
	}
	return 1;
}

int GisExtendPluginsManager::LoadPlugin( const std::string& strPluginName )
{
	HMODULE hm = LoadLibraryA(strPluginName.c_str());

	if(hm == NULL) return -1;

	GetGisExtendFactory proMain = (GetGisExtendFactory)GetProcAddress(hm, "GetGisExtendFactory");
	//MineData isMyData = (MineData)GetProcAddress(hm, "MineData");

	m_pGisExtendFactory = proMain();

	return 1;
}

void* GisExtendPluginsManager::GetDataProviderPluginFromID( int i )
{
	return m_pGisExtendFactory;
}

void* GisExtendPluginsManager::GetMineFunctionFromID( int i )
{
	return 0;
}

int GisExtendPluginsManager::GetDataProviderPluginCount()
{
	return -1;
}

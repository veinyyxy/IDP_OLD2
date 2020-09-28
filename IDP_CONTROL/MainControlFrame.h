#pragma once
#include "idp_control_global.h"
#include "CoordinateTransform.h"
#include "Map.h"
#include <osgViewer/View>

class ControllerManager;
class ControllerFrameContext;

class IDP_CONTROL_EXPORT MainControlFrame
{
public:
	typedef ControllerManager* (*GetControllerManager)();

	~MainControlFrame(void);
	int LoadFramePlugins(std::string& strPluginConfig);

	goto_gis::Map* CreateMapFromFactory(goto_gis::CoordinateTransform* pCT);
	osgViewer::Viewer* CreateOsgView(goto_gis::Map* pMap);
	static MainControlFrame* GetMainControlFrameInstance();
	int LoadControlManagerFromPlugin(const std::string& factoryName, const std::string& strPluginConfig, const std::string& functionName);
	ControllerManager* GetControlerManager(const std::string& strName);
	ControllerFrameContext* GetCurrentContext();
	void SetCurrentContext(ControllerFrameContext* pCFC);

private:
	MainControlFrame(void);
	void operator=(const MainControlFrame&);
	static MainControlFrame* s_pMainControlFrame;
	std::map<std::string, ControllerManager*> m_ManagerMap;
	ControllerFrameContext* m_pCurrentContext;
};


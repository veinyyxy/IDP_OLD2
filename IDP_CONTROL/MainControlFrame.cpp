#include "MainControlFrame.h"
#include "GeosOnTheOsg.h"
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventHandler>
#include <QtCore/QTextStream>
#include <QtCore/QLibrary>

class MapPointer : public osg::Referenced
{
public:
	inline void SetMap(goto_gis::Map* pMap){
		m_pMap = pMap;
	}
private:
	goto_gis::Map* m_pMap;
};

MainControlFrame* MainControlFrame::s_pMainControlFrame = 0;

MainControlFrame::MainControlFrame(void) : m_pCurrentContext(0)
{
	
}

MainControlFrame::~MainControlFrame(void)
{
}

int MainControlFrame::LoadFramePlugins(std::string& strPluginConfig)
{
	goto_gis::GeosOnTheOsg::Instantiate()->LoadProviderPlugins("PrividerConfig.txt");
	goto_gis::GeosOnTheOsg::Instantiate()->LoadGisExtendPlugins("GisExtendPlugins");

	/*QFile configFile(QString::fromStdString(strPluginConfig));
	configFile.open(QIODevice::ReadOnly);
	QTextStream ts(&configFile);

	while(!ts.atEnd())
	{
		QString strPlugin = ts.readLine();
		if(strPlugin.length() == 0) continue;

		QLibrary lib(strPlugin);
		if(lib.load())
		{
			GetControllerManager pToolBoxPluginMain = (GetControllerManager)lib
				.resolve("GetControllerManager");
			
			Controller* pController = pToolBoxPluginMain.GetControllerFromName("");
		}
	}*/
	return 1;
}

goto_gis::Map* MainControlFrame::CreateMapFromFactory(goto_gis::CoordinateTransform* pCT)
{
	MapFactory* pMapFactory 
		= goto_gis::GeosOnTheOsg::Instantiate()->GetMapFactory();

	goto_gis::Map* pMap = pMapFactory->CreateMap();
	pMap->SetCoordinateTransform(pCT);

	return pMap;
}

osgViewer::Viewer* MainControlFrame::CreateOsgView(goto_gis::Map* pMap)
{
	osgViewer::Viewer* pViewer = new osgViewer::Viewer();
	pViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
	pViewer->addEventHandler(new osgViewer::StatsHandler);
	pViewer->addEventHandler(new osgViewer::WindowSizeHandler);
	pViewer->addEventHandler(new osgGA::GUIEventHandler);
	if(pMap)
		pViewer->setSceneData(pMap->GetRootNode());
	return pViewer;
}

void MainControlFrame::operator=( const MainControlFrame& )
{

}

MainControlFrame* MainControlFrame::GetMainControlFrameInstance()
{
	if(s_pMainControlFrame == 0)
	{
		s_pMainControlFrame = new MainControlFrame();
		return s_pMainControlFrame;
	}
	else
	{
		return s_pMainControlFrame;
	}
}

int MainControlFrame::LoadControlManagerFromPlugin( const std::string& factoryName, const std::string& strPluginConfig, const std::string& functionName )
{
	QLibrary lib(QString::fromStdString(strPluginConfig));
	if(lib.load())
	{
		GetControllerManager pToolBoxPluginMain = (GetControllerManager)lib
			.resolve(functionName.c_str());

		
		if(pToolBoxPluginMain != 0 && 
			m_ManagerMap.find(factoryName) == m_ManagerMap.end())
		{
			m_ManagerMap[factoryName] = pToolBoxPluginMain();
			return 1;
		}
		else
			return 2;
	}

	return 0;
}

ControllerManager* MainControlFrame::GetControlerManager( const std::string& strName )
{
	std::map<std::string, ControllerManager*>::iterator iterFinded 
		= m_ManagerMap.find(strName);

	if(iterFinded != m_ManagerMap.end())
	{
		return iterFinded->second;
	}
	else
		return 0;
}

ControllerFrameContext* MainControlFrame::GetCurrentContext()
{
	return m_pCurrentContext;
}

void MainControlFrame::SetCurrentContext( ControllerFrameContext* pCFC )
{
	m_pCurrentContext = pCFC;
}


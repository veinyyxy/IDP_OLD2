#include "MainFrameWindow.h"
#include "ToolBoxInterface.h"
#include "ToolBoxesFactory.h"
#include "MainControlFrame.h"
#include "ActionFactory.h"
#include "ControllerManager.h"
#include "ControllerFrameContextImps.h"
#include "WindowFrameContextImps.h"
#include "DockWidgetInterface.h"
#include "DockWidgetFactory.h"
#include "UpdateSlaveInterface.h"

MainFrameWindow* MainFrameWindow::s_pMainFrameWindow = 0;

MainFrameWindow::MainFrameWindow(void) 
	: m_pControllerFrameContext(new ControllerFrameContextImps)
	, m_pWindowFrameContext(new WindowFrameContextImps)
{
	m_pQMdiArea = new QMdiArea(this);
	m_pActionGroup = new QActionGroup(this);
	QToolBar* pToolBar = new QToolBar("Main Tools...", this);
	m_pToolTabs = new QTabWidget(pToolBar);
	pToolBar->addWidget(m_pToolTabs);
	this->addToolBar(pToolBar);

	connect(m_pActionGroup, SIGNAL(triggered( QAction *)), SLOT(slot_ActionTriggered(QAction*)));
	connect(m_pQMdiArea, SIGNAL(subWindowActivated ( QMdiSubWindow *)), SLOT(slot_subWindowActivated ( QMdiSubWindow *)));
	this->setCentralWidget(m_pQMdiArea);

	MainControlFrame* pMCF = MainControlFrame::GetMainControlFrameInstance();
	pMCF->SetCurrentContext(m_pControllerFrameContext);

	QMenuBar* SystemMenu = new QMenuBar(this);
	SystemMenu->setWindowIcon(QIcon(":/images/defaultToolBar/skin.png"));
	this->setMenuBar(SystemMenu);
}


MainFrameWindow::~MainFrameWindow(void)
{
}


int MainFrameWindow::LoadDockWidgetPlugins()
{
	return 0;
}

int MainFrameWindow::LoadToolBoxPlugins(QString strConfigFile)
{

	/*QFile configFile(strToolBoxConfig);
	configFile.open(QIODevice::ReadOnly);
	QTextStream ts(&configFile);

	while(!ts.atEnd())
	{
		QString strPlugin = ts.readLine();
		if(strPlugin.length() == 0) continue;

		QLibrary lib(strPlugin);
		if(lib.load())
		{
			GetToolBoxesFactory pToolBoxPluginMain = (GetToolBoxesFactory)lib
				.resolve("GetToolBoxesFactory");
			ToolBoxesFactory* pToolBoxFactoryInterface = pToolBoxPluginMain();
			int iCount = pToolBoxFactoryInterface->GetToolBoxCount();
			for(int i = 0; i < iCount; i++)
			{
				ToolBoxInterface* pToolBoxInterface = pToolBoxFactoryInterface
					->GetToolBoxesInterface(i);
				m_pToolTabs->addTab(pToolBoxInterface->GetToolTableWidget()
					, pToolBoxInterface->GetToolTableName());
			}
		}
	}*/

	MainControlFrame* pMainControlFrame = MainControlFrame::GetMainControlFrameInstance();
	pMainControlFrame->LoadFramePlugins(std::string(""));
	QFile file(strConfigFile);
	if(file.open(QFile::ReadOnly))
	{
		int i = 0, j = 0;

		unsigned int nbgColor;
		QDomDocument XmlReader;
		XmlReader.setContent(&file);

		QDomNodeList nodeList = XmlReader.elementsByTagName(QString::fromLocal8Bit("Meteorological_Frame"));
		int iCao = nodeList.size();

		if(iCao == 0) return 0;

		QDomNode domNode = nodeList.at(0);

		nodeList = domNode.childNodes();
		for(int i = 0; i < nodeList.length(); i++)
		{
			QDomNode domNode = nodeList.at(i);
			
			QString strNodeName = domNode.nodeName();
			if(strNodeName == QString::fromLocal8Bit("ToolsGroup"))
			{
				QDomElement Info_element =  domNode.toElement();
				QString strPluginName = Info_element.attribute(QString::fromLocal8Bit("name"));
				QDomNodeList pluginNodeList = domNode.childNodes();
				
				for(int j = 0; j < pluginNodeList.length(); j++)
				{
					domNode = pluginNodeList.at(j);
					QString strChildName = domNode.nodeName();
					GetToolBoxesFactory pToolBoxPluginMain = 0;
					GetActionFactory pActionFactory = 0;
					GetDisplayViewFactory pDisplayViewFactory = 0;

					if(strChildName == QString::fromLocal8Bit("ToolBoxPlugins"))
					{
						QDomElement domElement =  domNode.toElement();
						QString strFunctionName = domElement.attribute(QString::fromLocal8Bit("factory_function"));
						QString strModuleName = domElement.attribute(QString::fromLocal8Bit("module"));

						QLibrary lib(strModuleName);
						pToolBoxPluginMain = (GetToolBoxesFactory)lib
							.resolve(strFunctionName.toStdString().c_str());
						ToolBoxesFactory* pToolBoxFactoryInterface = pToolBoxPluginMain();
						m_ToolBoxFactoryMap.insert(strPluginName, pToolBoxFactoryInterface);
					}
					else if(strChildName == QString::fromLocal8Bit("ControllerPlugins"))
					{
						QDomElement domElement =  domNode.toElement();
						QString strFunctionName = domElement.attribute(QString::fromLocal8Bit("factory_function"));
						QString strModuleName = domElement.attribute(QString::fromLocal8Bit("module"));

						pMainControlFrame->LoadControlManagerFromPlugin(
							strPluginName.toStdString()
							, strModuleName.toStdString()
							, strFunctionName.toStdString());
					}
					else if(strChildName == QString::fromLocal8Bit("ActionFactory"))
					{
						QDomElement domElement =  domNode.toElement();
						QString strFunctionName = domElement.attribute(QString::fromLocal8Bit("factory_function"));
						QString strModuleName = domElement.attribute(QString::fromLocal8Bit("module"));

						QLibrary lib(strModuleName);
						pActionFactory = (GetActionFactory)lib
							.resolve(strFunctionName.toStdString().c_str());
						ActionFactory* pActionFactoryInterface = pActionFactory();
						m_ActionFactoryMap.insert(strPluginName, pActionFactoryInterface);
					}
					else if(strChildName == QString::fromLocal8Bit("DisplayViewPlugins"))
					{
						QDomElement domElement =  domNode.toElement();
						QString strFunctionName = domElement.attribute(QString::fromLocal8Bit("factory_function"));
						QString strModuleName = domElement.attribute(QString::fromLocal8Bit("module"));

						QLibrary lib(strModuleName);
						pDisplayViewFactory = (GetDisplayViewFactory)lib
							.resolve(strFunctionName.toStdString().c_str());
						DisplayViewFactory* pDVF = pDisplayViewFactory();
						m_DisplayViewFactory.insert(strPluginName, pDVF);
					}

					else if(strChildName == QString::fromLocal8Bit("TooxBoxConfig"))
					{
						QDomElement domElement =  domNode.toElement();
						QString strName = domElement.attribute(QString::fromLocal8Bit("name"));
						ToolBoxesFactory* pToolBoxesFactory 
							= m_ToolBoxFactoryMap.value(strPluginName);
						ToolBoxInterface* pToolBoxInterface 
							= pToolBoxesFactory->GetToolBoxesInterface(strName);

						QDomNodeList ActionNodeList = domNode.childNodes();

						ActionFactory* pActionFactory 
							= m_ActionFactoryMap.value(strPluginName);

						for(int i = 0; i < ActionNodeList.size(); i++)
						{
							QDomNode actionNode = ActionNodeList.at(i);

							QDomElement actionElement = actionNode.toElement();

							QString strActionName = actionElement
								.attribute(QString::fromLocal8Bit("name"));

							MyActionInterface* pAction 
								= pActionFactory->GetActionFromName(strActionName);

							pToolBoxInterface->AddAction(strActionName, pAction);

							m_pActionGroup->addAction(pAction);
						}
						m_pToolTabs->addTab(
							pToolBoxInterface->GetToolTableWidget()
							, pToolBoxInterface->GetToolTableName());
					}

					else if(strChildName == QString::fromLocal8Bit("ActionToControllerMap"))
					{
						QDomNodeList mapNodeList = domNode.childNodes();
						for(int k = 0; k < mapNodeList.length(); k++)
						{
							QDomNode mapNode = mapNodeList.at(k);
							QDomElement mapElement =  mapNode.toElement();
							QString strActionName = mapElement.attribute("action");
							QString strControllerName = mapElement.attribute("controller");

							ControllerManager* pControllerManager 
								= pMainControlFrame->GetControlerManager(
								strPluginName.toStdString());

							ActionFactory* pActionFactory 
								= m_ActionFactoryMap.value(strPluginName);
							MyActionInterface* pAction = pActionFactory
								->GetActionFromName(strActionName);

							Controller* pControler = pControllerManager->
								GetControllerFromName(
								strControllerName.toStdString());

							DisplayViewFactory* pDisplayViewFactory = 
								GetDisplayViewFactoryFromName(strPluginName);
							pAction->SetController(pControler);
							pAction->SetDisplayViewFactory(pDisplayViewFactory);
						}
					}
				}
			}
			
		}

		nodeList = XmlReader.elementsByTagName(QString::fromLocal8Bit("DockWidget"));
		iCao = nodeList.size();

		if(iCao == 0) return 0;

		for(int i = 0; i < nodeList.length(); i++)
		{
			QDomNode domNode = nodeList.at(i);
			LoadDockWidget(&domNode);
		}

		nodeList = XmlReader.elementsByTagName(QString::fromLocal8Bit("UpdateMap"));

		for(int i = 0; i < nodeList.length(); i++)
		{
			QDomNode domNode = nodeList.at(i);
			LoadUpdateMap(&domNode);
		}
	}
	return 0;
}

int MainFrameWindow::LoadDisplyAreaPlugins()
{
	return 0;
}

void MainFrameWindow::operator=( const MainFrameWindow& )
{

}

MainFrameWindow* MainFrameWindow::GetUIMainFramInstance()
{
	if(s_pMainFrameWindow == 0)
	{
		s_pMainFrameWindow = new MainFrameWindow();
		return s_pMainFrameWindow;
	}
	else
	{
		return s_pMainFrameWindow;
	}
}

DisplayViewFactory* MainFrameWindow::GetDisplayViewFactoryFromName( const QString& strName )
{
	return m_DisplayViewFactory.value(strName);
}

void MainFrameWindow::slot_ActionTriggered( QAction* action )
{
	MyActionInterface* pMyAction = dynamic_cast<MyActionInterface*>(action);
	pMyAction->MyAction();
}

QMdiSubWindow* MainFrameWindow::AddSubWindow( QWidget* pSub )
{
	return m_pQMdiArea->addSubWindow(pSub);
}

QMdiSubWindow* MainFrameWindow::GetCurrentWindow()
{
	return 	m_pQMdiArea->activeSubWindow();
}

goto_gis::Map* MainFrameWindow::GetCurrentMap()
{

	return 0;
}

osgViewer::Viewer* MainFrameWindow::GetCurrentViewer()
{
	return 0;
}

void MainFrameWindow::slot_subWindowActivated( QMdiSubWindow * window )
{
	if(!window) return;

	ViewerQT* pWidget = dynamic_cast<ViewerQT*>(window->widget());
	WindowFrameContextImps* pwfc = 
		dynamic_cast<WindowFrameContextImps*>(m_pWindowFrameContext);
	pwfc->SetMdiSubWindow(window);
	pwfc->SetViewerQT(pWidget);

	ControllerFrameContextImps* pcfc = 
		dynamic_cast<ControllerFrameContextImps*>(m_pControllerFrameContext);
	osgViewer::Viewer* pViewer = pWidget->getViewer();
	pcfc->SetCurrentViewer(pViewer);

	QVariant qVar = pWidget->property("goto_gis::Map");
	ULONGLONG ullMapPointer = qVar.toULongLong();
	pcfc->SetCurrentMap((goto_gis::Map*)ullMapPointer);

	//////////////////////////////////////////////////////////////////////////
	
	UpdateSlave();
	/*for(int i = 0; i < m_UpdateDockVector.size(); i++)
	{
	DockWidgetInterface* m_pDWI = m_UpdateDockVector.at(i);
	m_pDWI->Update();
	}*/
}

void MainFrameWindow::LoadDockWidget( QDomNode* pDN )
{
	QString strNodeName = pDN->toElement().attribute("name");
	QString strMasterUpdate = pDN->toElement().attribute("master_update");
	QString strSlaveUpdate = pDN->toElement().attribute("slave_update");

	MainControlFrame* pMainControlFrame = MainControlFrame::GetMainControlFrameInstance();

	QDomNodeList childs = pDN->childNodes();
	QString strDockWidgetName;

	for(int j = 0; j < childs.size(); j++)
	{
		QDomNode childNode = childs.at(j);
		QString childName = childNode.nodeName();

		if(childName == QString::fromLocal8Bit("DockWidgetPlugins"))
		{
			QDomElement de = childNode.toElement();
			QString strmodule = de.attribute("module");
			QString strFunction = de.attribute("factory_function");
			QLibrary lib(strmodule);
			GetDockWidgetFactory pDockWidgetFactory = (GetDockWidgetFactory)lib
				.resolve(strFunction.toStdString().c_str());
			if(pDockWidgetFactory == 0) continue;
			DockWidgetFactory* pDWF = pDockWidgetFactory();
			m_DockWidgetFactory.insert(strNodeName, pDWF);
		}
		else if(childName == QString::fromLocal8Bit("DockName"))
		{
			//QDomNode childNode = childNode.toElement();
			QDomElement de = childNode.toElement();
			strDockWidgetName = de.attribute("name");

			DockWidgetFactory* pDWF = m_DockWidgetFactory.value(strNodeName);
			DockWidgetInterface* pDWI = pDWF->GetDockWidgetInterface(
				strDockWidgetName.toStdString());

			if(pDWI != 0)
			{
				this->addDockWidget(pDWI->GetDockArea(), pDWI->GetDockWidget());
				if(strMasterUpdate == "true")
				{
					UpdateMasterInterface* pUMI 
						= dynamic_cast<UpdateMasterInterface*>(pDWI);
					if(pUMI)
						m_UpdateMasterMap.insert(strNodeName, pUMI);
				}

				if(strSlaveUpdate == "true")
				{
					UpdateSlaveInterface* pUSI 
						= dynamic_cast<UpdateSlaveInterface*>(pDWI);
					m_UpdateSlaveMap.insert(strNodeName, pUSI);
				}
			}
		}
		else if(childName == QString::fromLocal8Bit("DockWidgetConfig"))
		{
			QDomElement configMainElement = childNode.toElement();
			QString strParentName = configMainElement.attribute("name");
			QDomNodeList configNodeList = childNode.childNodes();
			DockWidgetFactory* pDWF = m_DockWidgetFactory.value(strNodeName);
			DockWidgetInterface* pDWI = pDWF->GetDockWidgetInterface(
				strDockWidgetName.toStdString());

			ActionFactory* pActionFactoryInterface = 0;

			for(int k = 0; k < configNodeList.length(); k++)
			{
				QDomNode configChildNode = configNodeList.at(k);
				//QDomElement de = configNode.toElement();
				QString childName = configChildNode.nodeName();
				if(childName == QString::fromLocal8Bit("ControllerPlugins"))
				{
					QDomElement domElement =  configChildNode.toElement();
					QString strFunctionName = domElement.attribute(QString::fromLocal8Bit("factory_function"));
					QString strModuleName = domElement.attribute(QString::fromLocal8Bit("module"));

					pMainControlFrame->LoadControlManagerFromPlugin(
						strParentName.toStdString()
						, strModuleName.toStdString()
						, strFunctionName.toStdString());
				}
				else if(childName == QString::fromLocal8Bit("ActionFactory"))
				{
					QDomElement domElement =  configChildNode.toElement();
					QString strFunctionName = domElement.attribute(QString::fromLocal8Bit("factory_function"));
					QString strModuleName = domElement.attribute(QString::fromLocal8Bit("module"));

					QLibrary lib(strModuleName);
					if(lib.load())
					{
						GetActionFactory pActionFactory = (GetActionFactory)lib
							.resolve(strFunctionName.toStdString().c_str());
						pActionFactoryInterface = pActionFactory();
						m_ActionFactoryMap.insert(strParentName, pActionFactoryInterface);
					}
					
				}
				else if(childName == QString::fromLocal8Bit("Controller"))
				{
					QDomElement domElement =  configChildNode.toElement();
					QString ControllerName = domElement.attribute("name");
					ControllerManager* pCM = pMainControlFrame->GetControlerManager(strParentName.toStdString());
					
					if(pCM != 0)
					{
						Controller*  pC = pCM->GetControllerFromName(ControllerName.toStdString());
						pDWI->SetController(pC, configChildNode);
					}
				}
				else if(childName == QString::fromLocal8Bit("ToolsAction"))
				{
					QDomNodeList mapNodeList = configChildNode.childNodes();
					for(int l = 0; l < mapNodeList.length(); l++)
					{
						QDomElement mapElement = mapNodeList.at(l).toElement();
						QString strActionName = mapElement.attribute("name");
						MyActionInterface* pAction = pActionFactoryInterface->GetActionFromName(strActionName);
						pDWI->AddToolsBarAction(strActionName, pAction);
					}
				}
				else if(childName == QString::fromLocal8Bit("ContextMenuAction"))
				{
					QDomNodeList mapNodeList = configChildNode.childNodes();
					for(int l = 0; l < mapNodeList.length(); l++)
					{
						QDomElement mapElement = mapNodeList.at(l).toElement();
						QString strActionName = mapElement.attribute("name");
						MyActionInterface* pAction = pActionFactoryInterface->GetActionFromName(strActionName);
						pDWI->AddContextMenuAction(strActionName, pAction);
					}
				}
				else if(childName == QString::fromLocal8Bit("ActionToControllerMap"))
				{
					QDomNodeList mapNodeList = configChildNode.childNodes();

					for(int l = 0; l < mapNodeList.length(); l++)
					{
						QDomElement mapElement = mapNodeList.at(l).toElement();
					}
				}
				else
				{

				}
			}

		}
		else
		{
			
		}
	}
}

DockWidgetInterface* MainFrameWindow::GetDockWidget( const QString& strFactoryName, const QString& strDockWidgetName )
{
	DockWidgetFactory* pDWF = m_DockWidgetFactory.value(strFactoryName);
	if(pDWF != 0)
	{
		DockWidgetInterface* pDWI = pDWF->GetDockWidgetInterface(strDockWidgetName.toStdString());
		return pDWI;
	}
	else
		return 0;
}

void MainFrameWindow::LoadUpdateConfigure( QDomNode* pDN )
{
	QString strNodeName = pDN->nodeName();
	if(strNodeName == "UpdateEvent")
	{
		QDomNodeList nodeList = pDN->childNodes();
		for(int i = 0; i < nodeList.size(); i++)
		{
			QDomNode childNode = nodeList.at(i);
			QString strChildNodeName = childNode.nodeName();
			if(strChildNodeName == "DockWidget")
			{
				QDomElement domElement = childNode.toElement();
				QString strFactoryName = domElement.attribute("factory_name");
				QString strDockWidgetName = domElement.attribute("dock_name");
				DockWidgetInterface* pDWI = GetDockWidget(strFactoryName, strDockWidgetName);

				m_UpdateDockVector.push_back(pDWI);
			}
			else
			{
				return;
			}
		}
	}
	
}

void MainFrameWindow::LoadUpdateMap( QDomNode* pDN )
{
	QString strNodeName = pDN->nodeName();
	if(strNodeName == "UpdateMap")
	{
		QDomNodeList nodeList = pDN->childNodes();
		for(int i = 0; i < nodeList.size(); i++)
		{
			QDomNode childNode = nodeList.at(i);
			if(childNode.nodeName() == "MapGroup")
			{
				QDomNodeList childMapNodeList = childNode.childNodes();
				for(int ii = 0; ii < childMapNodeList.size(); ii++)
				{
					QDomNode childMapNode = childMapNodeList.at(ii);
					QString strChildNodeName = childMapNode.nodeName();
					
					if(strChildNodeName == "Master")
					{
						QDomElement materElement = childMapNode.toElement();
						QString strMasterName = materElement.attribute("name");
						UpdateMasterInterface* pUMI = 0;

						if(strMasterName == "class.MainFrameWindow")
						{
							pUMI = this;
						}
						else
						{
							pUMI = m_UpdateMasterMap.value(strMasterName);
						}

						QDomNodeList slaveNodeList = childMapNode.childNodes();
						for(int iii = 0; iii < slaveNodeList.size(); iii++)
						{
							QDomNode slaveNode = slaveNodeList.at(iii);
							QDomElement domElement = slaveNode.toElement();
							QString strSlaveName = domElement.attribute("name");
							UpdateSlaveInterface* pUSI = m_UpdateSlaveMap.value(strSlaveName);
							if(pUMI)
								pUMI->AddSlave(pUSI);
						}
					}
					else
					{
						continue;
					}
				}
			}
		}
	}
}

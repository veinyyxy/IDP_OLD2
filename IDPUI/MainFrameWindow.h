#pragma once

#include "idpui_global.h"
#include <osgViewer/Viewer>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtXml/QtXml>
#include <QtGui/QActionGroup>
#include "Map.h"
#include "UpdateMasterInterface.h"

class ToolBoxInterface;
class ToolBoxesFactory;
class ActionFactory;
class ControllerManager;
class DisplayViewFactory;
class ControllerFrameContext;
class WindowFrameContext;
class DockWidgetFactory;
class DockWidgetInterface;

class IDPUI_EXPORT MainFrameWindow :
	public QMainWindow, UpdateMasterInterface
{
	Q_OBJECT
public:
	typedef ToolBoxesFactory* (*GetToolBoxesFactory)();
	typedef ActionFactory* (*GetActionFactory)();
	typedef DisplayViewFactory* (*GetDisplayViewFactory)();
	typedef DockWidgetFactory* (*GetDockWidgetFactory)();
	~MainFrameWindow(void);

	int LoadDockWidgetControlPlugins();
	int LoadDockWidgetPlugins();
	int LoadToolBoxPlugins(QString strConfigFile);
	int LoadDisplyAreaPlugins();

	static MainFrameWindow* GetUIMainFramInstance();
	DisplayViewFactory* GetDisplayViewFactoryFromName(const QString& strName);

	QMdiSubWindow* AddSubWindow(QWidget* pSub);
	QMdiSubWindow* GetCurrentWindow();
	goto_gis::Map* GetCurrentMap();
	osgViewer::Viewer* GetCurrentViewer();
	DockWidgetInterface* GetDockWidget(const QString& strFactoryName, const QString& strDockWidgetName);
	inline WindowFrameContext* GetWindowFrameContext(){
		return m_pWindowFrameContext;
	}
	inline QMdiArea* GetMdiArea(){return m_pQMdiArea;}
private:
	void LoadDockWidget(QDomNode* pDN);
	void LoadUpdateConfigure(QDomNode* pDN);
	void LoadUpdateMap(QDomNode* pDN);
	static MainFrameWindow* s_pMainFrameWindow;
	MainFrameWindow(void);
	void operator=(const MainFrameWindow&);
	QTabWidget* m_pToolTabs;
	QMap<QString, ToolBoxesFactory*> m_ToolBoxFactoryMap;
	QMap<QString, ActionFactory*> m_ActionFactoryMap;
	QMap<QString, DisplayViewFactory*> m_DisplayViewFactory;
	QMap<QString, DockWidgetFactory*> m_DockWidgetFactory;
	//QMap<QString, ControllerManager*> m_ControllerManagerMap;
	QActionGroup* m_pActionGroup;
	QMdiArea* m_pQMdiArea;
	QMdiSubWindow* m_CurrentSubWindow;
	ControllerFrameContext* m_pControllerFrameContext;
	WindowFrameContext* m_pWindowFrameContext;
	QVector<DockWidgetInterface*> m_UpdateDockVector;
	QMap<QString, UpdateMasterInterface*> m_UpdateMasterMap;
	QMap<QString, UpdateSlaveInterface*> m_UpdateSlaveMap;
	//goto_gis::Map* Get
	//osgViewer::Viewer*
public Q_SLOTS:
	void slot_ActionTriggered( QAction* action );
	void slot_subWindowActivated ( QMdiSubWindow * window );
};

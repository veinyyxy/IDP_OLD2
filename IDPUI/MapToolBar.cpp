#include "mainwindow.h"
#include "sysmainwindow.h"

#include "MapDataAccess.h"

#include "MapToolBar.h"


MapToolBar::MapToolBar(QWidget *parent): QToolBar(parent),m_DefaultActionGroup(0)
{
	ui.setupUi(this);
	m_pMW = SysMainWindow::GetMainWindow();

	foreach(QAction* action,this->actions())
	{
		m_DefaultActionGroup.addAction(action);
	}
	InitSlots();
}

MapToolBar::~MapToolBar()
{

}
void MapToolBar::InitSlots()
{
	connect(&m_DefaultActionGroup, SIGNAL(triggered(QAction*)),this,  SLOT(Slots_ActionTriggered(QAction*)));
}

void MapToolBar::Slots_ActionTriggered( QAction* action )
{
	try 
	{
		QWidget* viewerWindow = m_pMW->FromWidgetToQGLW();
		if (viewerWindow == NULL)
		{
			return;
		}
		//////////////////////////////////////////////////////////////////////////
		if (action == ui.actionMZoomout)
		{
			MapDataAccess::Instance()->MapMZoomOut(viewerWindow);
		}
		else if (action == ui.actionMZoomIn)
		{
			MapDataAccess::Instance()->MapMZoomIn(viewerWindow);
		}
		else if (action == ui.actionLeft)
		{
			MapDataAccess::Instance()->MapLeft(viewerWindow);
		}
		else if (action == ui.actionRight)
		{
			MapDataAccess::Instance()->MapRight(viewerWindow);
		}
		else if (action == ui.actionTop)
		{
			MapDataAccess::Instance()->MapTop(viewerWindow);
		}
		else if (action == ui.actionBottom)
		{
			MapDataAccess::Instance()->MapBottom(viewerWindow);
		}
		else if (action == ui.actionCenter)
		{
			MapDataAccess::Instance()->MapALLMap(viewerWindow);
		}
	}
	catch (...)
	{

	}
}
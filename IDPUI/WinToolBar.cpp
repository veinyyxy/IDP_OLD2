#include "mainwindow.h"
#include "sysmainwindow.h"


#include "WinToolBar.h"

//class tessellateDemoGeometry : public osg::Geometry, public osgUtil::Tessellator {
//	// We add the Tessellator to the geometry because we want to access the
//	// tessellatable contours again;  you can apply a Tessellator to a Geometry 
//	// to produce exactly a required tessellation once only, and then 
//	// the contours could be discarded since the geometry does not need to be retessellated.
//public:
//	tessellateDemoGeometry() {};
//
//protected:
//	virtual ~tessellateDemoGeometry() {};
//};

WinToolBar::WinToolBar(QWidget *parent): QToolBar(parent),m_WinActionGroup(0)
{
	ui.setupUi(this);

	foreach(QAction* action,this->actions())
	{
		m_WinActionGroup.addAction(action);
	}
	InitSlots();
}

WinToolBar::~WinToolBar()
{

}

void WinToolBar::InitSlots()
{
	connect(&m_WinActionGroup, SIGNAL(triggered(QAction*)),this,  SLOT(Slots_ActionTriggered(QAction*)));
}

void WinToolBar::Slots_ActionTriggered( QAction* action )
{
	double dr = 0;
	MainWindow* mw = SysMainWindow::GetMainWindow();
	try 
	{
		QWidget* bw  = mw->FromWidgetToQGLW();
		if (bw == 0)
		{
			return;
		}
		if (action == ui.actionFullScreen)
		{
				mw->Slot_FullScreen();
		}
		else if (action == ui.actionCascad)
		{
			QMdiArea* mdiArea = dynamic_cast<QMdiArea*>(mw->centralWidget());
			if (mdiArea)
			{
				foreach(QMdiSubWindow* subWin , mdiArea->subWindowList())
				{
					subWin->showNormal();
				}
				mdiArea->cascadeSubWindows();
			}
		}
		else if (action == ui.actionTile)
		{
			QMdiArea* mdiArea = dynamic_cast<QMdiArea*>(mw->centralWidget());
			if (mdiArea)
			{
				foreach(QMdiSubWindow* subWin , mdiArea->subWindowList())
				{
					subWin->showNormal();
				}
				mdiArea->tileSubWindows();
			}
			
		}
		else if (action == ui.actionClose)
		{
			QMdiArea* mdiArea = dynamic_cast<QMdiArea*>(mw->centralWidget());
			if (mdiArea)
			{
				mdiArea->closeActiveSubWindow();
			}
			
		}
		else if (action == ui.actionCloseAll)
		{
			QMdiArea* mdiArea = dynamic_cast<QMdiArea*>(mw->centralWidget());
			if (mdiArea)
			{
				mdiArea->closeAllSubWindows();
			}
		}
	}
	catch (...)
	{
	}
}


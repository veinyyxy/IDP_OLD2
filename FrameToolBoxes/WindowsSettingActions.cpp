#include "stdafx.h"
#include <QtGui/QtGui>
#include "MainFrameWindow.h"
#include "WindowsSettingActions.h"
#include "ConfigDialog.h"
ADD_MYACTION_CONTSTRUCTED_DECLARATION(FloatingWindowsAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DockingWindowsAction)

void FloatingWindowsAction::MyAction()
{
	QMdiSubWindow *currentSubW = m_pMdiArea->activeSubWindow();

	if (currentSubW)
	{
		QWidget* currentW = currentSubW->widget();

		if (currentW)
		{
			currentSubW->layout()->removeWidget(currentW);
			currentSubW->setWidget(0);
			currentSubW->hide();


			currentW->setParent(m_pMdiArea);
			currentW->setWindowFlags(Qt::Window);
			currentW->showMaximized();
		}
	}
}

void FloatingWindowsAction::InitMyAction()
{
	m_pMdiArea = MainFrameWindow::GetUIMainFramInstance()->GetMdiArea();
}

FloatingWindowsAction::~FloatingWindowsAction( void )
{

}

void FloatingWindowsAction::SetController( Controller* pController )
{
	//throw std::exception("The method or operation is not implemented.");
}

//////////////////////////////////////////////////////////////////////////
void DockingWindowsAction::MyAction()
{
	
}

void DockingWindowsAction::InitMyAction()
{
	m_pMdiArea = MainFrameWindow::GetUIMainFramInstance()->GetMdiArea();
}

DockingWindowsAction::~DockingWindowsAction( void )
{

}

void DockingWindowsAction::SetController( Controller* pController )
{
	//throw std::exception("The method or operation is not implemented.");
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(OverlapAction)
	void OverlapAction::MyAction()
{
	QMdiArea* mdiArea = m_pMdiArea;
	if (mdiArea)
	{
		foreach(QMdiSubWindow* subWin , mdiArea->subWindowList())
		{
			subWin->showNormal();
		}
		mdiArea->cascadeSubWindows();
	}
}

void OverlapAction::InitMyAction()
{
	m_pMdiArea = MainFrameWindow::GetUIMainFramInstance()->GetMdiArea();
}

void OverlapAction::SetController( Controller* pController )
{
	//throw std::exception("The method or operation is not implemented.");
}


ADD_MYACTION_CONTSTRUCTED_DECLARATION(TileAction)
	void TileAction::MyAction()
{
	
	if (m_pMdiArea)
	{
		foreach(QMdiSubWindow* subWin , m_pMdiArea->subWindowList())
		{
			subWin->showNormal();
		}
		m_pMdiArea->tileSubWindows();
	}
}

void TileAction::InitMyAction()
{
	m_pMdiArea = MainFrameWindow::GetUIMainFramInstance()->GetMdiArea();
}

void TileAction::SetController( Controller* pController )
{
	//throw std::exception("The method or operation is not implemented.");
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(CloseAllAction)
	void CloseAllAction::MyAction()
{
	m_pMdiArea->closeAllSubWindows();
}

void CloseAllAction::InitMyAction()
{
	m_pMdiArea = MainFrameWindow::GetUIMainFramInstance()->GetMdiArea();
}

void CloseAllAction::SetController( Controller* pController )
{
	//throw std::exception("The method or operation is not implemented.");
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(SystemSettingAction)
	void SystemSettingAction::MyAction()
{
	ConfigDialog* m_pConfigDialog = new ConfigDialog(MainFrameWindow::GetUIMainFramInstance());
	m_pConfigDialog->exec();
}

void SystemSettingAction::InitMyAction()
{
	m_pMdiArea = MainFrameWindow::GetUIMainFramInstance()->GetMdiArea();
}

void SystemSettingAction::SetController( Controller* pController )
{
	
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(FullScreenWindowAction)
	void FullScreenWindowAction::MyAction()
{
	QMdiSubWindow* currentSubW = m_pMdiArea->activeSubWindow();
	if(!currentSubW) return;
	QWidget *currentW = currentSubW->widget();
	if (currentW)
	{
		currentSubW->layout()->removeWidget(currentW);
		currentSubW->setWidget(0);
		currentSubW->hide();


		currentW->setParent(MainFrameWindow::GetUIMainFramInstance());
		currentW->setWindowFlags(Qt::Window);
		currentW->showFullScreen();
		currentW->setProperty("FullScreen",true);
	}

}

void FullScreenWindowAction::InitMyAction()
{
	m_pMdiArea = MainFrameWindow::GetUIMainFramInstance()->GetMdiArea();
}

void FullScreenWindowAction::SetController( Controller* pController )
{
	
}
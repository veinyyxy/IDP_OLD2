#include <QtGui/QtGui>

#include "WindowsSettingActions.h"

ADD_MYACTION_CONTSTRUCTED_DECLARATION(FloatingWindowsAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DockingWindowsAction)

extern QWidget* GetActiveWindow(QMdiArea* m_pMdiArea);

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
	m_pMdiArea = 0;
}

FloatingWindowsAction::~FloatingWindowsAction( void )
{

}
//////////////////////////////////////////////////////////////////////////
void DockingWindowsAction::MyAction()
{
	
}

void DockingWindowsAction::InitMyAction()
{
	m_pMdiArea = 0;
}

DockingWindowsAction::~DockingWindowsAction( void )
{

}

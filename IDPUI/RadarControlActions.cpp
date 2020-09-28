#include <QtGui/QtGui>

#include "RadarControlActions.h"
#include "RadarDataAccess.h"
#include "AreaDialog.h"

ADD_MYACTION_CONTSTRUCTED_DECLARATION(FourUnitScreenAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(ContourPlanesAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(SectionPlansAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(AreaStatisticsAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(VolumeRenderAction)

void FourUnitScreenAction::MyAction()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QWidget* win4 = m_pRadarDataAccess->Win4Viewer(m_pMap);
	if (win4)
	{
		connect(win4, SIGNAL(SetCurrentWidget(QWidget*)),this,  SLOT(Slot_ActivateWindow(QWidget*)));
		m_pMainWindow->CreateMDISubWindow(win4);
		win4->showMaximized();
	}
	QApplication::restoreOverrideCursor();
}

void FourUnitScreenAction::InitMyAction()
{
	
}

FourUnitScreenAction::~FourUnitScreenAction( void )
{

}
//////////////////////////////////////////////////////////////////////////
void ContourPlanesAction::MyAction()
{
	
}

void ContourPlanesAction::InitMyAction()
{

}

ContourPlanesAction::~ContourPlanesAction( void )
{

}
//////////////////////////////////////////////////////////////////////////
void SectionPlansAction::MyAction()
{
	m_pRadarDataAccess->PlotWidget(m_pViewer);
}

void SectionPlansAction::InitMyAction()
{
	m_pRadarDataAccess = 0;
	m_pViewer = 0;
}

SectionPlansAction::~SectionPlansAction( void )
{

}
//////////////////////////////////////////////////////////////////////////
void AreaStatisticsAction::MyAction()
{
	m_pRadarDataAccess->AreaWidget(m_pMap);
	AreaDialog* pAreaDialog = new AreaDialog(m_pMap, 0);
	pAreaDialog->show();
}

void AreaStatisticsAction::InitMyAction()
{
	m_pMap = 0;
	m_pRadarDataAccess = 0;
	m_pViewer = 0;
}

AreaStatisticsAction::~AreaStatisticsAction( void )
{

}
//////////////////////////////////////////////////////////////////////////
void VolumeRenderAction::MyAction()
{
	m_pRadarDataAccess->VolumeRender(m_pMap);
}

void VolumeRenderAction::InitMyAction()
{
	m_pMap = 0;
	m_pRadarDataAccess = 0;
}

VolumeRenderAction::~VolumeRenderAction( void )
{

}
//////////////////////////////////////////////////////////////////////////
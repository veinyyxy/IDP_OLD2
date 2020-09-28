#include "StdAfx.h"
#include "OpenDataAction.h"
#include "DisplayViewFactory.h"
#include "MainFrameWindow.h"
#include "Map.h"
#include "FrameModel.h"
#include <osgViewer/Viewer>

ADD_MYACTION_CONTSTRUCTED_DECLARATION(OpenDataAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(CloseSystemAction)

void OpenDataAction::MyAction()
{
	std::map<std::string, boost::any> outParam;
	m_pController->Excute();
	//m_pController->SetOutput(&outParam);
	FrameModel* pFM = m_pController->Output();
	boost::any* pAny = pFM->GetParameter("osg::View");
	
	osgViewer::View* pView = 0;
	goto_gis::Map* pMap = 0;

	MainFrameWindow* pMainFrameWindow = MainFrameWindow::GetUIMainFramInstance();

	QWidget* pWidget = 0;
	
	ULONGLONG address = boost::any_cast<ULONGLONG>(*pAny);
	pView = (osgViewer::View*)(address);

	
	pAny = pFM->GetParameter("goto_gis::Map");
	address = boost::any_cast<ULONGLONG>(*pAny);

	pMap = (goto_gis::Map*)(address);

	//QMdiArea* pMdiArea = new QMdiArea;
	//pMdiArea->addSubWindow(pWidget);
	
	pWidget = m_pDisplayViewFactory->GetDisplayView(pView, 0);

	pWidget->setProperty("goto_gis::Map", (ULONGLONG)pMap);
	//pMainFrameWindow->setCentralWidget(pWidget);
	QMdiSubWindow* pSub = pMainFrameWindow->AddSubWindow(pWidget);
	//pView->setSceneData(pMap->GetRootNode());
	pSub->showMaximized();
	m_pController->PostExcute();
}

void OpenDataAction::InitMyAction()
{
	
}

void OpenDataAction::SetController( Controller* pController )
{
	m_pController = pController;
}

//////////////////////////////////////////////////////////////////////////
void CloseSystemAction::MyAction()
{
	MainFrameWindow* pMainFrameWindow = MainFrameWindow::GetUIMainFramInstance();
	pMainFrameWindow->close();
}

void CloseSystemAction::InitMyAction()
{

}

void CloseSystemAction::SetController( Controller* pController )
{
	
}

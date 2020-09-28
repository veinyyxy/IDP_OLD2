#include "StdAfx.h"
#include "ParamSwapModel.h"
#include "OpenDisplayWindowController.h"
#include "MainControlFrame.h"
#include "CoordinateTransform.h"


OpenDisplayWindowController::OpenDisplayWindowController(void)
{
	m_MyName = "OpenDisplayWindow";
	m_pFrameModel = new ParamSwapModel;
}


OpenDisplayWindowController::~OpenDisplayWindowController(void)
{
}

int OpenDisplayWindowController::Excute()
{
	MainControlFrame* pMainControlFrame 
		= MainControlFrame::GetMainControlFrameInstance();
	goto_gis::Map* pMap = pMainControlFrame->CreateMapFromFactory(0);
	osg::StateSet* pStateSet = pMap->GetRootNode()->getOrCreateStateSet();
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
	pStateSet->setMode(GL_POLYGON_SMOOTH, osg::StateAttribute::ON);
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

	osgViewer::View* pView = pMainControlFrame->CreateOsgView(pMap);
	//osg::DisplaySettings::instance()->setNumMultiSamples(16);
	pMap->SetAttachView((osgViewer::View*)pView);
	osg::Camera* pC = pView->getCamera();

	QString strMapConfig = QCoreApplication::applicationDirPath()
		+ QString::fromLocal8Bit("/Config/Default.prj");
	pMap->Load2DMapFromConfig(strMapConfig.toStdString());
	
	boost::any pAny;
	pAny = (ULONGLONG)(pView);
	m_pFrameModel->SetParameter("osg::View", &pAny);
	pAny = (ULONGLONG)(pMap);
	m_pFrameModel->SetParameter("goto_gis::Map", &pAny);

	m_pMap = pMap;
	//pMainControlFrame->CreateOsgView();
	return 1;
}

void OpenDisplayWindowController::GetName(std::string& strID )
{
	strID = m_MyName;
}

void OpenDisplayWindowController::Input( const  FrameModel* pFM )
{
	
}

FrameModel* OpenDisplayWindowController::Output()
{
	return m_pFrameModel;
}

int OpenDisplayWindowController::PreExcute()
{
	return 0;
}

int OpenDisplayWindowController::PostExcute()
{
	goto_gis::Map* pMap = (goto_gis::Map*)m_pMap;
	pMap->CreateMapData();

	//pMap->GetAttachView()->setCameraManipulator(new osgGA::TrackballManipulator);
	pMap->GetAttachView()->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1));
	/*osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(pMap->GetAttachView());
	pViewer->run();*/
	pMap->ScaleToLayer();
	return 1;
}

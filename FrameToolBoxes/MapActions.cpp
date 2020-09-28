#include "StdAfx.h"
#include "GridSetting.h"
#include "MapActions.h"
#include "DisplaySettingsDialog.h"
#include "MainFrameWindow.h"
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DisplaySettingsAction)

void DisplaySettingsAction::MyAction()
{
	MainFrameWindow* pMFW = MainFrameWindow::GetUIMainFramInstance();
	DisplaySettingsDialog* pDisplaySettingsDialog = new DisplaySettingsDialog(pMFW);
	pDisplaySettingsDialog->exec();
}

void DisplaySettingsAction::InitMyAction()
{
	
}

void DisplaySettingsAction::SetController( Controller* pController )
{
	
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(MapSettingsAction)
void MapSettingsAction::MyAction()
{
	MainFrameWindow* pMFW = MainFrameWindow::GetUIMainFramInstance();
	DisplaySettingsDialog* pDisplaySettingsDialog = new DisplaySettingsDialog(pMFW);
	pDisplaySettingsDialog->setModal(true);
	pDisplaySettingsDialog->showNormal();
	pDisplaySettingsDialog->StackedWidgetCurrentIndex(1);
}

void MapSettingsAction::InitMyAction()
{
	
}

void MapSettingsAction::SetController( Controller* pController )
{
	
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(AddLatLonNetAction)
void AddLatLonNetAction::MyAction()
{
	MainFrameWindow* pMFW = MainFrameWindow::GetUIMainFramInstance();
	GridSetting* m_pGridAddDlg = new GridSetting(pMFW);
	m_pGridAddDlg->exec();
}

void AddLatLonNetAction::InitMyAction()
{
	
}

void AddLatLonNetAction::SetController( Controller* pController )
{
	
}

#include <osgDB/WriteFile>
class SnapImageDrawCallback : public osg::Camera::DrawCallback 
{ 
public:
	SnapImageDrawCallback() 
	{ 
		_snapImageOnNextFrame = false; 
	}

	void setFileName(const std::string& filename) { _filename = filename; } 
	const std::string& getFileName() const { return _filename; }
	void setSnapImageOnNextFrame(bool flag) { _snapImageOnNextFrame = flag; } 
	bool getSnapImageOnNextFrame() const { return _snapImageOnNextFrame; }
	virtual void operator () (const ::osg::Camera& camera) const 
	{
		if (!_snapImageOnNextFrame) return;
		int x,y,width,height;
		x = camera.getViewport()->x();
		y = camera.getViewport()->y();
		width = camera.getViewport()->width();
		height = camera.getViewport()->height();
		::osg::ref_ptr< ::osg::Image> image = new ::osg::Image();
		image->readPixels(x,y,width,height,GL_RGB,GL_UNSIGNED_BYTE);
		osgDB::writeImageFile(*image,_filename);
		_snapImageOnNextFrame = false;
	}
protected:
	::std::string _filename; 
	mutable bool _snapImageOnNextFrame;

};

ADD_MYACTION_CONTSTRUCTED_DECLARATION(CenterProjectionAction)
	void CenterProjectionAction::MyAction()
{
	ProjCenterSetting* m_pProjCenterDlg = new ProjCenterSetting();
	/*ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(FromWidgetToQGLW());
	if (viewerWindow)
	{
		SnapImageDrawCallback* pImageDrawCallback = new SnapImageDrawCallback;
		pImageDrawCallback->setFileName((IDPApplication::TempPath()+"Thematic.png").toStdString());
		viewerWindow->getViewer()->getCamera()->setFinalDrawCallback(pImageDrawCallback);
		m_pCommonToolBar->CreateImage();
	}*/
	m_pProjCenterDlg->exec();
}

void CenterProjectionAction::InitMyAction()
{
	
}

void CenterProjectionAction::SetController( Controller* pController )
{
	
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(ProjectionSettingAction)
	void ProjectionSettingAction::MyAction()
{

}

void ProjectionSettingAction::InitMyAction()
{
	
}

void ProjectionSettingAction::SetController( Controller* pController )
{
	
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(MakeProfessionalChartAction)
	void MakeProfessionalChartAction::MyAction()
{

}

void MakeProfessionalChartAction::InitMyAction()
{
	
}

void MakeProfessionalChartAction::SetController( Controller* pController )
{
	
}


ADD_MYACTION_CONTSTRUCTED_DECLARATION(MakeProfessionalReportAction)
	void MakeProfessionalReportAction::MyAction()
{

}

void MakeProfessionalReportAction::InitMyAction()
{
	
}

void MakeProfessionalReportAction::SetController( Controller* pController )
{
	
}
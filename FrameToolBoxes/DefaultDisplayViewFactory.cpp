#include "StdAfx.h"
#include "DefaultDisplayViewFactory.h"
#include "AdapterWidget.h"
#include "MainFrameWindow.h"

DefaultDisplayViewFactory::DefaultDisplayViewFactory(void)
{
}


DefaultDisplayViewFactory::~DefaultDisplayViewFactory(void)
{
}

QWidget* DefaultDisplayViewFactory::GetDisplayView( osg::View* pView, osg::Camera* pCamera )
{
	ViewerQT* pViewer = new ViewerQT((osgViewer::Viewer*)pView, MainFrameWindow::GetUIMainFramInstance());
	return pViewer;
}

int DefaultDisplayViewFactory::GetDisplayViewType( QWidget* pWidget )
{
	return 0;
}

osg::View* DefaultDisplayViewFactory::GetWidgetsView()
{
	return 0;
}

osg::Camera* DefaultDisplayViewFactory::GetWidgetsCamera()
{
	return 0;
}

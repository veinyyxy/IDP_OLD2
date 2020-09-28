#include "GraphicsControlerFactory.h"

GraphicsControlerFactory* GraphicsControlerFactory::g_GraphicsControlerFactory = 0;

GraphicsControlerFactory::GraphicsControlerFactory(void)
{

}


GraphicsControlerFactory::~GraphicsControlerFactory(void)
{
}

GraphicsControlerFactory* GraphicsControlerFactory::Instance()
{
	if(!g_GraphicsControlerFactory)
	{
		g_GraphicsControlerFactory = new GraphicsControlerFactory();
	}
	return g_GraphicsControlerFactory;
}

ViewerQT* GraphicsControlerFactory::CreateViewerQT(osgViewer::Viewer* pVbn /*= 0*/, QWidget * parent /*= 0*/, QGLWidgetEventCallback* pEventCallback /*= 0*/ )
{
	ViewerQT* viewerWindow = new ViewerQT(pVbn,parent,pEventCallback);
	viewerWindow->setProperty("Map",0); //初始化Map属性为空
	return viewerWindow;
}

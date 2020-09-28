#pragma once

#include "AdapterWidget.h"
#include "GraphicsControler_global.h"

class GRAPHICSCONTROLER_EXPORT GraphicsControlerFactory
{
public:
	virtual ~GraphicsControlerFactory(void);
	static GraphicsControlerFactory* Instance();
	ViewerQT* CreateViewerQT(osgViewer::Viewer* pVbn = 0, QWidget * parent = 0, QGLWidgetEventCallback* pEventCallback = 0);
private:
	GraphicsControlerFactory(void);
	GraphicsControlerFactory(GraphicsControlerFactory&){}
	void operator=(GraphicsControlerFactory&){}

	//////////////////////////////////////////////////////////////////////////
	static GraphicsControlerFactory* g_GraphicsControlerFactory;
};


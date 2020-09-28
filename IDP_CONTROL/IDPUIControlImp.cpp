#include <QtCore/QMap>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtOpenGL/QGLWidget>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QtGui>
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osgWidget/WindowManager>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/ViewerEventHandlers>
#include <osg/Notify>
#include <osgWidget/Util>
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgGA/TrackballManipulator>

#include "RadarVectorLayer.h"
#include "OsgEarthBridge.h"
#include "ViewManager.h"
#include "AdapterWidget.h"
#include "DataProvider.h"
#include "Map.h"
#include "AnimaticFrame.h"
#include "AnimaticControlerInterface.h"
#include "GISMapManager.h"
#include "OsgEarthTransform.h"
#include "AnimaticDataLoader.h"
#include "ViewManager.h"
#include "LayerTreeControler.h"
#include "RadarDataAccess.h"
#include "AnimaticControler.h"
#include "IDPUIControlImp.h"


IDPUIControlImp::IDPUIControlImp(void) : m_pGLWidget(0), m_pOsgViewer(0), m_AnimaticControler(0)
{
	m_RaderDataAccess= RadarDataAccess::Instance(&m_ViewManager);
}

IDPUIControlImp::~IDPUIControlImp(void)
{
}

int IDPUIControlImp::InitIDPUIControler()
{
	int iI = -1;
	return 0;
	
}

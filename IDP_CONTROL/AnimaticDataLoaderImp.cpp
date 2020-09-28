#include <QtCore/QMap>
#include <QtCore/QStringList>
#include <QtCore/QRunnable>
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
#include <osgEarth/SpatialReference>
#include <osgEarthUtil/EarthManipulator>
#include <osgGA/TrackballManipulator>
#include <osg/Vec3>

#include "Map.h"
#include "RadarDataAccess.h"
#include "RadarVectorLayer.h"
#include "ViewManager.h"
#include "AdapterWidget.h"
#include "DataProvider.h"
#include "AnimaticFrame.h"
#include "Radar3DFrameImp.h"
#include "AnimaticControlerInterface.h"
#include "OsgEarthTransform.h"
#include "RadarAnimaticImp.h"
#include "AnimaticDataLoader.h"
#include "AnimaticDataLoaderImp.h"


using namespace goto_gis;

class LoadFileThread : public QRunnable
{
public:
	virtual void run()
	{
		goto_gis::Map* pAnimaticMap = new goto_gis::Map(0);
		m_pRaderDataAccess->LoadRadarDataThreadSafe(m_fileName, pAnimaticMap);
		//pProDialog->setValue(fileNum + 1);

		Radar3DFrameImp* pAF = new Radar3DFrameImp;
		
		pAF->setAnimaticSwitch(m_pAnimaticSwith);
		pAF->setFrameID(fileNum);
		m_pInsertAnimaticBufferMutex->lock();
		m_pAnimaticSwith->addChild(pAnimaticMap->GetRootNode(), true);
		animaticBuffer->insert(fileNum, pAF);
		m_pInsertAnimaticBufferMutex->unlock();
	}
	QProgressDialog* pProDialog;
	QMutex* ProDialogMutex;
	int fileNum;
	QString m_fileName;
	RadarAnimaticImp::P_FRAME_BUFFER animaticBuffer;
	osg::Switch* m_pAnimaticSwith;
	RadarDataAccess* m_pRaderDataAccess;
	QMutex* m_pInsertAnimaticBufferMutex;
};

AnimaticDataLoaderImp::AnimaticDataLoaderImp( osgViewer::View* pVM ) : m_pPlayWindow(pVM), m_pWidget(0), m_pLoadFileCallback(0)
{
	m_pRaderDataAccess = RadarDataAccess::Instance();
}

AnimaticDataLoaderImp::~AnimaticDataLoaderImp(void)
{
}

AnimaticControlerInterface* AnimaticDataLoaderImp::LoadAnimaticFromFileList( const QStringList& fineList )
{
	osg::ref_ptr<osg::Switch> pSwitch = new osg::Switch;

	RadarAnimaticImp* pRadarAnimatic = new RadarAnimaticImp;
	RadarAnimaticImp::FRAME_BUFFER animaticBuffer;
	//osg::ref_ptr<osg::Switch> pAnimaticSwitchRoot = new osg::Switch;
	osg::Group* pGroup = 0;
#if 0	
	m_pRaderDataAccess->SetViewManager(m_pViewManager);
	ViewerQT* viewerWindow =dynamic_cast<ViewerQT*>(m_pViewManager->CreateViewport(0));
	goto_gis::Map* m_pBasicMap = m_pViewManager->GetMap2D();
	goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();
	
	pMap->SetCoordinateTransform(m_pBasicMap->GetCoordinateTransform());
	*pMap =  *m_pBasicMap;

	osg::ref_ptr<osg::Group> rootNode = pMap->GetRootNode();

	viewerWindow->getViewer()->setSceneData(rootNode); //加载场景
	viewerWindow->getViewer()->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	viewerWindow->getViewer()->realize();
	viewerWindow->getViewer()->addEventHandler(new osgGA::StateSetManipulator(rootNode->getOrCreateStateSet()));
	viewerWindow->getViewer()->addEventHandler(new osgViewer::StatsHandler);
	viewerWindow->getViewer()->addEventHandler(new osgViewer::WindowSizeHandler);
	viewerWindow->getViewer()->addEventHandler(new osgGA::GUIEventHandler);

	goto_gis::Map* _pMap = new goto_gis::Map(true);
	_pMap->SetCoordinateTransform(pMap->GetCoordinateTransform());
#endif
	//////////////////////////////////////////////////////////////////////////
	QProgressDialog progress("正在加载数据...", "撤销", 0, fineList.size(), 0);
	progress.setWindowModality(Qt::WindowModal);
	progress.show();
	//////////////////////////////////////////////////////////////////////////
#if 1
	for(int fileNum = 0; fileNum < fineList.size(); fileNum++)
	{
		if (progress.wasCanceled())
			break;

		goto_gis::Map* pAnimaticMap = new goto_gis::Map(0);
		m_pRaderDataAccess->LoadRadarData(fineList.at(fileNum), pAnimaticMap);
		progress.setValue(fileNum + 1);
		
		Radar3DFrameImp* pAF = new Radar3DFrameImp;
		pSwitch->addChild(pAnimaticMap->GetRootNode(), true);
		pAF->setAnimaticSwitch(pSwitch);
		pAF->setFrameID(fileNum);
		animaticBuffer.insert(fileNum, pAF);

		//if(m_pLoadFileCallback)
			//m_pLoadFileCallback->LoadingFile(fineList.at(fileNum), fileNum);

		pAnimaticMap->CreateMapData();
		pAnimaticMap->ScaleToLayer();
	}
#else
	int iThreadCount = fineList.size();
	QMutex mutexAnimaticBuff;
	LoadFileThread* pALLLFT = new LoadFileThread[iThreadCount];
	

	QThreadPool* pThreadPool = QThreadPool::globalInstance();
	pThreadPool->setMaxThreadCount(16);


	for(int fileNum = 0; fileNum < fineList.size(); fileNum++)
	{
		LoadFileThread* pLFT = pALLLFT + fileNum;
		pLFT->setAutoDelete(false);
		pLFT->m_pInsertAnimaticBufferMutex = &mutexAnimaticBuff;
		//pLFT->ProDialogMutex = &mutex;
		//pLFT->pProDialog = &progress;
		pLFT->animaticBuffer = &animaticBuffer;
		//pLFT->fileNum = fileNum;
		pLFT->m_fileName = fineList.at(fileNum);
		pLFT->m_pAnimaticSwith = pSwitch;
		
		pThreadPool->start(pLFT);

	}

	pThreadPool->waitForDone();
	//delete pALLLFT;
#endif
	progress.setValue(fineList.size());
	pRadarAnimatic->SetFrameBuffer(&animaticBuffer);
	
	m_pPlayWindow->setSceneData(pSwitch);
	m_pPlayWindow->setCameraManipulator(new DefaultManipulator);
	m_pPlayWindow->getCameraManipulator()->computeHomePosition();
	m_pPlayWindow->getCameraManipulator()->home(0);
	return pRadarAnimatic;
}

AnimaticControlerInterface* AnimaticDataLoaderImp::LoadAnimaticFromFile( const QString& fileName )
{
	return 0;
}

void* AnimaticDataLoaderImp::GetResult()
{
	return m_pWidget;
}

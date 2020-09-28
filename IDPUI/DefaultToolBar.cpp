#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/Notify>
#include <osgDB/WriteFile>
#include <osgGA/NodeTrackerManipulator>

#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <OsgEarth/ModelLayer>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <string>
#include <QtCore/QFileInfo>

#include "IDPApplication.h"
#include "DataProvider.h"
#include "RadarVectorLayer.h"
#include "Proj4Transform.h"
#include "VectorLayer.h"
#include "OsgEarthTransform.h"

#include "ExtendMap.h"
#include "GISMapManager.h"
#include "OsgEarthBridge.h"

#include "ManualDrawLine.h"
#include "DefaultManipulator.h"
#include "AdapterWidget.h"
#include "RoseGraph.h"

#include "EarthquakeFileDisplay.h"
#include "AnimaticControler.h"
#include "ViewManager.h"
#include "RadarDataAccess.h"

#include "mainwindow.h"
#include "sysmainwindow.h"
//#include "LeftDockWidget.h"
#include "statustoolbar.h"
#include "LegendWidget.h"
#include "CipasComposer.h"

#include "DefaultToolBar.h"

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

class SaveImageDrawCallback : public osg::Camera::DrawCallback 
{
public:
	SaveImageDrawCallback(osg::Image* pI){m_gImage = pI;}

	virtual void operator () (const ::osg::Camera& camera) const 
	{
		osgDB::writeImageFile(*m_gImage, "0000000.bmp");
	}

	osg::Image* m_gImage;
};

class DataInterface
{
public:
	virtual void DataInput(
		size_t iLonNCCount, 
		size_t iLatNCCount,  
		size_t iTimeNCCount, 
		size_t iLevelNCCount, 
		float fLonBegin, 
		float fLonEnd,
		float fLonDelta,
		float fLatBegin, 
		float fLatEnd,
		float fLatDelta,
		float fMissingValue,
		float fLevelBegin,
		float fLevelEnd,
		float fLevelDelta, 
		std::vector<float>& fLevelVector, 
		std::string& strTimeBegin,
		std::string& strTimeEnd,
		int iTimeDelta,
		double* dData, 
		int nModel, 
		int useNineP) = 0;
	virtual void DataInput( const std::string& ctlFile, const std::string& dataFile ) = 0;
	virtual void DataOutput() = 0;
	virtual void Draw(int iType) = 0;
};


DefaultToolBar::DefaultToolBar(QWidget *parent)	: QToolBar(parent),m_DefaultActionGroup(0)
{
	ui.setupUi(this);
	//m_pConfigDialog = NULL;
	
	m_pRaderDataAccess = RadarDataAccess::Instance();
	foreach(QAction* action,this->actions())
	{
		m_DefaultActionGroup.addAction(action);
	}

	m_pMW = SysMainWindow::GetMainWindow();

	InitSlots();
}

DefaultToolBar::~DefaultToolBar()
{

}

void DefaultToolBar::InitSlots()
{
	connect(&m_DefaultActionGroup, SIGNAL(triggered(QAction*)),this,  SLOT(Slots_ActionTriggered(QAction*)));
}


/** 
  * @brief    文件工具栏槽   
  * @author   易家祥  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   DefaultToolBar  文件工具栏槽
  * @note     易家祥
  * @date     2013年3月                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void DefaultToolBar::Slots_ActionTriggered( QAction* action )
{
	if (action == ui.actionOpen)
	{
		m_pMW->ShowMeteoDataDialog();
	}
	else if (action == ui.actionConfig)
	{
		/*m_pConfigDialog = new ConfigDialog(this);
		m_pConfigDialog->exec();*/
	}
	else if (action == ui.actionExit)
	{
		m_pMW->close();
	}
}


void DefaultToolBar::LoadData(const QString & fileName)
{
	bool bOpenedFile = false;
	static int num = -1;
	if (!QFile::exists(fileName))
	{
		QMessageBox::warning(this, tr("历史文件"),tr("没有找到文件 %1:\n.").arg(fileName));
		return;
	}
	m_pMW->LoadData(fileName);
	//else if("grads" == QFileInfo(fileName).suffix())
	//{
	//	typedef DataProvider* (*DataProviderPluginMain)();
	//	DataProvider* radarDProvider = 0;
	//	QLibrary lib("GradsProvider.dll");

	//	if(lib.load())
	//	{
	//		DataProviderPluginMain proMain = (DataProviderPluginMain)lib.resolve("DataProviderPluginMain");
	//		radarDProvider = proMain();

	//		DataInterface* pDI = (DataInterface*)(radarDProvider->GetExtendInterface(0));

	//		QFile txt(fileName);
	//		txt.open(QIODevice::ReadOnly);

	//		QTextStream stream(&txt);

	//		double* pData = new double[100];

	//		int i = 0;
	//		QString strValue;
	//		do {

	//			strValue = stream.readLine();
	//			if(i < 100)
	//			{
	//				
	//				double fLon = strValue.toDouble();

	//				*(pData + i) = fLon;
	//			}
	//			i++;

	//		} while (!strValue.isNull());
	//		
	//		 txt.close();
	//		 
	//		 //初始化图层给数据提供器
	//		 goto_gis::VectorLayer* pLayer = new goto_gis::VectorLayer;

	//		 pLayer->SetDataProvider(radarDProvider);
	//		 pLayer->FontSize(18);
	//		 
	//		 radarDProvider->ParentLayer(pLayer);

	//		 std::vector<float> fLevelVector;
	//		 std::string strTimeBegin, strTimeEnd;
	//		 pDI->DataInput(10, 10, 1, 1, 101, 146, 5, 50, 68, 2, 99999, 0, 0, 0,  fLevelVector, strTimeBegin, strTimeEnd, 1, pData, 1, 0);
	//		 pDI->Draw(1);
	//		 
	//		 //添加图层
	//		 ViewManager* viewManger = m_pMW->GetViewManger();
	//		 ViewerQT* viewerWindow =dynamic_cast<ViewerQT*>(viewManger->CreateViewport(0));
	//		 Map* m_p2DBasicMap = viewManger->GetMap2D();

	//		 m_p2DBasicMap->AddLayer("adfasdf", pLayer);

	//		 m_p2DBasicMap->CreateMapData();
	//		 m_pMW->GetCentralMdiArea()->CreateSubWindow(viewerWindow);

	//		 viewerWindow->getViewer()->setSceneData(m_p2DBasicMap->GetRootNode());
	//		 viewerWindow->getViewer()->setCameraManipulator(new osgGA::TrackballManipulator);
	//		 viewerWindow->showMaximized();
	//	}
	//}
}

osg::Vec3d DefaultToolBar::WorldToScreen( ViewerQT *view,osg::Vec3 worldpoint )
{
	double in[4],out[4];
	in[0] = worldpoint._v[0];
	in[1] = worldpoint._v[1];
	in[2] = worldpoint._v[2];
	in[3] = 1.0;
	//获得当前的投影矩阵和模型视图矩阵
	osg::Matrix projectMatrix = view->getViewer()->getCamera()->getProjectionMatrix();
	osg::Matrix viewprojectMatrix = view->getViewer()->getCamera()->getViewMatrix();
	//变换模型视图矩阵
	double modelViewMatrix[16];
	memcpy(modelViewMatrix,viewprojectMatrix.ptr(),sizeof(GLdouble)*16);
	Transform_Point(out,modelViewMatrix,in);
	//变换投影矩阵
	double myprojectmatrix[16];
	memcpy(myprojectmatrix,projectMatrix.ptr(),sizeof(GLdouble)*16);
	Transform_Point(in,myprojectmatrix,out);
	//变换视口变换矩阵
	if (in[3] ==0.0)
	return osg::Vec3d(0,0,0);
	in[0]/=in[3];
	in[1]/=in[3];
	in[2]/=in[3];

	int viewPort[4];

	osg::Viewport * myviewPort = view->getViewer()->getCamera()->getViewport();
	 viewPort[0] = myviewPort->x();
	 viewPort[1] = myviewPort->y();
	 viewPort[2] = myviewPort->width();
	 viewPort[3] = myviewPort->height();
	 //计算二维屏幕投影点
	 osg::Vec3d screePoint;
	 screePoint._v[0] = (int)(viewPort[0] + (1+in[0])*viewPort[2]/2 +0.5);
	 screePoint._v[1] = (int)(viewPort[1] + (1+in[1])*viewPort[3]/2 +0.5);
	 screePoint._v[2] = 0;
	 return screePoint;



}

void DefaultToolBar::Transform_Point( double out[4],const double m[16],const double in[4] )
{
#define M(row,col) m[col*4 +row]
	out[0] = M(0,0)*in[0] + M(0,1) *in[1] +M(0,2) *in[2]+M(0,3) *in[3];
	out[1] = M(1,0)*in[0] + M(1,1) *in[1] +M(1,2) *in[2]+M(1,3) *in[3];
	out[2] = M(2,0)*in[0] + M(2,1) *in[1] +M(2,2) *in[2]+M(2,3) *in[3];
	out[3] = M(3,0)*in[0] + M(3,1) *in[1] +M(3,2) *in[2]+M(3,3) *in[3];
#undef M
}

void DefaultToolBar::SetEmitForm( QString dataPath )
{
	m_EmitFormDataPath = dataPath;
}

//void DefaultToolBar::LoadCZData( QString filename )
//{
//	typedef DataProvider* (*DataProviderPluginMain)();
//	MainWindow* mw = SysMainWindow::GetMainWindow();
//	ViewManager* viewManger = SysMainWindow::GetMainWindow()->GetViewManger();
//	QWidget* viewwin = viewManger->CreateViewport(0);
//	RoseGraph* viewerWindow = dynamic_cast<RoseGraph*>(viewwin);
//	SysMainWindow::GetMainWindow()->GetCentralMdiArea()->CreateSubWindow(viewerWindow);
//	DataProvider* radarDProvider = 0;
//	
//	QLibrary myLib("EarthquakeDataProvider.dll");
//	DataProviderPluginMain GetRadarMosaicDataProvider = (DataProviderPluginMain) myLib.resolve("DataProviderPluginMain");
//	radarDProvider = GetRadarMosaicDataProvider();
//	//////////////////////////////////测试数据////////////////////////////////////////
//	QVector<int> varry;
//	for (int i = 0 ;i< 36; i++)
//	{
//		varry.append(rand()%8);
//	}
//	viewerWindow->SetSizeArray(varry);//修改varry
//	viewerWindow->showMaximized();
//}

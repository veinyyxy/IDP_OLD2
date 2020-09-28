#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtOpenGL/QGLWidget>
#include <osgGA/TrackballManipulator>
#include <osg/ClipPlane>
#include <osg/ShapeDrawable>

#include <QMessageBox>

#include <Windows.h>
#include <osgEarth/SpatialReference>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <osgEarth/ModelLayer>
#include <osgEarthUtil/SkyNode>
#include <osg/ImageUtils>

#include "DataProvider.h"
#include "VectorLayer.h"
#include "RadProjTransform.h"
#include "DrawGeometryData.h"

#include "cDrawProduct.h"
#include "InterpolationRadarData.h"

#include "RadarVectorLayer.h"
#include "AdapterWidget.h"
#include "CompositeViewerQT.h"
#include "DefaultManipulator.h"
#include "ViewManager.h"
#include "OsgEarthTransform.h"
#include "..\RadarSingleDataProvider\DrawLrmImage.h"
#include "ManualDrawRect.h"
#include <osgGA/GUIEventHandler>
#include "PlotDialog.h"
//#include "AreaDialog.h"

#include "ClipCallback.h"
#include "VolumeImage.h"
#include "radarsingledataprovider.h"
#include "ManualDrawLine.h"
#include "DB2Dialog.h"

#include "DrawRect.h"
#include "DrawGeometryActions.h"
#include "ClipBoard.h"
#include "ClipCallback.h"
#include "DataProvider.h"
#include "RadarDataAccess.h"
#include "RadProjTransform.h"
#include "IsoSurface.h"
#include "ClipPlaneInterface.h"

using namespace goto_gis;

class DeleteMap : public DeleteData
{
public:
	Map* m_pMap;

	virtual void DoDelete()
	{
		if(m_pMap)
			delete m_pMap;
	}
};

void ScreenToWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out )
{
	osg::Matrix VPW = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();
	osg::Matrix inverseVPW;
	inverseVPW.invert(VPW);
	v3Out = v3In  * inverseVPW;

	v3Out.set(v3Out.x(),v3Out.z(),v3Out.y());
	//v3Out = m_pCoord->Transform(v3Out);
}

class ViewerValueEventHandle : public osgGA::GUIEventHandler
{
public:
	ViewerValueEventHandle(DataProvider *da)
	{
		dp = da;
		m_Proj4 = new goto_gis::Proj4Transform;
	}
	virtual bool handle( const GUIEventAdapter& ea,GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor* )
	{
		osg::Vec3 currentPosition;
		ScreenToWorld(m_pCamera,osg::Vec3(ea.getX(),ea.getY(),0),currentPosition);

		currentPosition = m_Proj4->Transform(currentPosition);
		Layer * currentLayer = dp->ParentLayer()->GetParentMap()->CurrentLayer();
		if (NULL ==currentLayer)
		{
			return osgGA::GUIEventHandler::handle(ea,aa);;
		}
		std::string layername = currentLayer->LayerName();
		QString index = QString::fromStdString(layername).replace("第","");
		int layerIndex = (index.replace("仰角层","")).toInt() -1;
		double value = dp->GetPositionValue(currentPosition.x(),currentPosition.y(),layerIndex);
		QString val = QString::number(value) + dp->GetDataUnit();
		m_pMap = dp->ParentLayer()->GetParentMap();
		m_pMap->SetCurrentValue(val.toStdString());
		return osgGA::GUIEventHandler::handle(ea,aa);
	}
	goto_gis::Proj4Transform* m_Proj4;
	DataProvider *dp;
	Map* m_pMap;
	osg::Camera* m_pCamera;
};
RadarDataAccess* RadarDataAccess::g_RadarDataAccess = 0;

RadarDataAccess::RadarDataAccess(QObject* parent) : QObject(parent), m_DrawLrmImage(0)
{
}

RadarDataAccess::RadarDataAccess(ViewManager* pVM , QObject* parent /*= 0 */) : QObject(parent), m_pViewManager(pVM)
{

}


RadarDataAccess::~RadarDataAccess(void)
{
}

QWidget* RadarDataAccess::ReadRadarData( const QString& filename, int wdt,QWidget* parent /*= 0*/)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	typedef DataProvider* (*DataProviderPluginMain)();
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(m_pViewManager->CreateViewport(RadarData,true,parent));
	
	viewerWindow->getViewer()->setLightingMode(osg::View::NO_LIGHT);
	
	Map* extMap = (Map*)(viewerWindow->property("Map").toLongLong());//new ExtendMap(true,viewerWindow->getViewer());
	QString d3Map = QCoreApplication::applicationDirPath() + ("/Config/Default.earth");
	extMap->Load3DMapFromConfig(d3Map.toStdString());
	
	osg::ref_ptr<DeleteMap> pDelMap = new DeleteMap;
	pDelMap->m_pMap = extMap;

	viewerWindow->SetDeleteCallback(pDelMap);
	//osg::ref_ptr<osg::Group> rootNode = extMap->GetRootNode();
	LoadRadarData(filename,extMap);
	//double dfMinus,dfFreq,dfTime;
	//LARGE_INTEGER litmp;
	//LONGLONG Qpart1,Qpart2,Useingtime;
	//QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
	//dfFreq = (double)litmp.QuadPart;
	//QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
	//Qpart1 = litmp.QuadPart; //开始计时

	extMap->CreateMapData();
	//viewerWindow->getViewer()->setSceneData(rootNode); //加载场景
	//viewerWindow->getViewer()->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	//viewerWindow->getViewer()->realize();
	//viewerWindow->getViewer()->addEventHandler(new osgGA::StateSetManipulator(rootNode->getOrCreateStateSet()));
	//viewerWindow->getViewer()->addEventHandler(new osgViewer::StatsHandler);
	//viewerWindow->getViewer()->addEventHandler(new osgViewer::WindowSizeHandler);
	//viewerWindow->getViewer()->addEventHandler(new osgGA::GUIEventHandler);
	double dfMinus,dfFreq,dfTime;
	LARGE_INTEGER litmp;
	LONGLONG Qpart1,Qpart2,Useingtime;
	QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
	Qpart1 = litmp.QuadPart; //开始计时

	extMap->ScaleToLayer();

	//QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
	//Qpart2 = litmp.QuadPart; //终止计时
	//dfMinus = (double)(Qpart2 - Qpart1)/1000000;//计算计数器值
	//qDebug()<< "step Switch2DMap useing:"<<dfMinus;
	
	DefaultManipulator* dm = dynamic_cast<DefaultManipulator*>(viewerWindow->getViewer()->getCameraManipulator());//new DefaultManipulator;

	if (dm)
	{
		//viewerWindow->getViewer()->setCameraManipulator(dm);
		Layer* pCircleLayer = extMap->GetLayer("第1仰角层");
		if (pCircleLayer != 0)
		{
			ViewerValueEventHandle* PrintCurrentLonLat = new ViewerValueEventHandle(pCircleLayer->GetDataProvider());
			PrintCurrentLonLat->m_pCamera = viewerWindow->getViewer()->getCamera();
			viewerWindow->getViewer()->addEventHandler(PrintCurrentLonLat);
			goto_gis::CoordinateTransform* pCT = extMap->GetCoordinateTransform();
			PrintCurrentLonLat->m_Proj4->setSourceCRS(pCT->getDesCRS());
			PrintCurrentLonLat->m_Proj4->setSourceProjName(pCT->getDesProjName());
			PrintCurrentLonLat->m_Proj4->setDestCRS(pCT->getSourceCRS());
			PrintCurrentLonLat->m_Proj4->setDesProjName(pCT->getSourceProjName());

			osg::Node* pLayerNode = pCircleLayer->GetLayerNode();
			osg::Matrixd homeMatrix;
			osg::Vec3d eye, center, up;
			boost::any anyR;
			QString strR;
			std::string stdR;
			double dR = -1;

			if(pCircleLayer->GetLayerAtrributeFromName("有效范围：", anyR))
			{
				stdR = boost::any_cast<std::string>(anyR);
				strR = QString::fromStdString(stdR).remove("Km");
				dR = strR.toDouble()  * 1000;
			}

			dm->GoToNodeCenter(viewerWindow->getViewer()->getCamera(), pLayerNode, dR);
		}
	}
	QApplication::restoreOverrideCursor();
	return viewerWindow;
}


void RadarDataAccess::LoadRadarData( QString filename ,Map* map)
{
	//typedef DataProvider* (*DataProviderPluginMain)();
	//DataProvider* radarDProvider = 0;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	CoordinateTransform* coord = m_pViewManager->GetMap2D()->GetCoordinateTransform();//得到投影
	goto_gis::GeosOnTheOsg* pGeosOnTheosg = goto_gis::GeosOnTheOsg::Instantiate();
	
	goto_gis::RadarVectorLayer* vLayer0 = (goto_gis::RadarVectorLayer*)(pGeosOnTheosg->OpenRadarDataAsLayer(filename.toStdString(),map,coord));
	//////////////////////////////////////////////////////////////////////////
	{
	
		map->CurrentLayer(vLayer0);
		vLayer0->CoordTrans(coord);
		vLayer0->LayerZ(0.5f);
		//radarDProvider->ParentLayer(vLayer0);
		//vLayer0->CreateLayer(filename.toStdString());
		//vLayer0->CreateLayerData();
		//vLayer0->TransformLayer();
		QFileInfo info(filename);
		//QString layerName =filename + "-第1仰角层";
		QString layerName = "第1仰角层";

		map->AddLayer(layerName.toStdString(),vLayer0);
		//map->TramsformMap();

		

		m_DrawLrmImage = (DrawLrmImage*)(vLayer0->GetDataProvider()->GetExtendInterface(2));
	}
	QApplication::restoreOverrideCursor();
}

void RadarDataAccess::LoadRadarClipPlane(Map* p3DMap, IsoSurface* pOEB )
{
	Layer* pNamedLayer = p3DMap->GetLayer("距离圈");
	boost::any anyR;
	QString strR;
	std::string stdR;
	double dR = -1;
	osg::Vec3 v3Central;
	QRectF rectf;
	if(pNamedLayer)
	{
		if(pNamedLayer->GetLayerAtrributeFromName("有效范围：", anyR))
		{
			stdR = boost::any_cast<std::string>(anyR);
			strR = QString::fromStdString(stdR).remove("Km");
			dR = strR.toDouble()  * 1000;
		}
		if(pNamedLayer->GetLayerAtrributeFromName(QString::fromLocal8Bit("中心：").toStdString(), anyR))
		{
			v3Central = boost::any_cast<osg::Vec3>(anyR);
		}
		if(pNamedLayer->GetLayerAtrributeFromName("边界", anyR))
		{
			rectf = boost::any_cast<QRectF>(anyR);
		}
	}
	
	ClipPlaneInterface* pPI = (ClipPlaneInterface*)p3DMap->
		GetExtendInterface(std::string("ClipPlaneInterface"));
	pOEB->SetAddtionalData(&v3Central);
	osg::Vec4 osgRect(rectf.left(), rectf.top(), rectf.right(), rectf.bottom());
	pPI->AddClipPlane(p3DMap->GetRootNode(), p3DMap->GetRootSwitch(), osgRect);
}

void RadarDataAccess::InitMachCubeData( Map* p3DMap, IsoSurface* pOEB )
{
	Layer* pLayer0 = p3DMap->GetLayer("第1仰角层");
	if (pLayer0)
	{
		DataProvider* pDataProvider = pLayer0->GetDataProvider();
		void* pRDI = pDataProvider->GetExtendInterface(1);

		RadarMarchingCubes* pRMC = (RadarMarchingCubes*)pRDI;

		pOEB->SetRadarMarchingCubes(pRMC);
	}
}

void RadarDataAccess::SetRadarCenter(Map* p3DMap, IsoSurface* pOEB )
{
	Layer* pNamedLayer = p3DMap->GetLayer("距离圈");
	boost::any anyR;
	QString strR;
	osg::Vec3 v3Central;
	if(pNamedLayer)
	{
		if(pNamedLayer->GetLayerAtrributeFromName(QString::fromLocal8Bit("中心：").toStdString(), anyR))
		{
			v3Central = boost::any_cast<osg::Vec3>(anyR);
		}
	}

	pOEB->SetAddtionalData(&v3Central);
	//pOEB->SetRadarCenter(v3Central);
}

void RadarDataAccess::MakeFenCengZuHeFanSheLuRect(DefaultManipulator* pDrawRect)
{
	ManualDrawRect* pManualDrawRect  = pDrawRect->GetManualDrawRect();
	pManualDrawRect->GetScreenPoint();
	pManualDrawRect->GetWorldPoint();
}

RadarDataAccess* RadarDataAccess::Instance(ViewManager* pVM , QObject* parent)
{
	if(!g_RadarDataAccess)
	{
		g_RadarDataAccess = new RadarDataAccess(pVM,parent);
	}
	return g_RadarDataAccess;
}

void RadarDataAccess::Switch2DMap(ViewerQT* viewer)
{
	//viewer->getViewer()->setCameraManipulator(new DefaultManipulator , true);
}

void RadarDataAccess::Switch3DMap(ViewerQT* viewer)
{
	//viewer->getViewer()->setCameraManipulator(new osgEarth::Util::EarthManipulator, true);
}

QWidget* RadarDataAccess::Win4Viewer( Map* srcMap)
{
	CompositeViewerQT* compositeViewerQT = 0;
	QDialog dig(QApplication::desktop());
	dig.setWindowTitle(QString::fromLocal8Bit("选择图层框"));
	QGridLayout g;
	QLabel la(QString::fromLocal8Bit("请选择开始层："));
	QComboBox cmb;

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok| QDialogButtonBox::Cancel);
	connect(&buttonBox, SIGNAL(accepted()), &dig, SLOT(accept()));
	connect(&buttonBox, SIGNAL(rejected()), &dig, SLOT(reject()));
	
	std::map<std::string, Layer*> mapLayers = srcMap->GetMapLayers();
	std::map<std::string, Layer*>::iterator layerIter = mapLayers.begin();
	while(layerIter != mapLayers.end())
	{
		QString name = QString::fromStdString(layerIter->first);
		if (name.contains("仰角层") && !name.contains("距离圈"))
		{
			cmb.addItem(name);
		}
		layerIter++;
	}
	g.addWidget(&la,0,0);
	g.addWidget(&cmb,0,1);
	g.addWidget(&buttonBox,1,0,1,2);

	dig.setLayout(&g);

	if (dig.exec() == QDialog::Accepted)
	{
		int layerIndex = cmb.currentText().remove("第").remove("仰角层").trimmed().toInt();
		compositeViewerQT = new CompositeViewerQT(layerIndex);

		Map* map1 = new Map();
		Map* map2 =  new Map();
		Map* map3 =  new Map();
		Map* map4 =  new Map();
	
		map1->SetCoordinateTransform(srcMap->GetCoordinateTransform());
		*map1 =  *srcMap;

		map2->SetCoordinateTransform(srcMap->GetCoordinateTransform());
		*map2 =  *srcMap;

		map3->SetCoordinateTransform(srcMap->GetCoordinateTransform());
		*map3 =  *srcMap;

		map4->SetCoordinateTransform(srcMap->GetCoordinateTransform());
		*map4 =  *srcMap;

		map1->CreateMapData();
		map2->CreateMapData();
		map3->CreateMapData();
		map4->CreateMapData();

		QWidget* widget1 = compositeViewerQT->AddViewWidget( compositeViewerQT->CreateCamera(0,0,100,100), map1->GetRootNode() );
		QWidget* widget2 = compositeViewerQT->AddViewWidget( compositeViewerQT->CreateCamera(0,0,100,100), map2->GetRootNode());
		QWidget* widget3 = compositeViewerQT->AddViewWidget( compositeViewerQT->CreateCamera(0,0,100,100), map3->GetRootNode() );
		QWidget* widget4 = compositeViewerQT->AddViewWidget( compositeViewerQT->CreateCamera(0,0,100,100), map4->GetRootNode());
		
		widget1->setStyleSheet("background-color: rgb(0, 0, 0);");
		widget2->setStyleSheet("background-color: rgb(0, 0, 0);");
		widget3->setStyleSheet("background-color: rgb(0, 0, 0);");
		widget4->setStyleSheet("background-color: rgb(0, 0, 0);");

		widget1->setProperty("Map",(long long)map1);
		widget2->setProperty("Map",(long long)map2);
		widget3->setProperty("Map",(long long)map3);
		widget4->setProperty("Map",(long long)map4);

		compositeViewerQT->setProperty("DType",Win4);

		QGridLayout* grid = new QGridLayout(compositeViewerQT);
		grid->setContentsMargins(0,0,0,0);
		grid->setMargin(0);
		grid->setHorizontalSpacing(1);
		grid->setVerticalSpacing(1);
		grid->addWidget( widget1, 0, 0 );
		grid->addWidget( widget2, 0, 1 );
		grid->addWidget( widget3, 1, 0 );
		grid->addWidget( widget4, 1, 1 );
		compositeViewerQT->setLayout( grid );

		StartShowLayerIndex(map1,layerIndex);
		StartShowLayerIndex(map2,layerIndex+1);
		StartShowLayerIndex(map3,layerIndex+2);
		StartShowLayerIndex(map4,layerIndex+3);

		compositeViewerQT->CurrentWidget(widget1);
	}
	return compositeViewerQT;
}

void RadarDataAccess::StartShowLayerIndex(Map* map , int index)
{
	std::string layername = "第" + QString::number(index).toStdString()+"仰角层";

	std::map<std::string, Layer*> m_mapLayer = map->GetMapLayers();
	std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();

	QStringList blist = map->BasisLayerList();
	while(layerIter != m_mapLayer.end())
	{
		if (blist.contains(QString::fromStdString(layerIter->first)))
		{
			layerIter++;
			continue;
		}
		Layer* layer = layerIter->second;
		if(layer)
		{
			if (layerIter->first == layername)
			{
				map->SetVisibleLayer(layerIter->first,true);
			}
			else
			{
				map->SetVisibleLayer(layerIter->first,false);
			}
		}
		layerIter++;
	}
}

void RadarDataAccess::PlotWidget( ViewerQT* viewer )
{
	DefaultManipulator* dm = dynamic_cast<DefaultManipulator*>(viewer->getViewer()->getCameraManipulator());
	osg::Vec3Array* vec3arr = dm->GetManualDrawLine()->GetWorldPoint();
	if(vec3arr->size() <=0)
	{
		QMessageBox::information(0,tr("提示"),tr("请按住《Alt》键-----选择点！"));
		return;
	}

	Map* map = (goto_gis::Map*)viewer->property("Map").toLongLong();;
	Layer * layer0  = map->GetLayer("第1仰角层");
	if (!layer0)
	{
		return;
	}
	DataProvider* dataProvider = layer0->GetDataProvider();
	if (dataProvider)
	{
		void* pvoid = dataProvider->GetExtendInterface(0);
		InterpolationRadarData* pRadarInterface = (InterpolationRadarData*)(pvoid);
		if(pRadarInterface)
		{
			RadProjTransform* pRadProj = dynamic_cast<RadProjTransform*>(layer0->CoordTrans());

			Proj4Transform* pProj4 = new Proj4Transform();
			pProj4->setSourceCRS(map->GetCoordinateTransform()->getDesCRS());
			pProj4->setSourceProjName(map->GetCoordinateTransform()->getDesProjName());
			pProj4->setDestCRS(map->GetCoordinateTransform()->getSourceCRS());
			pProj4->setDesProjName(map->GetCoordinateTransform()->getDesProjName());

			osg::Vec3Array* p = dm->GetManualDrawLine()->GetScreenPoint();
			double x1, y1, x2, y2;

			osg::Vec3 dst1,dst2;
			dst1 = pProj4->Transform(osg::Vec3(vec3arr->at(1).x(),vec3arr->at(1).z(),0.5));
			x1 = dst1.x();
			y1 = dst1.y();

			dst2 = pProj4->Transform(osg::Vec3(vec3arr->at(0).x(),vec3arr->at(0).z(),0.5));
			x2 = dst2.x();
			y2 = dst2.y();

			osg::Vec3  p1 = pRadProj->xyWorld2DC(x1,y1);
			osg::Vec3  p2 = pRadProj->xyWorld2DC(x2,y2);

			double dRatio = abs(Get_Length(p1.x()/1000.0-p2.x()/1000.0,p1.y()/1000.0-p2.y()/1000.0))/abs(Get_Length(p->at(0).x()-p->at(1).x(),p->at(0).y()-p->at(1).y()));


			double ProductCenter_X = p->at(0).x()-p1.x()/1000.0/dRatio;
			double ProductCenter_Y = p->at(0).y()+p1.y()/1000.0/dRatio;

			//boost::any  _center , _elevtion;
			//map->GetLayer("第1仰角层")->GetLayerAtrributeFromName("中心：",_center);
			//map->GetLayer("第1仰角层")->GetLayerAtrributeFromName("仰角：",_elevtion);
			double _x1 , _y1, _x2, _y2;
			_x1 = p->at(0).x();
			_y1 = p->at(0).y();

			_x2 = p->at(1).x();
			_y2 = p->at(1).y();

			pRadarInterface->SetStartEnd(&_x1,  &_y1,  &_x2,  &_y2,dRatio,ProductCenter_X,ProductCenter_Y);
			pRadarInterface->Profiles();
			QStringList InfoList;
			P_FIELD_MAP layerAttributeDescripe = map->GetLayer("第1仰角层")->GetLayerAttributeMap();
			std::map<int, GeomField>::iterator layerIter = layerAttributeDescripe->begin();
			while(layerIter != layerAttributeDescripe->end())
			{
				int index = layerIter->first;
				std::string typeName = layerIter->second.typeName();
				std::string name = layerIter->second.name();
				std::string comment = layerIter->second.comment();
				boost::any _any = GetAttributeValue(index,map->GetLayer("第1仰角层"));
				if (typeName == "String") // 字符串
				{
					std::string _value =  boost::any_cast<std::string>(_any);
					InfoList.append(QString::fromStdString(_value));
				}
				layerIter++;
			}
			if (pRadarInterface->ds <= 0 || pRadarInterface->dh <= 0)
			{
				QMessageBox::information(0,tr("提示"),tr("数据异常，请重新选择数据！"));
				return;
			}
			PlotDialog* pPlotDialog = new PlotDialog(0);
			pPlotDialog->GetArray(tr(""),pRadarInterface->ds,pRadarInterface->dh,pRadarInterface->buf,pRadarInterface->max,pRadarInterface->maxH,pRadarInterface->maxAz,pRadarInterface->maxR,dataProvider->GetColorTable(),pRadarInterface->AzList,pRadarInterface->RList,InfoList);
			pPlotDialog->show();
		}
	}
}

void RadarDataAccess::DB2Widget( Map* srcMap )
{
	Layer* layer = srcMap->CurrentLayer();
	if (layer)
	{
		P_COLORTABLE_VECTOR p = layer->GetDataProvider()->GetColorTable();
		QList<double> start;
		QList<double> end;
		for (int i =0; i< p->size();i++)
		{
			start.append(p->at(i)->fStart);
			end.append(p->at(i)->fEnd);
		}

		qSort(start.begin(),start.end());  
		qSort(end.begin(),end.end(),qGreater<int>());  

		DB2Dialog* pDB2Dialog = new DB2Dialog(srcMap,start[0],end[0],0);
		pDB2Dialog->show();
	}
}

boost::any RadarDataAccess::GetAttributeValue(int index ,Layer* layer)
{
	boost::any _value;
	Layer::P_LAYER_ATTRIBUTE_MAP pLayerAttribteTable = layer->GetLayerAttribteTable();
	Layer::LAYER_ATTRIBUTE_MAP::iterator findResult = pLayerAttribteTable->find(index);
	_value = findResult->second;
	return _value;
}

double RadarDataAccess::Get_Length( double dx, double dy )
{
	return( sqrt(dx*dx + dy*dy) );
}

void RadarDataAccess::AreaWidget( Map* srcMap)
{
	//AreaDialog* pAreaDialog = new AreaDialog(srcMap, 0);
	//pAreaDialog->show();
}

void RadarDataAccess::VolumeRender( Map* srcMap )
{
		Map* pMap = (srcMap);
		IsoSurface* pOEB = (IsoSurface*)pMap->GetExtendInterface(std::string("IsoSurface"));
		//pOEB->ComputerIso(iValue);
		//pOEB->Computer3DClouds();
		goto_gis::Map* map1 = pMap;

		goto_gis::Layer* pCircleLayer = map1->GetLayer("距离圈");

		if (pCircleLayer != 0)
		{
			boost::any anyValue;
			QString qstrValue;
			std::string strValue;
			double dR = -1, dH = 0;
			if(pCircleLayer->GetLayerAtrributeFromName("有效范围：", anyValue))
			{
				strValue = boost::any_cast<std::string>(anyValue);
				qstrValue = QString::fromStdString(strValue).remove("Km");
				dR = qstrValue.toDouble()  * 1000;
			}
			else
			{
				return;
			}

			if(pCircleLayer->GetLayerAtrributeFromName("海拔高度：", anyValue))
			{
				strValue = boost::any_cast<std::string>(anyValue);
				qstrValue = QString::fromStdString(strValue);
			
				dH = qstrValue.toDouble();
			}
			else
			{
				return;
			}

			goto_gis::Layer* pRadarLayer = map1->GetLayer("第1仰角层");
			if(!pRadarLayer) return;
			goto_gis::DataProvider* pDP = pRadarLayer->GetDataProvider();
			if(!pDP) return;

			VolumeImage* pVI = (VolumeImage*)(pDP->GetExtendInterface(3));
			if(!pVI) return;

			QVector<QImage> QtImages;
			pVI->GetImages(&QtImages);

			osg::ImageList imageList;

			for(size_t i = 0; i < QtImages.count(); i++)
			{
				QImage image;
				image = QtImages.at(QtImages.count()-1-i);
				if(32 != image.depth()) return;
				image = QGLWidget::convertToGLFormat(image);//容易出问题崩溃
				unsigned char* data = new unsigned char[image.byteCount()];
				for (int i = 0; i< image.byteCount();i++)
				{
					data[i] = image.bits()[i];
				}
				osg::ref_ptr<osg::Image> image_osg   = new osg::Image;
				image_osg->setImage(image.width(), image.height(), 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, data, osg::Image::USE_NEW_DELETE, 1);

				//osg::Image* pOsgImage = osgDB::readImageFile(QString("radar%1.png").arg(i).toStdString());
				imageList.push_back(image_osg);
			}
			pOEB->ComputerVolume(imageList, dR*2, dR*2, dH);

			boost::any anyR;
			QString strR;
			std::string stdR;
						
			QRectF rectf;
			if(pRadarLayer)
			{
				if(pRadarLayer->GetLayerAtrributeFromName("有效范围：", anyR))
				{
					stdR = boost::any_cast<std::string>(anyR);
					strR = QString::fromStdString(stdR).remove("Km");
					dR = strR.toDouble()  * 1000;
				}
				if(pRadarLayer->GetLayerAtrributeFromName("边界", anyR))
				{
					rectf = boost::any_cast<QRectF>(anyR);
				}
			}

			osg::Vec4 osgRect(rectf.left(), rectf.top(), rectf.right(), rectf.bottom());

			pOEB->AddVolumeClipPlane(osgRect);
		}
}

void RadarDataAccess::Fuzzy( Map* srcMap )
{
	
}

void RadarDataAccess::ClipPlane( Map* srcMap, int iD )
{
	Map* pMap = srcMap;
	if(!pMap) return;

	IsoSurface* pOEB = (IsoSurface*)pMap->GetExtendInterface(std::string("IsoSurface"));
	Layer* pNamedLayer = pMap->GetLayer("距离圈");
	boost::any anyR;
	QString strR;
	std::string stdR;
	double dR = -1;
	osg::Vec3 v3Central;
	QRectF rectf;
	if(pNamedLayer)
	{
		if(pNamedLayer->GetLayerAtrributeFromName("有效范围：", anyR))
		{
			stdR = boost::any_cast<std::string>(anyR);
			strR = QString::fromStdString(stdR).remove("Km");
			dR = strR.toDouble()  * 1000;
		}
		if(pNamedLayer->GetLayerAtrributeFromName(QString::fromLocal8Bit("中心：").toStdString(), anyR))
		{
			v3Central = boost::any_cast<osg::Vec3>(anyR);
		}
		if(pNamedLayer->GetLayerAtrributeFromName("边界", anyR))
		{
			rectf = boost::any_cast<QRectF>(anyR);
		}
	}

	pOEB->SetAddtionalData(&v3Central);
	double dlt = abs(rectf.left() - rectf.right())/100.0;
	osg::Vec4 osgRect(rectf.left() +dlt*15, rectf.top()+dlt*15, rectf.right()-dlt*15 ,rectf.bottom()-dlt*15);

	ClipBoard* pClipBoard = 0;// = pOEB->AddClipPlane(pMap->GetRootNode(), pMap->GetRootSwitch(), osgRect);
	ClipPlaneInterface* pPI = (ClipPlaneInterface*)pMap->
		GetExtendInterface(std::string("ClipPlaneInterface"));
	switch(iD)
	{
	case 0:
		pClipBoard = pPI->AddClipPlane(pMap->GetRootNode(), pMap->GetRootSwitch(), osgRect);
		m_ClipBoardMap[0] = pClipBoard;
		break;
	case 1:
		pClipBoard = pPI->AddClipPlane(pMap->GetRootNode(), pMap->GetRootSwitch(), osgRect);
		m_ClipBoardMap[1] = pClipBoard;
		break;
	case 2:
		pClipBoard = pPI->AddClipPlane(pMap->GetRootNode(), pMap->GetRootSwitch(), osgRect);
		m_ClipBoardMap[2] = pClipBoard;
		break;
	}
	
	//pClipBoard->SetPlaneVisible(true);

	ClipCallback* pClipCallback = new ClipCallback(srcMap);
	pClipBoard->SetClipMoveCallback(pClipCallback);
}

void RadarDataAccess::DleClipPlane(Map* srcMap, int iD )
{
	Map* pMap = (srcMap);
	ClipPlaneInterface* pPI = (ClipPlaneInterface*)pMap->
		GetExtendInterface(std::string("ClipPlaneInterface"));

	std::map< int, void*>::iterator findedClip = m_ClipBoardMap.find(iD);

	if(findedClip != m_ClipBoardMap.end())
	{
		ClipBoard* pClipBoard = (ClipBoard*)(findedClip->second);

		pPI->DelClipPlane(pClipBoard);
	}
	
}

void RadarDataAccess::LoadRadarDataThreadSafe( QString filename ,Map* map )
{
	goto_gis::Proj4Transform* coord = new goto_gis::Proj4Transform;
	coord->setDestCRS("+proj=lcc +lat_1=20 +lat_2=60 +lat_0=0 +lon_0=103 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs");
	coord->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	//CoordinateTransform* coord = m_pViewManager->GetMap2D()->GetCoordinateTransform();//得到投影
	goto_gis::GeosOnTheOsg* pGeosOnTheosg = goto_gis::GeosOnTheOsg::Instantiate();

	goto_gis::RadarVectorLayer* vLayer0 = (goto_gis::RadarVectorLayer*)(pGeosOnTheosg->OpenRadarDataAsLayer(filename.toStdString(),map,coord));
	//////////////////////////////////////////////////////////////////////////

	//map->CurrentLayer(vLayer0);
	//vLayer0->CoordTrans(coord);
	//vLayer0->LayerZ(0.5f);
	//QFileInfo info(filename);
	//QString layerName = "第1仰角层";
	//map->AddLayer(layerName.toStdString(),vLayer0);
}

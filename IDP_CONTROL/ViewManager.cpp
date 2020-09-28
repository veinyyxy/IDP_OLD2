#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QtGui>
#include <QEvent>
#include <QCoreApplication>
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

#include "ColorTableActions.h"
#include "GDALProvider.h"
#include "GISMapManager.h"
#include "AnimaticControlerInterface.h"
#include "RadarAnimaticImp.h"
#include "Proj4Transform.h"
#include "VectorLayer.h"
#include "AdapterWidget.h"
#include "DefaultManipulator.h"
#include "Map.h"
#include "Layer.h"
//#include "../IDPUI/LegendWidget.h"
#include "OsgEarthTransform.h"
#include "ManualDrawLine.h"
#include "RadReadColor_XML.h"
#include "RoseGraph.h"
#include "ViewManager.h"
#include "GeosOnTheOsg.h"
#include "MapFactory.h"

#define UserUpdateGL2 0

using namespace goto_gis;

osg::Vec3d g_eye, g_centre, g_up;
double g_dis;


QGraphicsPixmapItemEX::QGraphicsPixmapItemEX( int x, int y)
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setPos(x, y);
}

QGraphicsPixmapItemEX::~QGraphicsPixmapItemEX()
{

}

void QGraphicsPixmapItemEX::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (!isSelected() && scene()) {
		scene()->clearSelection();
		setSelected(true);
	}

	QMenu menu;
	FlashAction *actionFlash = new FlashAction("闪速",0);//menu.addAction("闪速");
	actionFlash->setText("闪速");
	actionFlash->setProperty("PW",this->data(Qt::UserRole));
	actionFlash->setProperty("INDEX",this->data(Qt::ToolTipRole));
	menu.addAction(actionFlash);

	QAction *actionStopFlash = menu.addAction("停止闪速");
	QAction *selectedAction = menu.exec(event->screenPos());

	if (selectedAction == actionFlash)
	{
		actionFlash->MyDo();
	}
	else if (selectedAction == actionStopFlash)
	{

	}
	QWidget* cw = (QWidget*)this->data(Qt::UserRole).toLongLong();
	
	if (cw)
	{
		QMouseEvent mEvent(QEvent::MouseButtonRelease, QPoint(cw->width()/2,cw->height()/2), Qt::RightButton, Qt::RightButton, Qt::NoModifier);
		QApplication::sendEvent(cw,&mEvent);
	}
}

void QGraphicsPixmapItemEX::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsPixmapItem::mousePressEvent(event);
}

void QGraphicsPixmapItemEX::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsPixmapItem::mouseReleaseEvent(event);
}
//////////////////////////////////////////////////////////////////////////
ViewManager::ViewManager(void)
{
	m_p2DBasicMap = GeosOnTheOsg::Instantiate()->GetMapFactory()->CreateMap();
	//m_pGM = new GISMapManager(m_p2DBasicMap);

	//m_pOEB = new OsgEarthBridge;

	m_preViewMatrix.makeIdentity();
	m_preProjectionMatrix.makeIdentity();
	m_preViewMatrix3D.makeIdentity();
	m_preProjectionMatrix3D.makeIdentity();

	m_pRadarAnimatic = new RadarAnimaticImp;
}


ViewManager::~ViewManager(void)
{

}
void ViewManager::Init2DBasicNode( QString filename )
{
	m_p2DBasicMap->Load2DMapFromConfig(filename.toStdString());
	//m_p2DBasicMap->ReadMapScheme(filename,m_p2DBasicMap);
}

void ViewManager::Init3DBasicNode( QString filename )
{
	/*m_p3DBasicNode = m_pOEB->GetOsgEarthMapNode();
	m_pOEB->LoadEarthModel(filename.toLocal8Bit());

	osgEarth::MapNode* pMapNode = osgEarth::MapNode::findMapNode(m_p3DBasicNode);
	osgEarth::SpatialReference* pSRF = const_cast<osgEarth::SpatialReference* >(pMapNode->getMap()->getProfile()->getSRS());
	m_pProjEarth = new OsgEarthTransform(pSRF);*/
}

QWidget* ViewManager::CreateViewport(int wdt ,  bool basicMap/* = true*/ ,QWidget* parentWidget)
{
	if (wdt == RoseData)
	{
		RoseGraph* rg = new RoseGraph;
		return rg;
	}
	else
	{
		ViewerQT*  viewerWindow = new ViewerQT(0,parentWidget);
		//Map*  pExtMap = new ExtendMap(viewerWindow->getViewer());//三维地图
		MapFactory* pMF = GeosOnTheOsg::Instantiate()->GetMapFactory();
		Map*  pExtMap = pMF->CreateMap();
		pExtMap->SetAttachView(viewerWindow->getViewer());

		viewerWindow->getViewer()->setThreadingModel(osgViewer::Viewer::SingleThreaded);
		viewerWindow->getViewer()->setSceneData(pExtMap->GetRootNode());
		viewerWindow->getViewer()->realize();
		viewerWindow->getViewer()->addEventHandler(new osgViewer::StatsHandler);
		viewerWindow->getViewer()->addEventHandler(new osgViewer::WindowSizeHandler);
		viewerWindow->getViewer()->addEventHandler(new osgGA::GUIEventHandler);

		DefaultManipulator* dm = new DefaultManipulator;
		pExtMap->SetCoordinateTransform(m_p2DBasicMap->GetCoordinateTransform());
		viewerWindow->getViewer()->setCameraManipulator(dm);
		if (basicMap)
		{
			*pExtMap = *m_p2DBasicMap;
		}
		viewerWindow->setProperty("Map",(long long)pExtMap);
		viewerWindow->setProperty("Win2_3","2D");//默认2D显示
		viewerWindow->setProperty("DType",wdt);
		return viewerWindow;
	}
	return 0;
}

void ViewManager::CreateRadarViewport( int wdt ,QGLWidget* pGLWidget )
{
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(pGLWidget);
	
	QWidget* wCenter = new QWidget();
	wCenter->setWindowTitle(tr("雷达窗口"));
	wCenter->setStyleSheet(tr(" border:0px;"));
	wCenter->setObjectName("wCenter");
	wCenter->setProperty("DType",wdt);

	wCenter->setContentsMargins(0,0,0,0);
	QGridLayout* gLayout = new QGridLayout(wCenter);
	gLayout->setContentsMargins(0,0,0,0);
	wCenter->setLayout(gLayout);
	viewerWindow->setParent(wCenter);

	QGraphicsScene* gs = new QGraphicsScene();
	QGraphicsView* gv = new QGraphicsView(gs,wCenter);
	gv->setObjectName("RadarGraphicsView");
	gv->setContentsMargins(0,0,0,0);
	gv->setBackgroundBrush(Qt::black);
	gv->centerOn(0,0);
	gv->setFixedWidth(280);

	gLayout->addWidget(viewerWindow,0,0);
	gLayout->addWidget(gv,0,1);
}


void ViewManager::ShowRaderAttributeInfo(Layer* layer,QWidget* w)
{
	if (w->property("DType").toInt() == RadarData)
	{
		QGraphicsView* pGV = w->findChild<QGraphicsView*>();
		if(!pGV) return;
		pGV->show();
		P_FIELD_MAP layerAttributeDescripe = layer->GetLayerAttributeMap();
		std::map<int, GeomField>::iterator layerIter = layerAttributeDescripe->begin();
		QGraphicsScene* gs = pGV->scene();
		if (gs == 0 )
		{
			return;
		}
		gs->clear();
		QFont font1 = QFont(tr("黑体"),12);
		QVector<QColor> ColorVec;
		ColorVec.clear();
		ColorVec.push_back(QColor(255, 255, 255, 255));
		ColorVec.push_back( QColor(81, 146, 154, 255));
		ColorVec.push_back( QColor(245, 104, 2, 255));
		int nclolor = 1;
		QColor tempColor =ColorVec.at(0);
		font1.setPixelSize(13);
		font1.setBold(true);

		int height = 0;
		while(layerIter != layerAttributeDescripe->end())
		{
			int index = layerIter->first;
			std::string typeName = layerIter->second.typeName();
			//////////////////////////////////////////////////////////////////////////
			std::string name = layerIter->second.name();
			std::string comment = layerIter->second.comment();

			boost::any _any = GetAttributeValue(index,layer);
			if (typeName == "String") // 字符串
			{
				std::string _value =  boost::any_cast<std::string>(_any);
				QGraphicsTextItem* item = new QGraphicsTextItem(QString::fromStdString(name)+QString::fromStdString(_value));
				height = height+15;
		
				if (nclolor <= 5)
				{
				
					item->setDefaultTextColor(ColorVec.at(0));
				}
				else if (nclolor >= 6  && nclolor <= 11)
				{
					if (nclolor == 6)
					{
						height = height+15;
					}
					item->setDefaultTextColor(ColorVec.at(1));
				}
				else if(nclolor >= 12)
				{
					if (nclolor == 12)
					{
						height = height+15;
					}
					item->setDefaultTextColor(ColorVec.at(2));
				}
				item->setPos(0,height);
				nclolor++;
				item->setFont(font1);
				gs->addItem(item);
			}
		
			layerIter++;
		}
		height = height+45;
		layerIter = layerAttributeDescripe->begin();
		QPixmap pix(30,15);
		QPainter pt1(&pix);
		QPen pent1;
		while(layerIter != layerAttributeDescripe->end())
		{
			int index = layerIter->first;
			std::string typeName = layerIter->second.typeName();
			//////////////////////////////////////////////////////////////////////////
			std::string name = layerIter->second.name();
			std::string comment = layerIter->second.comment();

			boost::any _any = GetAttributeValue(index,layer);
			if (typeName == "ColorList")
			{
				P_COLORTABLE_VECTOR colorVector =  boost::any_cast<P_COLORTABLE_VECTOR>(_any);
				for (int i = 0 ; i< colorVector->size() ;i++)
				{
					P_COLORNODE_PRODUCT rgb = colorVector->at(i);
					int r = rgb->nRed;
					int g = rgb->nGreen;
					int b = rgb->nBlue;
					pix.fill(QColor(r,g,b,255));
					pt1.begin(&pix);
					pt1.setRenderHint(QPainter::Antialiasing,true);
					pent1 = pt1.pen();
					//QPen penText1 = pent1;
					pent1.setWidth(0);
					pent1.setColor(QColor(255,255,255));
					pt1.setPen(pent1);
					pt1.drawRect(0,0,30,15);
					pt1.end();

					height = height+15;
					QGraphicsPixmapItemEX* item= new QGraphicsPixmapItemEX(5,height);
					item->setData(Qt::UserRole , (long long)w);
					item->setData(Qt::ToolTipRole,i);
					item->setPixmap(pix);
					item->setPos(5,height);
					gs->addItem(item);

				/*	QPixmap pixText(30,15);
					pixText.fill(QColor(0,0,0,255));
					QPainter ptText(&pixText);
					ptText.begin(&pixText);
					ptText.setRenderHint(QPainter::Antialiasing,true);
					QPen pent = ptText.pen();
					QPen penText = pent;
					pent.setWidth(0);
					penText.setColor(QColor(255,255,255,255));
					ptText.setPen(pent);*/
					QString key = QString::number(rgb->fStart) + tr("-") + QString::number(rgb->fEnd);
					//ptText.drawText(0,10,key);
					//ptText.end();
					QGraphicsTextItem* itemText= new QGraphicsTextItem(key);
					itemText->setDefaultTextColor(Qt::white);
					int h = height - 5;
					itemText->setPos(35,h);
					gs->addItem(itemText);
				}
			}
			layerIter++;
		}
		gs->update();
	}
}


boost::any ViewManager::GetAttributeValue(int index ,goto_gis::Layer* layer)
{
	boost::any _value;
	Layer::P_LAYER_ATTRIBUTE_MAP pLayerAttribteTable = layer->GetLayerAttribteTable();
	Layer::LAYER_ATTRIBUTE_MAP::iterator findResult = pLayerAttribteTable->find(index);
	_value = findResult->second;
	return _value;
}



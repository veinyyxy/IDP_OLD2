#include <QDebug>
#include <QtOpenGL>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/SkyNode>

#include "Map.h"
#include "Layer.h"

#include "IsoSurface.h"

#include "AdapterWidget.h"
#include "ViewManager.h"
#include "RadarDataAccess.h"
#include "RadarControlDockWidget.h"
using namespace goto_gis;

#include "mainwindow.h"
#include "sysmainwindow.h"
RadarControlDockWidget::RadarControlDockWidget( QWidget *parent /*= 0*/ ): QDockWidget(parent)
{
	ui.setupUi(this);
	m_pMap = NULL;
	m_pMW = SysMainWindow::GetMainWindow();

	InitLeftDockWidget();
	InitSlots();




}


RadarControlDockWidget::~RadarControlDockWidget(void)
{
}

void RadarControlDockWidget::InitSlots()
{
	connect(ui.cmblayerID,SIGNAL(currentIndexChanged ( int )),this,SLOT(Slot_ChangLayer(int)));

	connect(ui.checkBox_isosurface1, SIGNAL(clicked(bool)), SLOT(Slot_IsoSurface1_Clicked1(bool)));
	connect(ui.checkBox_isosurface2, SIGNAL(clicked(bool)), SLOT(Slot_IsoSurface1_Clicked2(bool)));
	connect(ui.checkBox_section1, SIGNAL(clicked(bool)), SLOT(Slot_Section1_Clicked1(bool)));
	connect(ui.checkBox_section2, SIGNAL(clicked(bool)), SLOT(Slot_Section2_Clicked2(bool)));
	connect(ui.checkBox_section3, SIGNAL(clicked(bool)), SLOT(Slot_Section3_Clicked3(bool)));
	connect(ui.horizontalSlider_iso1,SIGNAL(sliderMoved ( int  )),SLOT(Slot_Slider1Moved(int)));
	connect(ui.horizontalSlider_iso2,SIGNAL(sliderMoved ( int  )),SLOT(Slot_Slider2Moved(int)));

}

void RadarControlDockWidget::InitLeftDockWidget()
{
	QString text = QString::number(0) +" DBz";
	ui.label_iso1_value->setText(text);
	ui.label_iso2_value->setText(text);

}

void RadarControlDockWidget::setVisible( bool visible )
{
	//if (visible)
	{
		QAction* action = (QAction*)this->property("Action").toLongLong();
		if (action)
		{
			action->setCheckable(visible);
			action->setChecked(visible);
		}
	}
	QDockWidget::setVisible(visible);
}



void RadarControlDockWidget::AddLayersToCmb(Map* map)
{
	m_pMap = map;

	ui.cmblayerID->clear();//ÏÈÇåÀí
	if (map)
	{
		int maxValue = 0;
		std::map<std::string, Layer*> mapLayers = map->GetMapLayers();
		std::map<std::string, Layer*>::iterator layerIter = mapLayers.begin();
		while(layerIter != mapLayers.end())
		{
			QString name = QString::fromStdString(layerIter->first);
			if (name.contains("Ñö½Ç²ã") && !name.contains("¾àÀëÈ¦"))
			{
				ui.cmblayerID->addItem(name);
				maxValue++;
			}
			layerIter++;
		}
	}
}

void RadarControlDockWidget::Slot_ChangLayer( int index )
{
	if (index == -1)
	{
		return;
	}

	if (m_pMap)
	{
		index = index+1;
		std::string layername = "µÚ" + QString::number(index).toStdString()+"Ñö½Ç²ã";
		std::map<std::string, Layer*> m_mapLayer = m_pMap->GetMapLayers();
		std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();

		QStringList blist = m_pMap->BasisLayerList();
		blist<<tr("¾àÀëÈ¦");
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
					m_pMap->SetVisibleLayer(layerIter->first,true);
				}
				else
				{
					m_pMap->SetVisibleLayer(layerIter->first,false);
				}
			}
			layerIter++;
		}
		m_pMW->Slot_ShowRaderAttributeInfo(m_pMW->FromWidgetToQGLW(),m_pMap->GetLayer(layername));//////////////////////////////////////////////////////////////////////////
	}
}


void RadarControlDockWidget::Slot_IsoSurface1_Clicked1( bool checked )
{
	static int iOldValue1 = 0;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	IsoSurface* pOEB = (IsoSurface*)m_pMap->GetExtendInterface(std::string("IsoSurface"));
	if (NULL == pOEB) return;

	int iValue = ui.horizontalSlider_iso1->value();
	if(checked && (iOldValue1 != iValue))
	{
		pOEB->DelIsoSuerface(iOldValue1);
		pOEB->ComputerIso(iValue);
		iOldValue1 = iValue;
	}
	else
	{
		pOEB->DelIsoSuerface(iOldValue1);
		iOldValue1 = 0;
	}
	QApplication::restoreOverrideCursor();
}

void RadarControlDockWidget::Slot_IsoSurface1_Clicked2( bool checked )
{
	static int iOldValue2 = 0;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	IsoSurface* pOEB = (IsoSurface*)m_pMap->GetExtendInterface(std::string("IsoSurface"));
	int iValue = ui.horizontalSlider_iso2->value();
	if(checked && (iOldValue2 != iValue))
	{
		pOEB->DelIsoSuerface(iOldValue2);
		pOEB->ComputerIso(iValue);
		iOldValue2 = iValue;
	}
	else
	{
		pOEB->DelIsoSuerface(iOldValue2);
		iOldValue2 = 0;
	}
	QApplication::restoreOverrideCursor();
}

//OsgEarthBridge* RadarControlDockWidget::GetOsgEarthBridge()
//{
//	OsgEarthBridge* pOEB = 0;
//	goto_gis::Map* pMap = m_pMW->GetCurrentMap();
//	if(!pMap) return 0;
//	ExtendMap* pExtenMap = dynamic_cast<ExtendMap*>(pMap);
//	if(pExtenMap)
//		pOEB = pExtenMap->GetOsgEarthBridge();
//
//	return pOEB;
//}

void RadarControlDockWidget::Slot_Section3_Clicked3( bool checked )
{
	RadarDataAccess* pRadarDataAccess = RadarDataAccess::Instance();

	goto_gis::Map* pMap = m_pMW->GetCurrentMap();

	if(checked)
		pRadarDataAccess->ClipPlane(pMap, 2);
	else
		pRadarDataAccess->DleClipPlane(pMap, 2);
}

void RadarControlDockWidget::Slot_Section2_Clicked2( bool checked )
{
	RadarDataAccess* pRadarDataAccess = RadarDataAccess::Instance();

	goto_gis::Map* pMap = m_pMW->GetCurrentMap();

	if(checked)
		pRadarDataAccess->ClipPlane(pMap, 1);
	else
		pRadarDataAccess->DleClipPlane(pMap, 1);
}

void RadarControlDockWidget::Slot_Section1_Clicked1( bool checked )
{
	RadarDataAccess* pRadarDataAccess = RadarDataAccess::Instance();

	goto_gis::Map* pMap = m_pMW->GetCurrentMap();
	if(NULL == pMap) return;
	if(checked)
		pRadarDataAccess->ClipPlane(pMap, 0);
	else
		pRadarDataAccess->DleClipPlane(pMap, 0);
}

void RadarControlDockWidget::Slot_Slider1Moved( int value )
{
	QString text = QString::number(value) +" DBz";
	ui.label_iso1_value->setText(text);
}

void RadarControlDockWidget::Slot_Slider2Moved( int value )
{
	QString text = QString::number(value) +" DBz";
	ui.label_iso2_value->setText(text);
}

void RadarControlDockWidget::GroupEnd( bool bEnd )
{
	ui.groupBox_2->setEnabled(bEnd);
}

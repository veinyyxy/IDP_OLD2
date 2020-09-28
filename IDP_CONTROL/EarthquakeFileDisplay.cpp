#include "DataProvider.h"
#include "CoordinateTransform.h"
#include "VectorLayer.h"
#include "Map.h"


#include "AdapterWidget.h"
#include "osg/Hint"
#include <QtCore/QObjectUserData>
#include <osgEarth/MapNode>
#include <osgEarth/SpatialReference>
#include <osgEarthUtil/EarthManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include "OsgEarthTransform.h"
#include "EarthQuakeSympolLibrary.h"
#include "DrawRect.h"
#include "DrawGeometryManipulator.h"
#include "ManualDrawLayer.h"
#include "EarthquakeFileDisplay.h"
#include "IDPdb.h"

EarthquakeFileDisplay::EarthquakeFileDisplay(ViewerQT* pvewer) : m_pMap((goto_gis::Map*)pvewer->property("Map").toLongLong())
{
	m_pViewer = pvewer;
}


EarthquakeFileDisplay::~EarthquakeFileDisplay(void)
{
}

QWidget* EarthquakeFileDisplay::CreateView()
{
	m_pViewer= new ViewerQT(0);
	m_pViewer->getViewer()->setLightingMode(osg::View::NO_LIGHT);
	QWidget* pGLWidget = m_pViewer;
	pGLWidget->setUserData(0, this);
	return pGLWidget;
}

Layer* EarthquakeFileDisplay::ReadEarthquakeFileToLayer(const QString& fileName, goto_gis::CoordinateTransform* ct)
{
	goto_gis::DataProvider* pDataRovider;
	typedef DataProvider* (*DataProviderPluginMain)();
	QLibrary myLib("EarthquakeDataProvider.dll");
	DataProviderPluginMain GetRadarMosaicDataProvider = (DataProviderPluginMain) myLib.resolve("DataProviderPluginMain");

	pDataRovider = GetRadarMosaicDataProvider();

	goto_gis::Layer* pLayer = new goto_gis::VectorLayer(pDataRovider, ct, 1);
	///////////////////////////////////ÉèÖÃÍ¼²ãµÄ·ûºÅ¿âÊµÏÖ///////////////////////////////////////
	EarthQuakeSympolLibrary* pEQSL = new EarthQuakeSympolLibrary;
	pLayer->SetSymbolLibrary(pEQSL);

	pDataRovider->ParentLayer(pLayer);
	pDataRovider->LoadFile(fileName.toStdString());
	
	return pLayer;
}

void EarthquakeFileDisplay::LoadBasicGeogInfo( const QString& configName )
{
	m_pMap->Load2DMapFromConfig(configName.toStdString());
	//m_MapManager.AddFishNet(70,55,140,0,5,5,0.5);
}

void EarthquakeFileDisplay::OpenEarthquakeFile( const QString& mapConfig, const QString& fileName )
{
	//LoadBasicGeogInfo(mapConfig);
	m_pMap->AddFishNet(70,55,140,0,5,5,0.5);
	goto_gis::Layer* pEarthauakeLayer = ReadEarthquakeFileToLayer(fileName, m_pMap->GetCoordinateTransform());
	pEarthauakeLayer->LayerZ(.7f);
	m_pMap->AddLayer(fileName.toStdString(), pEarthauakeLayer);
	m_pMap->CurrentLayer(pEarthauakeLayer);
	//pEarthauakeLayer->CreateLayerData();
	m_pMap->CreateMapData();

	//m_pMap->TramsformMap();
	osg::Node* pRootNode = m_pMap->GetRootNode();
	///////////////////////////////////Ìí¼Ó¿¹¾â³Ý///////////////////////////////////////
	//osg::StateSet* stateSet = pRootNode->getOrCreateStateSet();
	//osg::ref_ptr <osg::LineWidth>liwidth = new osg::LineWidth;
	//osg::ref_ptr<osg::Hint> linehint = new osg::Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//liwidth->setWidth(10.0);     
	//stateSet->setAttributeAndModes(liwidth, osg::StateAttribute::ON);
	//stateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
	//stateSet->setAttribute(linehint);
	///////////////////////////////////////////////////////////////////////////////////////
	m_pViewer->getViewer()->setSceneData(pRootNode);
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keySwitchMaipulator = new osgGA::KeySwitchMatrixManipulator;
	//osg::ref_ptr<DrawGeometryManipulator> DrawMaipulator = new DrawGeometryManipulator(0);
	/////////////////////////////////////Ìí¼ÓÊÖ»æÍ¼ÐÎ///////////////////////////////////////
	//DrawMaipulator->SetDrawGeometryData(new DrawRect);
	//osg::Node* pDrawNode = DrawMaipulator->CreateCanvas(100000000, 100000000, osg::Vec4(0, 0, 0, 1));
	//osg::Group* pGroup = dynamic_cast<osg::Group*>(pRootNode);
	//pGroup->addChild(pDrawNode);
	//pGroup->addChild(DrawMaipulator->GetDrawGeometryData()->GetDrawNode());

	//goto_gis::ManualDrawLayer* pLayer = new goto_gis::ManualDrawLayer;
	//pLayer->LayerZ(100);

	//DrawMaipulator->GetDrawGeometryData()->SetDrawLayer(pLayer);
	//DrawMaipulator->GetDrawGeometryData()->SetColor(osg::Vec4(0, 1, 0, 1));
	//m_pMap.AddLayer("DrawLayer", pLayer);
	///////////////////////////////////////////////////////////////////////////////////////
	keySwitchMaipulator->addMatrixManipulator('1', "1", new DefaultManipulator);
	DefaultManipulator *dm = new DefaultManipulator;
	//dm->CoordTrans(m_pMap->GetCoordinateTransform());
	keySwitchMaipulator->addMatrixManipulator('2', "2", new osgGA::TrackballManipulator);
//	keySwitchMaipulator->addMatrixManipulator('3', "3", DrawMaipulator);

	m_pViewer->getViewer()->setCameraManipulator(keySwitchMaipulator);
	m_pViewer->getViewer()->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	//pAdapterWidget->getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EarthquakeFileDisplay::OpenEarthquakeFile(const QString& fileName )
{
	goto_gis::Layer* pEarthauakeLayer = ReadEarthquakeFileToLayer(fileName, m_pMap->GetCoordinateTransform());
	pEarthauakeLayer->LayerZ(.7f);
	pEarthauakeLayer->CreateLayerData();
	m_pMap->AddLayer(fileName.toStdString(), pEarthauakeLayer);

}

Layer* EarthquakeFileDisplay::ReadTIFFFileToLayer( const QString& fileName, goto_gis::CoordinateTransform* ct )
{
	goto_gis::DataProvider* pDataRovider;
	typedef DataProvider* (*DataProviderPluginMain)();
	QLibrary myLib("TIFFProvider.dll");
	DataProviderPluginMain GetTiffProvider = (DataProviderPluginMain) myLib.resolve("DataProviderPluginMain");

	pDataRovider = GetTiffProvider();

	goto_gis::Layer* pLayer = new goto_gis::VectorLayer(pDataRovider, ct, 1);
	pLayer->LayerZ(.3);
	pDataRovider->ParentLayer(pLayer);
	pDataRovider->LoadFile(fileName.toStdString());

	return pLayer;
}

void EarthquakeFileDisplay::OpenTIFFFile( const QString& fileName,const QString& mapConfig /*= QString()*/ )
{
	m_pMap->AddFishNet(70,55,135,0,5,5,0.5);
	goto_gis::Layer* pEarthauakeLayer = ReadTIFFFileToLayer(fileName, m_pMap->GetCoordinateTransform());
	pEarthauakeLayer->LayerZ(0.4f);
	pEarthauakeLayer->UseLayerZ(true);
	m_pMap->AddLayer(fileName.toStdString(), pEarthauakeLayer);
	m_pMap->CurrentLayer(pEarthauakeLayer);
	pEarthauakeLayer->CreateLayerData();
	//m_pMap->CreateMapData();

	m_pMap->TramsformMap();
	osg::Node* pRootNode = m_pMap->GetRootNode();
	m_pViewer->getViewer()->setSceneData(pRootNode);
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keySwitchMaipulator = new osgGA::KeySwitchMatrixManipulator;

	keySwitchMaipulator->addMatrixManipulator('1', "1", new DefaultManipulator);
	DefaultManipulator *dm = new DefaultManipulator;
	//dm->CoordTrans(m_pMap->GetCoordinateTransform());
	keySwitchMaipulator->addMatrixManipulator('2', "2", new osgGA::TrackballManipulator);
	m_pViewer->getViewer()->setCameraManipulator(keySwitchMaipulator);
	m_pViewer->getViewer()->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
}


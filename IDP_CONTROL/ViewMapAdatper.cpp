#include "ViewMapAdatper.h"
#include "GeosOnTheOsg.h"
#include "MapFactory.h"
#include <osgViewer/Viewer>

//ViewMapAdatper::ViewMapAdatper(void)
//{
//	m_pView = new osgViewer::Viewer;
//	MapFactory* pMapFactory 
//		= goto_gis::GeosOnTheOsg::Instantiate()->GetMapFactory();
//	m_pGisMap = pMapFactory->CreateMap();
//}

ViewMapAdatper::ViewMapAdatper( osg::View* pV, goto_gis::Map* pMap )
{
	m_pView = pV;
	m_pGisMap = pMap;
	UnitedViewAndMap();
}


ViewMapAdatper::~ViewMapAdatper(void)
{
}

void* ViewMapAdatper::GetUserData(std::string& strName)
{
	std::map<std::string, void*>::iterator iFined = m_UserData.find(strName);
	
	if(iFined != m_UserData.end())
	{
		return iFined->second;
	}
	else
		return 0;
}

void ViewMapAdatper::SetUserData( std::string& strName, void* pData )
{
	m_UserData[strName] = pData;
}

void ViewMapAdatper::DelUserData( std::string& strName )
{
	m_UserData.erase(strName);
}

void ViewMapAdatper::UnitedViewAndMap()
{
	if(m_pView != 0 && m_pGisMap != 0)
	{
		osgViewer::Viewer* pViewer 
			= dynamic_cast<osgViewer::Viewer*>(m_pView.get());
		pViewer->setSceneData(m_pGisMap->GetRootNode());
		//pViewer->setUserValue< goto_gis::Map* >("GIS_MAP", m_pGisMap);
	}
}

goto_gis::Layer* ViewMapAdatper::LoadDataToLayer( std::string& strFileName )
{
	goto_gis::Layer* pLayer = new goto_gis::VectorLayer();
	goto_gis::GeosOnTheOsg::Instantiate()->OpenDataAsLayer(strFileName, pLayer);
	if(m_pGisMap)
	{
		m_pGisMap->AddLayer(pLayer->GetLayerName(), pLayer);
	}
	return pLayer;
}


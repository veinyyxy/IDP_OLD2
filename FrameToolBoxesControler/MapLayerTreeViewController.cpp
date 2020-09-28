#include "StdAfx.h"
#include "Map.h"
#include "MapLayerTreeViewController.h"
#include "TreeViewEventImps.h"

MapLayerTreeViewController::MapLayerTreeViewController(void)
{
	m_pTreeViewEvent = new TreeViewEventImps;

	boost::any any;
	any = m_pTreeViewEvent;
	boost::any anyType = std::string("TreeModel");
	m_SwapModel.SetInterface(std::string("TreeViewEventModel"), &any);
	m_SwapModel.SetParameter("type", &anyType);
}


MapLayerTreeViewController::~MapLayerTreeViewController(void)
{
}

int MapLayerTreeViewController::PreExcute()
{
	return -1;
}

int MapLayerTreeViewController::Excute()
{
	return -1;
}

int MapLayerTreeViewController::PostExcute()
{
	return -1;
}

void MapLayerTreeViewController::GetName( std::string& strID )
{
	
}

void MapLayerTreeViewController::Input( const FrameModel* pFM )
{
	
}

FrameModel* MapLayerTreeViewController::Output()
{
	return &m_SwapModel;
}

#include "StdAfx.h"
#include "FrameToolBoxesContrlerManager.h"
#include "OpenDisplayWindowController.h"
#include "MapLayerTreeViewController.h"

FrameToolBoxesContrlerManager::FrameToolBoxesContrlerManager(void)
{
	m_pOpenDisplayWindowController = new OpenDisplayWindowController;
	m_pDockLayerTreeController = new MapLayerTreeViewController;
}


FrameToolBoxesContrlerManager::~FrameToolBoxesContrlerManager(void)
{
}

Controller* FrameToolBoxesContrlerManager::GetControllerFromName( const std::string& strName )
{
	if(strName == "OpenData")
		return m_pOpenDisplayWindowController;
	if(strName == "DockLayerTreeController")
		return m_pDockLayerTreeController;
	else
		return 0;
}

void FrameToolBoxesContrlerManager::GetControllerVector( std::vector<Controller*>* pOutVector)
{
	
}

void FrameToolBoxesContrlerManager::GetControllerNameVector( std::vector<std::string>* pOutVector)
{
	
}

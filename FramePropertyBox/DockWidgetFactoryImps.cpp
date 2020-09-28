#include "StdAfx.h"
#include "DockWidgetFactoryImps.h"
#include "LayerTreeDockWidget.h"
#include "LayerAttribteDockWidget.h"

DockWidgetFactoryImps::DockWidgetFactoryImps(void)
{
	m_pDockWidgetInterface = new LayerTreeDockWidget;
	m_pDockLayerAttribute = new LayerAttribteDockWidget;
}


DockWidgetFactoryImps::~DockWidgetFactoryImps(void)
{
}

DockWidgetInterface* DockWidgetFactoryImps::GetDockWidgetInterface( const std::string& name )
{
	if(name == "DockLayerTree")
	{
		return m_pDockWidgetInterface;
	}
	if(name == "DockLayerAttribute")
	{
		return m_pDockLayerAttribute;
	}
	else
		return 0;
}

int DockWidgetFactoryImps::GetDockWidgetCount()
{
	return 3;
}

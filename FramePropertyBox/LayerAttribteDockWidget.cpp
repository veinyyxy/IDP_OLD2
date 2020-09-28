#include "StdAfx.h"
#include "LayerAttribteDockWidget.h"

extern goto_gis::Map* GetCurentMap();

LayerAttribteDockWidget::LayerAttribteDockWidget(void)
{
	m_pAttributeDockWidget = new AttributeDockWidget;
}


LayerAttribteDockWidget::~LayerAttribteDockWidget(void)
{
}

QDockWidget* LayerAttribteDockWidget::GetDockWidget()
{
	return m_pAttributeDockWidget;
}

Qt::DockWidgetArea LayerAttribteDockWidget::GetDockArea()
{
	return Qt::LeftDockWidgetArea;
}

void LayerAttribteDockWidget::SetController( Controller* pFM, const QDomNode& discribNode )
{
	
}

void LayerAttribteDockWidget::AddContextMenuAction( const QString& strName, MyActionInterface* pAction )
{
	
}

void LayerAttribteDockWidget::DelContextMenuAction( const QString& strName )
{
	
}

MyActionInterface* LayerAttribteDockWidget::GetContextMenuAction( const QString& strName )
{
	return 0;
}

void LayerAttribteDockWidget::AddToolsBarAction( const QString& strName, MyActionInterface* pAction )
{
	
}

void LayerAttribteDockWidget::DelToolsBarAction( const QString& strName )
{
	
}

MyActionInterface* LayerAttribteDockWidget::GetToolsBarAction( const QString& strName )
{
	return 0;
}

void LayerAttribteDockWidget::UpdateSlave()
{
	
}

void LayerAttribteDockWidget::Update()
{

}

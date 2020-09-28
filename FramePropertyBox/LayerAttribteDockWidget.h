#pragma once
#include "DockWidgetInterface.h"
#include "AttributeDockWidget.h"
#include "UpdateSlaveInterface.h"

class LayerAttribteDockWidget :
	public DockWidgetInterface
{
public:
	LayerAttribteDockWidget(void);
	~LayerAttribteDockWidget(void);

	virtual QDockWidget* GetDockWidget();

	virtual Qt::DockWidgetArea GetDockArea();

	virtual void SetController( Controller* pFM, const QDomNode& discribNode );

	virtual void AddContextMenuAction( const QString& strName, MyActionInterface* pAction );

	virtual void DelContextMenuAction( const QString& strName );

	virtual MyActionInterface* GetContextMenuAction( const QString& strName );

	virtual void AddToolsBarAction( const QString& strName, MyActionInterface* pAction );

	virtual void DelToolsBarAction( const QString& strName );

	virtual MyActionInterface* GetToolsBarAction( const QString& strName );

	virtual void UpdateSlave();
	virtual void Update();
private:
	AttributeDockWidget* m_pAttributeDockWidget;
};


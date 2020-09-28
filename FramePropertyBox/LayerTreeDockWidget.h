#pragma once
#include "DockWidgetInterface.h"
#include "LeftDockWidget.h"
#include "UpdateMasterInterface.h"
#include "UpdateSlaveInterface.h"

class LayerTreeDockWidget :
	public DockWidgetInterface
{
public:
	typedef QMap<QString, MyActionInterface*> ACTION_MAP;
	
	LayerTreeDockWidget(void);
	~LayerTreeDockWidget(void);

	virtual QDockWidget* GetDockWidget();

	virtual Qt::DockWidgetArea GetDockArea();

	virtual void SetController( Controller* pFM, const QDomNode& discribNode );

	virtual void AddContextMenuAction(const QString& strName, MyActionInterface* pAction);
	virtual void DelContextMenuAction(const QString& strName);
	virtual MyActionInterface* GetContextMenuAction(const QString& strName);
	virtual void AddToolsBarAction(const QString& strName, MyActionInterface* pAction);
	virtual void DelToolsBarAction(const QString& strName);
	virtual MyActionInterface* GetToolsBarAction(const QString& strName);

	virtual void UpdateSlave();
	virtual void Update();
private:
	LeftDockWidget* m_pLeftDockWidget;
	ACTION_MAP m_ActionsMap;
	TreeViewEventInterface* m_TreeViewEventInterface;
};


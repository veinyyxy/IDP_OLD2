#pragma once
#include <QtGui/QDockWidget>
#include <QtXml/QDomNode>
#include "UpdateMasterInterface.h"
#include "UpdateSlaveInterface.h"

class Controller;
class MyActionInterface;

class DockWidgetInterface : public UpdateMasterInterface, public UpdateSlaveInterface
{
public:
	virtual QDockWidget* GetDockWidget() = 0;
	virtual Qt::DockWidgetArea GetDockArea() = 0;
	virtual void SetController(Controller* pFM, const QDomNode& discribNode) = 0;
	
	virtual void AddContextMenuAction(const QString& strName, MyActionInterface* pAction) = 0;
	virtual void DelContextMenuAction(const QString& strName) = 0;
	virtual MyActionInterface* GetContextMenuAction(const QString& strName) = 0;
	virtual void AddToolsBarAction(const QString& strName, MyActionInterface* pAction) = 0;
	virtual void DelToolsBarAction(const QString& strName) = 0;
	virtual MyActionInterface* GetToolsBarAction(const QString& strName) = 0;
	virtual void Update() = 0;
};


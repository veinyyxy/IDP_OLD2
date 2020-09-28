#pragma once
#include <QtGui/QtGui>

class MyActionInterface;

class ToolBoxInterface
{
public:
	virtual QWidget* GetToolTableWidget() = 0;
	virtual QString GetToolTableName() = 0;
	virtual MyActionInterface* GetActionFromName(const QString& name) = 0;
	virtual void AddAction(const QString& name, MyActionInterface* pAction) = 0;
	virtual void RemoveAction(const QString& name) = 0;
};


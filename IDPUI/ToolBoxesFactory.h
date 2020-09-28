#pragma once
#include <QtCore/QString>

class ToolBoxInterface;

class ToolBoxesFactory
{
public:
	virtual ToolBoxInterface* GetToolBoxesInterface(const QString& name) = 0;
	virtual int GetToolBoxCount() = 0;
};


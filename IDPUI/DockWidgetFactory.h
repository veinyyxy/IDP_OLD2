#pragma once
#include <string>
class DockWidgetInterface;

class DockWidgetFactory
{
public:
	virtual DockWidgetInterface* GetDockWidgetInterface(
		const std::string& name) = 0;
	virtual int GetDockWidgetCount() = 0;
};


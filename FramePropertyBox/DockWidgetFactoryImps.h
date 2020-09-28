#pragma once
#include "DockWidgetFactory.h"

class DockWidgetFactoryImps :
	public DockWidgetFactory
{
public:
	DockWidgetFactoryImps(void);
	~DockWidgetFactoryImps(void);

	virtual DockWidgetInterface* GetDockWidgetInterface( const std::string& name );

	virtual int GetDockWidgetCount();
private:
	DockWidgetInterface* m_pDockWidgetInterface;
	DockWidgetInterface* m_pDockLayerAttribute;
};


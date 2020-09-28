#pragma once
#include "displayviewfactory.h"
class DefaultDisplayViewFactory :
	public DisplayViewFactory
{
public:
	DefaultDisplayViewFactory(void);
	~DefaultDisplayViewFactory(void);

	virtual QWidget* GetDisplayView( osg::View* pView, osg::Camera* pCamera );

	virtual int GetDisplayViewType( QWidget* pWidget );

	virtual osg::View* GetWidgetsView();

	virtual osg::Camera* GetWidgetsCamera();

};


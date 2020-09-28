#pragma once
#include <QtGui/QWidget>
#include <osg/View>
#include <osg/Camera>

class DisplayViewFactory
{
public:
	virtual QWidget* GetDisplayView(osg::View* pView, osg::Camera* pCamera) = 0;
	virtual int GetDisplayViewType(QWidget* pWidget) = 0;
	virtual osg::View* GetWidgetsView() = 0;
	virtual osg::Camera* GetWidgetsCamera() = 0;
};

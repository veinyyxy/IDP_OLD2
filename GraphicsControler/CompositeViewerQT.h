#ifndef COMPOSITEVIEWERQT_H
#define COMPOSITEVIEWERQT_H

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgEarthUtil/SkyNode>
#include <QtGui/QtGui>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMdiArea>
#include "DefaultManipulator.h"
using Qt::WindowFlags;

#include <iostream>
#include "GraphicsControler_global.h"
#include "RenderThread.h"

class GRAPHICSCONTROLER_EXPORT CompositeViewerQT : public QWidget, public osgViewer::CompositeViewer
{
	Q_OBJECT

public:
	CompositeViewerQT(int index = 0 ,QWidget* parent = 0);
	~CompositeViewerQT();
	void AddMenuItem();
	void CurrentWidget(QWidget* cw);
	QWidget* CurrentWidget();
	QWidget* AddViewWidget( osg::Camera* camera, osg::Node* scene );
	osg::Camera* CreateCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );
private:
	virtual void paintEvent( QPaintEvent* event );
	virtual void closeEvent( QCloseEvent *event );
	virtual bool winEvent(MSG *m, long *result);
Q_SIGNALS:
	void SetCurrentWidget(QWidget* currentWidget);
public Q_SLOTS:
	virtual void setVisible(bool visible);
		void Slot_Notify();
protected:
	DefaultManipulator* _dm;
	RenderThread mThreadHandle;
	QWidget* m_CurrentWidget;
};


#endif // COMPOSITEVIEWERQT_H
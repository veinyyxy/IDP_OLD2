#pragma once
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <QtGui/QtGui>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMdiArea>
#include <osgQt/GraphicsWindowQt>
#include "PosterPrinter.h"
#include "PrintPosterHandler.h"

#include "DefaultManipulator.h"
using Qt::WindowFlags;

#include <iostream>
#include "GraphicsControler_global.h"
#include "OSGGLWidget.h"
#include "RenderThread.h"

class osg::Node;
class PrintImageToQImage;

class QGLWidgetEventCallback
{
public:
	virtual void keyPressEvent( QKeyEvent* event ) = 0;
	virtual void keyReleaseEvent( QKeyEvent* event ) = 0;
	virtual void mousePressEvent( QMouseEvent* event ) = 0;
	virtual void mouseReleaseEvent( QMouseEvent* event ) = 0;
	virtual void mouseMoveEvent( QMouseEvent* event ) = 0;
	virtual void wheelEvent(QWheelEvent * event) = 0;
};

class DeleteData : public osg::Referenced
{
public:
	virtual void DoDelete() = 0;
};

class GRAPHICSCONTROLER_EXPORT ViewerQT : public QWidget
{
	

	Q_OBJECT

public:
	ViewerQT(osgViewer::Viewer* pVbn = 0, QWidget * parent = 0, QGLWidgetEventCallback* pEventCallback = 0);
	virtual ~ViewerQT();
	inline osgViewer::GraphicsWindow* getGraphicsWindow() { return _gw.get(); }
	inline const osgViewer::GraphicsWindow* getGraphicsWindow() const { return _gw.get(); }
	inline osgViewer::Viewer* getViewer(){return _pViewer.get();}
	void setViewer(osgViewer::Viewer* pVb);
	inline QGLWidgetEventCallback* GetEventCallback(){return _pEventCallBack;}
	inline void setEventCallback(QGLWidgetEventCallback* pECB){_pEventCallBack = pECB;}
	inline void SetDeleteCallback(DeleteData* pDD){m_pDeleteData = pDD;}
	void SetImageSize(float fWidth, float fHeight);
	void SendGiveMeImageSignal();

	LONGLONG getMapScale(int iType);
Q_SIGNALS:
	void SetCurrentWidget(QWidget* currentWidget);
	void PrintImage(osg::Image* pImage);
	void toUpdate();

public Q_SLOTS:
		virtual void setVisible(bool visible);
		void Slot_Notify();
		void GiveYouFuckImage(float fWidth, float fHeight);
public:
	//void init();
	//virtual void paintGL();
	//virtual void resizeGL( int width, int height );
	virtual void paintEvent( QPaintEvent* event );
	virtual bool winEvent(MSG *m, long *result);
	virtual void resizeEvent( QResizeEvent* event );
	virtual void closeEvent( QCloseEvent *event );

	virtual void keyPressEvent( QKeyEvent* event );
	virtual void keyReleaseEvent( QKeyEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseReleaseEvent( QMouseEvent* event );
	virtual void mouseMoveEvent( QMouseEvent* event );
	virtual void wheelEvent(QWheelEvent * event);
	
	void setKeyboardModifiers( QInputEvent* event );
	void initOsgPoster(osg::Node*);
	void SaveImage();
	void AddMenuItem();
private:
	osg::ref_ptr<osgViewer::GraphicsWindow> _gw;
	osgQt::GLWidget* _glwidget; 
	
	RenderThread mThreadHandle;
	osg::ref_ptr<osgViewer::Viewer> _pViewer;
	QGLWidgetEventCallback* _pEventCallBack;


	//////////////////////////////////////////////////////////////////////////
	
	osg::ref_ptr<osg::Camera> m_Camera;
	osg::ref_ptr<PosterPrinter> m_PosterPrinter;
	osg::ref_ptr<PrintPosterHandler> m_PrintPosterHandler;
	osg::ref_ptr<osg::Image> m_osgImage;

	I_PrintImage* p_PrintImage;
	osg::ref_ptr<DeleteData> m_pDeleteData;
};


class  PrintImageToQImage : public I_PrintImage
{
public:
	PrintImageToQImage(ViewerQT* pMyViewer = 0) : g_ViewrQt(pMyViewer){}
	virtual void printImage( osg::Image* pImage )
	{
		if(g_ViewrQt) g_ViewrQt->SaveImage();
	}
	ViewerQT* g_ViewrQt;
};
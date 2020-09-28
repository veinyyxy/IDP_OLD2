#include <osgQt/GraphicsWindowQt>
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <OsgEarth/ModelLayer>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include "CompositeViewerQT.h"

#define IDM_INMDI_E 0x0013


class CompositeViewerQTViewClicked:public osgGA::GUIEventHandler
{
public:
	CompositeViewerQTViewClicked(osg::ref_ptr<osgViewer::CompositeViewer>compositeViewer,osg::ref_ptr<osgViewer::View>view):m_compositeviewer(compositeViewer),m_view(view){}
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&aa )
	{
		switch(ea.getEventType())
		{
		case osgGA::GUIEventAdapter::RELEASE:
			{
				osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( m_view->getCamera()->getGraphicsContext() );
				if (gw)
				{
					dynamic_cast<CompositeViewerQT*>(m_compositeviewer.get())->CurrentWidget(gw ? gw->getGLWidget() : NULL);
				}
			}
			break;
		default:
			break;
		}
		return osgGA::GUIEventHandler::handle(ea,aa);
	}
public:
	osg::ref_ptr<osgViewer::CompositeViewer>m_compositeviewer;
	osg::ref_ptr<osgViewer::View>m_view;
};

CompositeViewerQT::CompositeViewerQT(int index ,QWidget* parent) : QWidget(parent)
{
	setThreadingModel(SingleThreaded);
	_dm  =new DefaultManipulator;
	setObjectName("CompositeViewerQT");

	connect(&mThreadHandle, SIGNAL(notify()), this, SLOT(Slot_Notify()));
	mThreadHandle.startRender();
}


void CompositeViewerQT::AddMenuItem()
{
	QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
	if (subW)
	{
		HMENU hMenu = ::GetSystemMenu(winId(), FALSE);
		if (hMenu != NULL)
		{
			::AppendMenuA(hMenu, MF_SEPARATOR, 0, 0);
			::AppendMenuA(hMenu, MF_STRING, IDM_INMDI_E, "窗口锁定");
		}
	}
}

void CompositeViewerQT::setVisible( bool visible )
{
	if (visible)
	{
		AddMenuItem();
	}
	QWidget::setVisible(visible);
}

void CompositeViewerQT::paintEvent( QPaintEvent* event )
{ 
	frame(); 
}

bool CompositeViewerQT::winEvent(MSG *m, long *result)
{
	if (m->message == WM_SYSCOMMAND)
	{
		if ((m->wParam) == IDM_INMDI_E)
		{
			*result = 0;
			QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
			if (subW)
			{
				this->setWindowFlags(Qt::Widget);
				subW->setWidget(this);
				subW->showMaximized();
			}
			return (true);
		}
	}
	return (false);
}

void CompositeViewerQT::closeEvent( QCloseEvent *event )
{
	int ret = QMessageBox::information(0,tr("提示"),tr("正在运行！\n是否确认关闭当前视口？"),QMessageBox::Ok | QMessageBox::Cancel);
	if (ret == 1024)
	{
		QMdiSubWindow* pw = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
		if(pw)
		{
			pw->close();
		}
	}
	else
	{
		QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
		if (subW)
		{
			this->setWindowFlags(Qt::Widget);
			subW->setWidget(this);
			subW->showMaximized();
		}
		event->ignore();
	}
}

QWidget* CompositeViewerQT::AddViewWidget( osg::Camera* camera, osg::Node* scene )
{
	osgViewer::Viewer* view = new osgViewer::Viewer;
	view->setCamera( camera );
	addView( view );
	//view->setLightingMode(osg::View::NO_LIGHT);
	
	//添加事件处理
	view->setThreadingModel(osgViewer::Viewer::SingleThreaded);
	view->setSceneData( scene );
	view->realize();
	view->addEventHandler(new osgViewer::StatsHandler);
	view->addEventHandler(new osgViewer::WindowSizeHandler);
	view->addEventHandler(new osgGA::GUIEventHandler);

	view->addEventHandler(new CompositeViewerQTViewClicked(this,view));
	view->setCameraManipulator( _dm );
	osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
	QWidget* cw = gw ? gw->getGLWidget() : NULL;
	if (cw)
	{
		cw->setProperty("Viewer",(long long)view);
	}
	return cw;
}

osg::Camera* CompositeViewerQT::CreateCamera( int x, int y, int w, int h, const std::string& name/*=""*/, bool windowDecoration/*=false */ )
{
	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowName = name;
	traits->windowDecoration = windowDecoration;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;
	traits->alpha = ds->getMinimumNumAlphaBits();
	traits->stencil = ds->getMinimumNumStencilBits();
	traits->sampleBuffers = ds->getMultiSamples();
	traits->samples = ds->getNumMultiSamples();

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );

	//camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
	camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
	//camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
	camera->setProjectionMatrixAsOrtho(0, traits->width, 0, traits->height, -1000000, 1000000);
	return camera.release();
}

void CompositeViewerQT::CurrentWidget( QWidget* cw )
{
	this->setProperty("Map",cw->property("Map"));
	m_CurrentWidget = cw;
	emit SetCurrentWidget(cw);
}

QWidget* CompositeViewerQT::CurrentWidget()
{
	return m_CurrentWidget;
}

void CompositeViewerQT::Slot_Notify()
{
	try
	{
		this->update();
	}
	catch (...)
	{

	}
}

CompositeViewerQT::~CompositeViewerQT()
{
	mThreadHandle.stopRender();
	mThreadHandle.wait();
}

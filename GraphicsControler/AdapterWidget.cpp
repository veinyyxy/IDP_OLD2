#include <QtGui/QtGui>
#include <QtOpenGL/QGLWidget>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/api/win32/GraphicsWindowWin32>

#include <osg/CullFace>
#include <osgDB/WriteFile>

#include "../include/IDPApplication.h"

#include "AdapterWidget.h"

class QtKeyboardMap
{

public:
	QtKeyboardMap()
	{
		mKeyMap[Qt::Key_Escape     ] = osgGA::GUIEventAdapter::KEY_Escape;
		mKeyMap[Qt::Key_Delete   ] = osgGA::GUIEventAdapter::KEY_Delete;
		mKeyMap[Qt::Key_Home       ] = osgGA::GUIEventAdapter::KEY_Home;
		mKeyMap[Qt::Key_Enter      ] = osgGA::GUIEventAdapter::KEY_KP_Enter;
		mKeyMap[Qt::Key_End        ] = osgGA::GUIEventAdapter::KEY_End;
		mKeyMap[Qt::Key_Return     ] = osgGA::GUIEventAdapter::KEY_Return;
		mKeyMap[Qt::Key_PageUp     ] = osgGA::GUIEventAdapter::KEY_Page_Up;
		mKeyMap[Qt::Key_PageDown   ] = osgGA::GUIEventAdapter::KEY_Page_Down;
		mKeyMap[Qt::Key_Left       ] = osgGA::GUIEventAdapter::KEY_Left;
		mKeyMap[Qt::Key_Right      ] = osgGA::GUIEventAdapter::KEY_Right;
		mKeyMap[Qt::Key_Up         ] = osgGA::GUIEventAdapter::KEY_Up;
		mKeyMap[Qt::Key_Down       ] = osgGA::GUIEventAdapter::KEY_Down;
		mKeyMap[Qt::Key_Backspace  ] = osgGA::GUIEventAdapter::KEY_BackSpace;
		mKeyMap[Qt::Key_Tab        ] = osgGA::GUIEventAdapter::KEY_Tab;
		mKeyMap[Qt::Key_Space      ] = osgGA::GUIEventAdapter::KEY_Space;
		mKeyMap[Qt::Key_Delete     ] = osgGA::GUIEventAdapter::KEY_Delete;
		mKeyMap[Qt::Key_Alt      ] = osgGA::GUIEventAdapter::KEY_Alt_L;
		mKeyMap[Qt::Key_Shift    ] = osgGA::GUIEventAdapter::KEY_Shift_L;
		mKeyMap[Qt::Key_Control  ] = osgGA::GUIEventAdapter::KEY_Control_L;
		mKeyMap[Qt::Key_Meta     ] = osgGA::GUIEventAdapter::KEY_Meta_L;

		mKeyMap[Qt::Key_F1             ] = osgGA::GUIEventAdapter::KEY_F1;
		mKeyMap[Qt::Key_F2             ] = osgGA::GUIEventAdapter::KEY_F2;
		mKeyMap[Qt::Key_F3             ] = osgGA::GUIEventAdapter::KEY_F3;
		mKeyMap[Qt::Key_F4             ] = osgGA::GUIEventAdapter::KEY_F4;
		mKeyMap[Qt::Key_F5             ] = osgGA::GUIEventAdapter::KEY_F5;
		mKeyMap[Qt::Key_F6             ] = osgGA::GUIEventAdapter::KEY_F6;
		mKeyMap[Qt::Key_F7             ] = osgGA::GUIEventAdapter::KEY_F7;
		mKeyMap[Qt::Key_F8             ] = osgGA::GUIEventAdapter::KEY_F8;
		mKeyMap[Qt::Key_F9             ] = osgGA::GUIEventAdapter::KEY_F9;
		mKeyMap[Qt::Key_F10            ] = osgGA::GUIEventAdapter::KEY_F10;
		mKeyMap[Qt::Key_F11            ] = osgGA::GUIEventAdapter::KEY_F11;
		mKeyMap[Qt::Key_F12            ] = osgGA::GUIEventAdapter::KEY_F12;
		mKeyMap[Qt::Key_F13            ] = osgGA::GUIEventAdapter::KEY_F13;
		mKeyMap[Qt::Key_F14            ] = osgGA::GUIEventAdapter::KEY_F14;
		mKeyMap[Qt::Key_F15            ] = osgGA::GUIEventAdapter::KEY_F15;
		mKeyMap[Qt::Key_F16            ] = osgGA::GUIEventAdapter::KEY_F16;
		mKeyMap[Qt::Key_F17            ] = osgGA::GUIEventAdapter::KEY_F17;
		mKeyMap[Qt::Key_F18            ] = osgGA::GUIEventAdapter::KEY_F18;
		mKeyMap[Qt::Key_F19            ] = osgGA::GUIEventAdapter::KEY_F19;
		mKeyMap[Qt::Key_F20            ] = osgGA::GUIEventAdapter::KEY_F20;

		mKeyMap[Qt::Key_hyphen         ] = '-';
		mKeyMap[Qt::Key_Equal         ] = '=';

		mKeyMap[Qt::Key_division      ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
		mKeyMap[Qt::Key_multiply      ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
		mKeyMap[Qt::Key_Minus         ] = '-';
		mKeyMap[Qt::Key_Plus          ] = '+';
		//mKeyMap[Qt::Key_H              ] = osgGA::GUIEventAdapter::KEY_KP_Home;
		//mKeyMap[Qt::Key_                    ] = osgGA::GUIEventAdapter::KEY_KP_Up;
		//mKeyMap[92                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Up;
		//mKeyMap[86                    ] = osgGA::GUIEventAdapter::KEY_KP_Left;
		//mKeyMap[87                    ] = osgGA::GUIEventAdapter::KEY_KP_Begin;
		//mKeyMap[88                    ] = osgGA::GUIEventAdapter::KEY_KP_Right;
		//mKeyMap[83                    ] = osgGA::GUIEventAdapter::KEY_KP_End;
		//mKeyMap[84                    ] = osgGA::GUIEventAdapter::KEY_KP_Down;
		//mKeyMap[85                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Down;
		mKeyMap[Qt::Key_Insert        ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
		//mKeyMap[Qt::Key_Delete        ] = osgGA::GUIEventAdapter::KEY_KP_Delete;
	}

	~QtKeyboardMap()
	{
	}

	int remapKey(QKeyEvent* event)
	{
		KeyMap::iterator itr = mKeyMap.find(event->key());
		if (itr == mKeyMap.end())
		{
			return int(*(event->text().toAscii().data()));
		}
		else
			return itr->second;
	}

private:
	typedef std::map<unsigned int, int> KeyMap;
	KeyMap mKeyMap;
};

static QtKeyboardMap s_QtKeyboardMap;
#define IDM_INMDI_M 0x0012

ViewerQT::ViewerQT(osgViewer::Viewer* pVb, QWidget * parent, QGLWidgetEventCallback* pEventCallback) 
	: QWidget(parent), _pViewer(pVb), _pEventCallBack(pEventCallback), m_pDeleteData(0)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::Widget|Qt::CustomizeWindowHint);
	setStyleSheet("background-color: rgb(0, 0, 0);");
	setMinimumSize(281,208);
	setMouseTracking(true);
	setProperty("FullScreen",false);
	setFocusPolicy(Qt::StrongFocus);
	

	//osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	/*if(_pViewer)
	{
	_pViewer = new osgViewer::Viewer();
	}*/
	//_pViewer->setCamera(camera);
	_pViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
	//_pViewer->setLightingMode(osg::View::NO_LIGHT);
	 
	//_gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
	_glwidget = new OSGGLWidget(this, 0, 0, true);
	connect(_glwidget, SIGNAL(SetCurrentWidget(QWidget*)),this,  SIGNAL(SetCurrentWidget(QWidget*)));
	_pViewer->getCamera()->setGraphicsContext(new osgQt::GraphicsWindowQt(_glwidget));

	_pViewer->getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
#if 1
	_pViewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f);
#else
	_pViewer->getCamera()->setProjectionMatrixAsOrtho(-width()/2.0, width()/2.0, -height()/2.0, height()/2.0, -1000000, 1000000);
#endif
	_gw = dynamic_cast<osgViewer::GraphicsWindow*>(_pViewer->getCamera()->getGraphicsContext());
	_pViewer->getCamera()->setGraphicsContext(_gw);

	QGridLayout* g = new QGridLayout(this);
	g->setContentsMargins(1,0,0,0);
	 QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	 _glwidget->setSizePolicy(sizePolicy);
	this->setLayout(g);
	g->addWidget(_glwidget,0,0,1,1);
	g->setHorizontalSpacing(0);
	g->setSpacing(0);
	connect(&mThreadHandle, SIGNAL(notify()), this, SLOT(Slot_Notify()));
	mThreadHandle.startRender();
}

ViewerQT::~ViewerQT()
{
	mThreadHandle.stopRender();
	mThreadHandle.wait();
}


void ViewerQT::closeEvent( QCloseEvent *event )
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

	if(m_pDeleteData) m_pDeleteData->DoDelete();
}

void ViewerQT::setKeyboardModifiers( QInputEvent* event )
{
	int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
	unsigned int mask = 0;
	if ( modkey & Qt::ShiftModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
	if ( modkey & Qt::ControlModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
	if ( modkey & Qt::AltModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
	_gw->getEventQueue()->getCurrentEventState()->setModKeyMask( mask );
}

void ViewerQT::paintEvent( QPaintEvent* event )
{
	try
	{
		if(_pViewer)
			_pViewer->frame();
	}
	catch (...)
	{

	}
	
}

void ViewerQT::AddMenuItem()
{
	QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
	if (subW)
	{
		HMENU hMenu = ::GetSystemMenu(winId(), FALSE);
		if (hMenu != NULL)
		{
			::AppendMenuA(hMenu, MF_SEPARATOR, 0, 0);
			::AppendMenuA(hMenu, MF_STRING, IDM_INMDI_M, "窗口锁定");
		}
	}
}

void ViewerQT::setVisible( bool visible )
{
	if (visible)
	{
		AddMenuItem();
	}
	QWidget::setVisible(visible);
}

bool ViewerQT::winEvent(MSG *m, long *result)
{
	if (m->message == WM_SYSCOMMAND)
	{
		if ((m->wParam) == IDM_INMDI_M)
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

void ViewerQT::resizeEvent( QResizeEvent* event )
{
	try
	{
		int width = this->width();
		int height = this->height();

		_glwidget->resize(width,height);
		if (_gw)
		{
			_gw->getEventQueue()->windowResize(0, 0, width, height );
			_gw->resized(0,0,width,height);
#if 1
			_pViewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width)/static_cast<double>(height), 1.0f, 10000.0f);
#else
			_pViewer->getCamera()->setProjectionMatrixAsOrtho(-width/2.0, width/2.0, -height/2.0, height/2.0, -1000000, 1000000);
#endif
		}

		
	}
	catch (...)
	{

	}
}


void ViewerQT::keyPressEvent( QKeyEvent* event )
{
	int value = s_QtKeyboardMap.remapKey( event );
	_gw->getEventQueue()->keyPress( value );
}

void ViewerQT::keyReleaseEvent( QKeyEvent* event )
{
	int value = s_QtKeyboardMap.remapKey( event );
	_gw->getEventQueue()->keyRelease( value );

	if(_pEventCallBack) _pEventCallBack->keyReleaseEvent(event);
}

void ViewerQT::mousePressEvent( QMouseEvent* event )
{
	int button = 0;
	switch(event->button())
	{
	case(Qt::LeftButton): button = 1; break;
	case(Qt::MidButton): button = 2; break;
	case(Qt::RightButton): button = 3; break;
	case(Qt::NoButton): button = 0; break;
	default: button = 0; break;
	}
	int x = event->x();
	int y = event->y();
	_gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);

	if(_pEventCallBack) _pEventCallBack->mousePressEvent(event);
}

void ViewerQT::mouseReleaseEvent( QMouseEvent* event )
{
	int button = 0;
	switch(event->button())
	{
	case(Qt::LeftButton): button = 1; break;
	case(Qt::MidButton): button = 2; break;
	case(Qt::RightButton): button = 3; break;
	case(Qt::NoButton): button = 0; break;
	default: button = 0; break;
	}
	int x = event->x();
	int y = event->y();
	_gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);

	if(_pEventCallBack) _pEventCallBack->mouseReleaseEvent(event);

	//if (this->parentWidget()->objectName().contains("CompositeViewerQT"))
	{
		//emit SetCurrentWidget(this);
	}
}

void ViewerQT::mouseMoveEvent( QMouseEvent* event )
{
	_gw->getEventQueue()->mouseMotion(event->x(), event->y());

	if(_pEventCallBack) _pEventCallBack->mouseMoveEvent(event);
}

void ViewerQT::wheelEvent( QWheelEvent * event )
{
	_gw->getEventQueue()->mouseScroll(
		event->orientation() == Qt::Vertical ?
		(event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
		(event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT) );

	if(_pEventCallBack) _pEventCallBack->wheelEvent(event);
}

void ViewerQT::Slot_Notify()
{
	try
	{
		this->update();
	}
	catch (...)
	{

	}
}



void ViewerQT::setViewer( osgViewer::Viewer* pVb )
{
	_pViewer = pVb;

	if(pVb)
	{
		_pViewer->getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
#if 1
		_pViewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f);
#else
		_pViewer->getCamera()->setProjectionMatrixAsOrtho(-width()/2.0, width()/2.0, -height()/2.0, height()/2.0, -1000000, 1000000);
#endif
		_pViewer->getCamera()->setGraphicsContext(getGraphicsWindow());
	}
}


void ViewerQT::initOsgPoster(osg::Node* pImageNode)
{
	m_Camera = new osg::Camera;
	m_PosterPrinter = new PosterPrinter;
	m_PrintPosterHandler = new PrintPosterHandler;
	m_osgImage = new osg::Image;
	p_PrintImage = new PrintImageToQImage(this);

	osg::Vec4 clearColor = _pViewer->getCamera()->getClearColor();
	m_Camera->setClearColor(clearColor);
	m_Camera->setClearMask( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	m_Camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	m_Camera->setRenderOrder( osg::Camera::PRE_RENDER );
	m_Camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	//camera->setViewport( 0, 0, 800, 600);
	//camera->addChild(pMakeToImageNode);

	// Set the printer
	//printer->setTileSize( 800, 600 );
	//printer->setPosterSize( 800, 600 );
	m_PosterPrinter->setCamera(m_Camera);

	//posterImage->allocateImage( 800, 600, 1, GL_RGBA, GL_UNSIGNED_BYTE );
	m_PosterPrinter->setFinalPoster(m_osgImage);
	m_PosterPrinter->setPrintCallback(p_PrintImage);
	//printer->setOutputPosterName("000000.png");

	m_PrintPosterHandler->setPosterPrinter(m_PosterPrinter);
	//pViewer->addEventHandler( new PrintPosterHandler(printer) );

	//pParentGroup->addChild(camera);


	osg::Group* pParentGroup = new osg::Group;
	osg::Node* pOldNode = _pViewer->getSceneData();
	//osg::Group* pToImageGroup = dynamic_cast<osg::Group*>(pParentGroup->getChild(0));
	m_Camera->addChild(pOldNode);
	pParentGroup->addChild(pOldNode);
	pParentGroup->addChild(m_Camera);
	_pViewer->addEventHandler(m_PrintPosterHandler);
	_pViewer->setSceneData(pParentGroup);
	//pParentGroup->addChild(m_Camera);
}

void ViewerQT::SetImageSize( float fWidth, float fHeight )
{
	///////////////////////////////////告诉OSG渲染图片///////////////////////////////////////
	m_Camera->setViewport( 0, 0, fWidth, fHeight);
	m_PosterPrinter->setTileSize( fWidth, fHeight);
	m_PosterPrinter->setPosterSize( fWidth, fHeight );

	m_osgImage->allocateImage( fWidth, fHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE );

	//m_PrintPosterHandler->setGiveMeImage(true);
}

void ViewerQT::SendGiveMeImageSignal()
{
	emit PrintImage(m_osgImage);
}

void ViewerQT::GiveYouFuckImage(float fWidth, float fHeight )
{
	SetImageSize(fWidth, fHeight);
	emit toUpdate();
}

void ViewerQT::SaveImage()
{
	QString strFile = IDPApplication::TempPath()+"Thematic.png";
	std::string stdStrFile = strFile.toStdString();
	osgDB::writeImageFile(*m_osgImage, stdStrFile);
}

LONGLONG ViewerQT::getMapScale( int iType )
{
	osg::Camera* pCamera = _pViewer->getCamera();

	double P1X(10), P1Y(0), P2X(11), P2Y(0);
	
	//double dSreenDistance = sqrt((P1X-P2X) * (P1X-P2X) + (P1Y - P2Y) * (P1Y - P2Y));

	osg::Vec3 v3Screen1(P1X, P1Y, 0), v3Screen2(P2X, P2Y, 0), v3World1, v3World2;
	
	osg::Matrix VPW = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();
	osg::Matrix inverseVPW;
	inverseVPW.invert(VPW);
	v3World1 = v3Screen1  * inverseVPW;
	v3World1.set(v3World1.x(), v3World1.z(), 0);

	v3World2 = v3Screen2  * inverseVPW;
	v3World2.set(v3World2.x(), v3World2.z(), 0);


	double dWorldDistance = sqrt((v3World1.x()-v3World2.x()) * (v3World1.x()-v3World2.x()) 
		+ (v3World1.y()-v3World2.y()) * (v3World1.y()-v3World2.y()));

	return dWorldDistance;
}


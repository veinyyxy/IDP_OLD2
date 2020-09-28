#include "OSGGLWidget.h"


OSGGLWidget::OSGGLWidget( QWidget* parent /*= NULL*/, const QGLWidget* shareWidget /*= NULL*/, Qt::WindowFlags f /*= 0*/, bool forwardKeyEvents /*= false */ ):osgQt::GLWidget(parent,shareWidget,f,forwardKeyEvents)
{

}

OSGGLWidget::~OSGGLWidget()
{

}

void OSGGLWidget::mouseReleaseEvent( QMouseEvent* event )
{
	QWidget* Viewqt = this->parentWidget()->parentWidget();
	if (Viewqt != NULL)
	{
		//if (Viewqt->objectName().contains("CompositeViewerQT"))
		{
			emit SetCurrentWidget(this->parentWidget());
		}
	}
	osgQt::GLWidget::mouseReleaseEvent(event);
}

#ifndef OSGGLWIDGET_H
#define OSGGLWIDGET_H

#include <QGLWidget>
#include <osgQt/GraphicsWindowQt>

class  OSGGLWidget : public osgQt::GLWidget
{
	Q_OBJECT

public:
	OSGGLWidget( QWidget* parent = NULL, const QGLWidget* shareWidget = NULL, Qt::WindowFlags f = 0, bool forwardKeyEvents = false );
	~OSGGLWidget();

Q_SIGNALS:
	void SetCurrentWidget(QWidget* currentWidget);

protected:
	virtual void mouseReleaseEvent( QMouseEvent* event );
};


#endif // OSGGLWIDGET_H

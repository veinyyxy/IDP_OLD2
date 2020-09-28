#pragma once
/////////////////////////////////Core/////////////////////////////////
#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QLibrary>
#include <QtCore/QRectF>
#include <QtCore/QVariant>
#include <QtCore/QByteArray>
#include <QtCore/QThreadPool>
#include <QtCore/QRunnable>
#include <QtCore/QDateTime>
////////////////////////////////////UI/////////////////////////////
#include <QtGui/QVector3D>
#include <QtGui/QVector2D>
#include <QtGui/QColor>
#include <QtGui/QMatrix4x4>
#include <QtGui/QWidget>
////////////////////////////////////OpenGL//////////////////////////////////////
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLPixelBuffer>
////////////////////////////////////Xml//////////////////////////////////////
#include <QtXml/QXmlStreamReader>

//#include <Qt/qpointer.h>


struct	DataRadialGrid
{
	QVector3D ps[4];
	float val;
};

typedef QList<DataRadialGrid> RadialGridDataList;

#define START_NAMESPACE namespace QE {
#define END_NAMESPACE }

#ifndef _WIN32
#define CALLBACK
#endif
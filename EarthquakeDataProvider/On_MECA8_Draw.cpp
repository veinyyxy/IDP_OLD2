#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/CullVisitor>


#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>

#include "goMark.h"
#include "EarthquakeProductionDraw.h"
#include "On_MECA8_Draw.h"


On_MECA8_Draw::On_MECA8_Draw(void)
{
}


On_MECA8_Draw::~On_MECA8_Draw(void)
{
}

EARTHQUAKE_DATA_TYPE On_MECA8_Draw::GetTypeOfProduct()
{
	return STRUCT_MECA8;
}

void On_MECA8_Draw::LoadFile( const QString& filename )
{
	throw std::exception("The method or operation is not implemented.");
}

bool On_MECA8_Draw::DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable /*= 0 */ )
{
	throw std::exception("The method or operation is not implemented.");
}

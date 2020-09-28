#pragma once
/////////////////////////////////STL Header/////////////////////////////////////////
#include <string>
#include <map>
#include <sstream>
/////////////////////////////////OSG Header/////////////////////////////////////////
#include <osg/Vec3d>
#include <osg/Vec3>
#include <osg/Array>
#include <osg/Geometry>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <osgGA/StateSetManipulator>
/////////////////////////////////GEOS Header/////////////////////////////////////////
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/CoordinateSequence.h>
using namespace geos;
///////////////////////////////////BOOST Header///////////////////////////////////////
#include <boost/Any.hpp>
//////////////////////////////////////////////////////////////////////////

#include "GeosOnTheOsg_Global.h"

#define GOTO_NAMESPACE_START namespace goto_gis{
#define GOTO_NAMESPACE_END }
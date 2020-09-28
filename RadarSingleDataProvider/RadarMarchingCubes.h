#pragma once
#include <osg/Vec3>
#include <osg/Array>
#include <osg/Vec4>
#include <osg/Group>
#include <osg/Geometry>
#include "RadReadColor_XML.h"

struct v3Point
{
	double x, y, z;
	double value;
};

struct Cube
{
	v3Point p0;
	v3Point p1;
	v3Point p2;
	v3Point p3;
	v3Point p4;
	v3Point p5;
	v3Point p6;
	v3Point p7;
};

typedef std::vector<v3Point> CUBE_DATA_VECTOR, *P_CUBE_DATA_VECTOR;

class RadarMarchingCubes
{
public:
	virtual void GetIsosurfaceGeometry(double dIsom, osg::Group& geo) = 0;
	virtual void SetColorTable(COLORTABLE_VECTOR& pColorTable) = 0;
	virtual void SetRadarCentral(double x, double y, double z) = 0;
	virtual void GetIsosurfaceGeometry(double dIsom, osg::Vec3Array** vertexArray, osg::Vec3Array** normalArray, osg::Vec4Array** colorArray) = 0;
	virtual P_CUBE_DATA_VECTOR GetCubes() = 0;
	virtual Cube* GetCubes(int* iCount) = 0;
	virtual P_COLORTABLE_VECTOR GetColorTable() = 0;
};

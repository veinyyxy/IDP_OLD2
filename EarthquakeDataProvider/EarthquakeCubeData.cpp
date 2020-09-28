#include "EarthquakeCubeData.h"
#include <osgEarth/Map>

EarthquakeCubeData::EarthquakeCubeData(osg::Geometry* pGeom, P_CUBE_DATA_VECTOR pCubeData)
	: m_pGeom(pGeom), m_CubeDataBuffer(pCubeData)
{
	/*vertexArray = new osg::Vec3Array;
	normalArray = new osg::Vec3Array;
	colorArray = new osg::Vec4Array;*/
}


EarthquakeCubeData::~EarthquakeCubeData(void)
{
}

int EarthquakeCubeData::GetSize()
{
	return m_CubeDataBuffer->size();
}

void EarthquakeCubeData::GetCubePoint( int iIndex, double& x, double& y, double& z, double& iValue )
{
	v3Point pp = m_CubeDataBuffer->at(iIndex);
	x = pp.x;
	y = pp.y;
	z = pp.z;
	iValue = pp.value;
}

void EarthquakeCubeData::GetColorFromValue( int iValue, double& dr, double& dg, double& db, double& da )
{
	if(iValue>=0 && iValue<=500)
	{
		dr = 1;
		dg = 0;
		db = 0;
		da = 1;
	}
	else if(iValue>=500&&iValue<=600)
	{
		dr = 0;
		dg = 1;
		db = 0;
		da = 1;
	}
	else if(iValue>=600&&iValue<=700)
	{
		dr = 0;
		dg = 0;
		db = 1;
		da = 1;
	}
	else if(iValue>=700&&iValue<=800)
	{
		dr = 0.4;
		dg = 1;
		db = 0.3;
		da = 1;
	}
	else
	{
		dr = 0.5;
		dg = 0.8;
		db = 1;
		da = 1;
	}
	
}

void EarthquakeCubeData::OutVertexData( int iIndex, const double& x, const double& y, const double& z )
{
	vertexArray->push_back(osg::Vec3(x, y, z));
}

void EarthquakeCubeData::OutNormalData( int iIndex, const double& x, const double& y, const double& z )
{
	//normalArray->push_back(osg::Vec3(x, y, z));
}

void EarthquakeCubeData::OutColorData( int iValue, const double& dr, const double& dg, const double& db, const double& da )
{
	colorArray->push_back(osg::Vec4(dr, dg, db, da));
}

void EarthquakeCubeData::MarchingCubesFinish()
{
	/*m_pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertexArray->size()));
	m_pGeom->setVertexArray(vertexArray);

	m_pGeom->setColorArray(colorArray);
	m_pGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);*/
}

void EarthquakeCubeData::MarchingCubesStart()
{
	
}


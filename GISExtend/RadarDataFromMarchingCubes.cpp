#include "RadarDataFromMarchingCubes.h"
#include <osgEarth/Map>

RadarDataFromMarchingCubes::RadarDataFromMarchingCubes(RadarMarchingCubes* rmc, osg::Geometry* pGeom, P_CUBE_DATA_VECTOR pCubeData)
	: m_pRadarMarchingCubes(rmc), m_pGeom(pGeom), m_CubeDataBuffer(pCubeData)
{
	/*vertexArray = new osg::Vec3Array;
	normalArray = new osg::Vec3Array;
	colorArray = new osg::Vec4Array;*/
}


RadarDataFromMarchingCubes::~RadarDataFromMarchingCubes(void)
{
}

int RadarDataFromMarchingCubes::GetSize()
{
	return m_CubeDataBuffer->size();
}

void RadarDataFromMarchingCubes::GetCubePoint( int iIndex, double& x, double& y, double& z, double& iValue )
{
	v3Point pp = m_CubeDataBuffer->at(iIndex);
	x = pp.x;
	y = pp.y;
	z = pp.z;
	iValue = pp.value;
}

void RadarDataFromMarchingCubes::GetColorFromValue( int iValue, double& dr, double& dg, double& db, double& da )
{
	if(!m_pRadarMarchingCubes) return;
	for(int iC = 0; iC < m_pRadarMarchingCubes->GetColorTable()->size(); iC++)
	{
		if(iValue >= m_pRadarMarchingCubes->GetColorTable()->at(iC)->fStart && iValue <= m_pRadarMarchingCubes->GetColorTable()->at(iC)->fEnd)
		{
			dr = m_pRadarMarchingCubes->GetColorTable()->at(iC)->nRed / 255.0;
			dg = m_pRadarMarchingCubes->GetColorTable()->at(iC)->nGreen / 255.0;
			db = m_pRadarMarchingCubes->GetColorTable()->at(iC)->nBlue / 255.0;
			da = 1.0;
		}
	}
}

void RadarDataFromMarchingCubes::OutVertexData( int iIndex, const double& x, const double& y, const double& z )
{
	vertexArray->push_back(osg::Vec3(x, y, z));
}

void RadarDataFromMarchingCubes::OutNormalData( int iIndex, const double& x, const double& y, const double& z )
{
	//normalArray->push_back(osg::Vec3(x, y, z));
}

void RadarDataFromMarchingCubes::OutColorData( int iValue, const double& dr, const double& dg, const double& db, const double& da )
{
	colorArray->push_back(osg::Vec4(dr, dg, db, da));
}

void RadarDataFromMarchingCubes::MarchingCubesFinish()
{
	/*m_pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertexArray->size()));
	m_pGeom->setVertexArray(vertexArray);

	m_pGeom->setColorArray(colorArray);
	m_pGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);*/
}

void RadarDataFromMarchingCubes::MarchingCubesStart()
{
	
}


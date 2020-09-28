#pragma once

#include <vector>
#include <osg/Geometry>
#include "../GISExtend/marchingcubesdatainterface.h"

namespace osgEarth
{
	class Map;
}

struct v3Point
{
	double x, y, z;
	double value;
};

typedef std::vector<v3Point> CUBE_DATA_VECTOR, *P_CUBE_DATA_VECTOR;

class EarthquakeCubeData :
	public MarchingCubesDataInterface
{
public:
	EarthquakeCubeData(osg::Geometry* pGeom = 0, P_CUBE_DATA_VECTOR pCubeData = 0);
	virtual ~EarthquakeCubeData(void);

	virtual int GetSize();

	virtual void GetCubePoint( int iIndex, double& x, double& y, double& z, double& iValue );

	virtual void GetColorFromValue( int iValue, double& dr, double& dg, double& db, double& da );

	virtual void OutVertexData( int iIndex, const double& x, const double& y, const double& z );

	virtual void OutNormalData( int iIndex, const double& x, const double& y, const double& z );

	virtual void OutColorData( int iValue, const double& dr, const double& dg, const double& db, const double& da );

	virtual void MarchingCubesFinish();

	virtual void MarchingCubesStart();

	inline void SetGeomtry(osg::Geometry* pGeom){m_pGeom = pGeom;}
	inline void SetCubeData(P_CUBE_DATA_VECTOR pCubeData){m_CubeDataBuffer = pCubeData;}
	inline void SetVertexColor(osg::Vec3Array* pV3A, osg::Vec4Array* pV4C){vertexArray = pV3A; colorArray = pV4C;}
private:
	osg::Geometry* m_pGeom;
	P_CUBE_DATA_VECTOR m_CubeDataBuffer;

	osg::Vec3Array* vertexArray;
	osg::Vec3Array* normalArray;
	osg::Vec4Array* colorArray;
};


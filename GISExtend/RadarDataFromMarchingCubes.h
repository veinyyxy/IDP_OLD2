#pragma once
#include "marchingcubesdatainterface.h"
#include "RadarMarchingCubes.h"

namespace osgEarth
{
	class Map;
}

class RadarDataFromMarchingCubes :
	public MarchingCubesDataInterface
{
public:
	RadarDataFromMarchingCubes(RadarMarchingCubes* rmc = 0, osg::Geometry* pGeom = 0, P_CUBE_DATA_VECTOR pCubeData = 0);
	virtual ~RadarDataFromMarchingCubes(void);

	virtual int GetSize();

	virtual void GetCubePoint( int iIndex, double& x, double& y, double& z, double& iValue );

	virtual void GetColorFromValue( int iValue, double& dr, double& dg, double& db, double& da );

	virtual void OutVertexData( int iIndex, const double& x, const double& y, const double& z );

	virtual void OutNormalData( int iIndex, const double& x, const double& y, const double& z );

	virtual void OutColorData( int iValue, const double& dr, const double& dg, const double& db, const double& da );

	virtual void MarchingCubesFinish();

	virtual void MarchingCubesStart();

	inline void SetRadarMarchingCubes(RadarMarchingCubes* pRMC){m_pRadarMarchingCubes = pRMC;}
	inline void SetGeomtry(osg::Geometry* pGeom){m_pGeom = pGeom;}
	inline void SetCubeData(P_CUBE_DATA_VECTOR pCubeData){m_CubeDataBuffer = pCubeData;}
	inline void SetVertexColor(osg::Vec3Array* pV3A, osg::Vec4Array* pV4C, osg::Vec3Array* pNormorl){
		vertexArray = pV3A;
		colorArray = pV4C;
		normalArray = pNormorl;
	}
private:
	osg::Geometry* m_pGeom;
	P_CUBE_DATA_VECTOR m_CubeDataBuffer;
	RadarMarchingCubes* m_pRadarMarchingCubes;

	osg::Vec3Array* vertexArray;
	osg::Vec3Array* normalArray;
	osg::Vec4Array* colorArray;
};


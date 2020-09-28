#pragma once
#include "radarmarchingcubes.h"
#include "SingleDataProvider.h"
#include "RadProjTransform.h"

class RadarDataParser : public SingleDataProvider
{
public:
	void dumpAllCube(P_COLORTABLE_VECTOR pColorTable, P_CUBE_DATA_VECTOR pCDV);
	inline void SetCentral(osg::Vec3 point){m_RadarCentral = point;};
private:
	void dumpPoint(int iLayer,int jBlock,int kRad,int zGate,RadProjTransform *proj ,P_CUBE_DATA_VECTOR pCDV);
	//osg::Vec3  stationPosition();//设置和获得雷达站中心
private:
	osg::Vec3 m_RadarCentral;
};

class RadarMarchingCubesImp :
	public RadarMarchingCubes
{
public:
	RadarMarchingCubesImp(GenerationData* pPB = 0);
	virtual ~RadarMarchingCubesImp(void);
	virtual void GetIsosurfaceGeometry( double dIsom, osg::Group& geo );
	virtual void GetIsosurfaceGeometry(double dIsom, osg::Vec3Array** vertexArray, osg::Vec3Array** normalArray, osg::Vec4Array** colorArray);
	virtual void SetColorTable(COLORTABLE_VECTOR& pColorTable);
	virtual P_COLORTABLE_VECTOR GetColorTable(){return &m_ColorTable;}
	virtual void SetRadarCentral(double x, double y, double z);
	inline P_CUBE_DATA_VECTOR GetCubes();
	Cube* GetCubes(int* iCount);
	inline void SetProductBase(GenerationData* pPB){m_RadarDataParer = (RadarDataParser*)pPB;}

private:
	void SelectScell(osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Vec3& p5, osg::Vec3& p6, osg::Vec3& p7, osg::Vec3& p8);
	void GetCubeData();
	
private:
	RadarDataParser* m_RadarDataParer;
	CUBE_DATA_VECTOR m_CubeDataBuffer;
	COLORTABLE_VECTOR m_ColorTable;
	osg::Vec3 m_RadarCentral;
	double m_dIsoValue;
	osg::ref_ptr<osg::Vec3Array> m_TriArray;
	osg::ref_ptr<osg::Vec3Array> m_TriNomArray;
};


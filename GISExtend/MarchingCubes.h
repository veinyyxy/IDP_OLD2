#pragma once

#include "GISExtendConfig.h"

class MarchingCubesDataInterface;
class MarchingCubesOutputInterface;

class MarchingCubes
{
public:
	struct v3Point
	{
		double x, y, z;
		double value;
	};

	struct CubeEdge
	{
		v3Point start;
		v3Point end;
	};

	MarchingCubes(MarchingCubesDataInterface* pCubeDataBuffer = 0);
	virtual ~MarchingCubes(void);

	void MarchingCubesMain(int m_dIsoValue);
	void MarchingCubesMain(float m_dIsoValue);
	inline void SetCubeDataBuffer(MarchingCubesDataInterface* pCubeDataBuffer){m_CubeDataBuffer = pCubeDataBuffer;}
private:
	void InitEdgeMap(CubeEdge* pCubeEdge, int iCubeID);
	void ComputeValueOnEdge(CubeEdge* pCubeEdge, int iEdgeID, int ivalue, v3Point& result);
	float computeENG(float x,float y,float z);
	void vNormalizeVector(v3Point &rfVectorResult, v3Point &rfVectorSource);
	void vGetNormal(v3Point& rfNormal, float fX, float fY, float fZ);
	MarchingCubesDataInterface* m_CubeDataBuffer;
};


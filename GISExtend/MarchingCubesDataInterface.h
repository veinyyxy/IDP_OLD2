#pragma once

class MarchingCubesDataInterface
{
public:
	virtual int GetSize() = 0;
	virtual void GetCubePoint(int iIndex, double& x, double& y, double& z, double& iValue) = 0;
	virtual void GetColorFromValue(int iValue, double& dr, double& dg, double& db, double& da) = 0;
	virtual void OutVertexData(int iIndex, const double& x, const double& y, const double& z) = 0;
	virtual void OutNormalData(int iIndex, const double& x, const double& y, const double& z) = 0;
	virtual void OutColorData(int iValue, const double& dr, const double& dg, const double& db, const double& da) = 0;
	virtual void MarchingCubesFinish() = 0;
	virtual void MarchingCubesStart() = 0;
};


#pragma once
#include <osg/Node>
#include <osg/Vec4>
#include <osg/ImageUtils>

class RadarMarchingCubes;

class IsoSurface
{
public:
	virtual void ComputerIso(int,bool = false) = 0;
	virtual void DelIsoSuerface(int) = 0;
	virtual void SetAddtionalData(void* pData) = 0;
	virtual void SetRadarMarchingCubes(RadarMarchingCubes*) = 0;

	virtual void ComputerVolume(osg::ImageList& imageList, float fVLen, float fVWidth, float fVHeight) = 0;
	virtual void AddVolumeClipPlane(const osg::Vec4& rect) = 0;
};


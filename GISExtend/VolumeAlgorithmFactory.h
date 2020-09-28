#pragma once
#include "GISExtendConfig.h"
class VolumeDraw;

class VolumeAlgorithmFactory
{
public:
	static VolumeDraw* GetVolumeDraw();
};


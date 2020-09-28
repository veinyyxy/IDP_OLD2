#include "VolumeAlgorithmFactory.h"
#include "osgVolumeImp.h"

VolumeDraw* VolumeAlgorithmFactory::GetVolumeDraw()
{
	return new osgVolumeImp;
}

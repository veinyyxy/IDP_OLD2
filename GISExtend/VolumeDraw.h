#pragma once

#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/ImageUtils>

class VolumeDraw
{
public:
	virtual void InputImage(osg::ImageList& pImages, int maximumTextureSize) =0;
	virtual void GetVolume(osg::Group* pVolumeNode) = 0;
};


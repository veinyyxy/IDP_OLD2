#pragma once
#include "VolumeDraw.h"

class osgVolumeImp :
	public VolumeDraw
{
public:
	osgVolumeImp(void);
	virtual ~osgVolumeImp(void);

	virtual void InputImage(osg::ImageList& pImages, int maximumTextureSize);

	virtual void GetVolume( osg::Group* pVolumeNode );
private:
	typedef std::list< osg::ref_ptr<osg::Image> > Images;
	Images images;
};


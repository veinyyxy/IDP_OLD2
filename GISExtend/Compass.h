#pragma once
#include "GISExtendConfig.h"

class Compass : public osg::Drawable
{
public:
	Compass(void);
	virtual ~Compass(void);
	 virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
	 virtual osg::Object* cloneType() const;
     virtual osg::Object* clone(const osg::CopyOp&) const;
};


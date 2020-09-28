#pragma once
#include "goConfig.h"
#include "GeosOnTheOsg_Global.h"

GOTO_NAMESPACE_START

class GEOSONTHEOSG_EXPORT goSymbolGeode :
	public osg::Billboard
{
public:
	goSymbolGeode(void);
	virtual ~goSymbolGeode(void);
	bool computeMatrix(osg::Matrix& modelview, const osg::Vec3& eye_local, const osg::Vec3& pos_local) const;
};

GOTO_NAMESPACE_END
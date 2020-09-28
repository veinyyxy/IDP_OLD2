#pragma once
#include "Layer.h"

GOTO_NAMESPACE_START
class GEOSONTHEOSG_EXPORT ManualDrawLayer :
	public Layer
{
public:
	ManualDrawLayer(DataProvider* data = 0, CoordinateTransform* ct = 0, int iNodeType = 1);
	virtual ~ManualDrawLayer(void);

	virtual void TransformLayer();

	virtual void CreateLayerData();

	virtual int CreateLayer( const std::string& filename );
};

GOTO_NAMESPACE_END

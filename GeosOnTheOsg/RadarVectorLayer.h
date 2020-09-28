#pragma once
#include "goConfig.h"
#include "DataProvider.h"
#include "Layer.h"
#include "CoordinateTransform.h"
#include "Proj4Transform.h"

using namespace std;

GOTO_NAMESPACE_START

class GEOSONTHEOSG_EXPORT RadarVectorLayer :public Layer
{
public:
	RadarVectorLayer(DataProvider* data = 0, CoordinateTransform* ct = 0);
	virtual ~RadarVectorLayer(void);
	virtual void TransformLayer();
	virtual void CreateLayerData();
	virtual int CreateLayer(const std::string& filename);
	virtual void operator=(const RadarVectorLayer& radarVectorLayer);


};

GOTO_NAMESPACE_END
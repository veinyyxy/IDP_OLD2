#pragma once

#include "goConfig.h"
#include "Layer.h"
#include "CoordinateTransform.h"

using namespace std;

GOTO_NAMESPACE_START

typedef enum _VECTOR_DATA_TYPE
{
	VECTOR_POINT = 1,
	VECTOR_LINE,
	VECTOR_POLYGON
} VECTOR_DATA_TYPE, *P_VECTOR_DATA_TYPE;


class  GEOSONTHEOSG_EXPORT VectorLayer: public Layer
{
public:
	VectorLayer(DataProvider* data = 0, CoordinateTransform* ct = 0, int iNT = 0);
	virtual ~VectorLayer(void);

public:
	virtual void TransformLayer();
	virtual int CreateLayer(const std::string& filename);
	virtual void CreateLayerData();
	inline void VectorDataType(VECTOR_DATA_TYPE vdt){m_vectorDataType = vdt;}
	inline VECTOR_DATA_TYPE VectorDataType(){return m_vectorDataType;}

	virtual void operator=(const VectorLayer& vectorLayer);
private:
	VECTOR_DATA_TYPE m_vectorDataType;

};

GOTO_NAMESPACE_END

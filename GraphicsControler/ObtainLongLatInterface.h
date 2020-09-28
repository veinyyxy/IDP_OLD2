#pragma once
#include <string>
#include "GraphicsControler_global.h"

class ObtainLongLatInterface
{
public:
	typedef enum{FROM_NODE = 1, FROM_DRAWABLE} OBTAIN_OBJECT_TYPE;
public:
	virtual void GetObjectName(OBTAIN_OBJECT_TYPE& iObjectType, std::string& strName) = 0;
	//virtual void GetCoordinateReference(goto_gis::CoordinateTransform** ct) = 0;
	virtual void OutLongLatHeight(double dLo, double dLa, double dH) = 0;
};


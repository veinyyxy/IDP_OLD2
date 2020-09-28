#include "ManualDrawLayer.h"

GOTO_NAMESPACE_START

ManualDrawLayer::ManualDrawLayer(DataProvider* data , CoordinateTransform* ct, int iNodeType) : Layer(data, ct, iNodeType)
{
}


ManualDrawLayer::~ManualDrawLayer(void)
{
}

void ManualDrawLayer::TransformLayer()
{
	//throw std::exception("The method or operation is not implemented.");
}

void ManualDrawLayer::CreateLayerData()
{
	//throw std::exception("The method or operation is not implemented.");
}

int ManualDrawLayer::CreateLayer( const std::string& filename )
{
	return 0;
}

GOTO_NAMESPACE_END
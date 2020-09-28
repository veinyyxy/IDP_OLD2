#include "gogeometry.h"
#include "goColorLump.h"

GOTO_NAMESPACE_START

goColorLump::goColorLump(void)
{
}


goColorLump::~goColorLump(void)
{
}

void goColorLump::CreateOsgFromGeos( osg::Node* pNode  )
{
	throw std::exception("The method or operation is not implemented.");
}

void goColorLump::SetCoordinateTransform( CoordinateTransform* pCT )
{
	throw std::exception("The method or operation is not implemented.");
}

geom::GeometryTypeId goColorLump::GetGeometryType()
{
	throw std::exception("The method or operation is not implemented.");
}

void goColorLump::TransformGeometry()
{
	throw std::exception("The method or operation is not implemented.");
}

GOTO_NAMESPACE_END
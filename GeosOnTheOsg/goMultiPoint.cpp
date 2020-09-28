#include "goMultiPoint.h"
#include "Layer.h"
GOTO_NAMESPACE_START

goMultiPoint::goMultiPoint(void)
{
}

goMultiPoint::goMultiPoint( geom::Geometry* geometry, CoordinateTransform* pCT /*= 0*/, Layer* pL /*= 0*/ )
	: goGeometry(geometry, pCT, pL)
{
	m_pCoordinateSequence = m_pGeosGeomtry->getCoordinates();
}

goMultiPoint::~goMultiPoint(void)
{
}

void goMultiPoint::CreateOsgFromGeos(osg::Node* pNode)
{
	osg::Vec3 srcVec3, destVec3;
	if(!m_pCorrdinateTransform) return;
	osg::Geode* geode = pNode->asGeode();
	if(geode == 0) return;

	std::size_t  fcsSize  =  m_pGeosGeomtry->getNumGeometries();
	osg::ref_ptr<osg::Geometry>* pOsgGeometry = new osg::ref_ptr<osg::Geometry>;

	for(std::size_t fscs = 0; fscs < fcsSize; fscs++)
	{
		const geom::Geometry* geom = m_pGeosGeomtry->getGeometryN(fscs);
		m_pCoordinateSequence = geom->getCoordinates();

		*pOsgGeometry = new osg::Geometry;

		osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
		std::size_t  csSize =  m_pCoordinateSequence->getSize();
		for(std::size_t scs = 0; scs < csSize; scs++)
		{
			srcVec3.set((*m_pCoordinateSequence)[scs].x,(*m_pCoordinateSequence)[scs].y, (*m_pCoordinateSequence)[scs].z);
			TransormPoint(srcVec3, destVec3);
			pyramidVertices->push_back(destVec3);
		}
		(*pOsgGeometry)->setVertexArray(pyramidVertices);
		if(m_bUserLayerColor)
		{
			m_pColors->clear();
			m_pColors->push_back(m_pParentLayer->GetLayerColor());
			m_pOsgGeometry->setColorArray(m_pColors);
			m_pOsgGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
		}
		else
		{
			m_pOsgGeometry->setColorArray(m_pColors);
			m_pOsgGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
		}
		(*pOsgGeometry)->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,csSize)); 
		geode->addDrawable((*pOsgGeometry));
	}
}

void goMultiPoint::SetCoordinateTransform( CoordinateTransform* pCT )
{
	m_pCorrdinateTransform = pCT;
}

void goMultiPoint::TransformGeometry()
{
	osg::Vec3 srcVec3, destVec3;
	std::size_t  fcsSize  =  m_pGeosGeomtry->getNumGeometries();

	osg::Geode* pGeode = m_pParentLayer->GetLayerNode()->asGeode();
	if(pGeode == 0) return;

	osg::Geode::DrawableList dList = pGeode->getDrawableList();

	for(std::size_t fscs = 0; fscs < fcsSize; fscs++)
	{
		const geom::Geometry* geom = m_pGeosGeomtry->getGeometryN(fscs);
		geom::CoordinateSequence* pCoordinateSequence = geom->getCoordinates();
		std::size_t csSize = pCoordinateSequence->getSize();

		osg::Geometry* pOsgGeometry = dynamic_cast<osg::Geometry*>(dList.at(fscs).get());
		osg::Vec3Array* pyramidVertices  = dynamic_cast<osg::Vec3Array*>(pOsgGeometry->getVertexArray());
		pyramidVertices->clear();

		for (size_t scs = 0; scs<csSize; scs++)
		{
			srcVec3.set((*pCoordinateSequence)[scs].x,(*pCoordinateSequence)[scs].y, (*pCoordinateSequence)[scs].z);

			TransormPoint(srcVec3, destVec3);
			pyramidVertices->push_back(destVec3);
		}
		pOsgGeometry->dirtyDisplayList();
		pOsgGeometry->dirtyBound();
	}
}

void goMultiPoint::operator=( const goMultiPoint& other )
{
	this->m_pGeosGeomtry.reset(other.m_pGeosGeomtry->clone());
}

GOTO_NAMESPACE_END
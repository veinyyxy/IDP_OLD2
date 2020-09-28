#include <osgUtil/Tessellator>
#include "goMultiPolygon.h"
#include "Layer.h"
GOTO_NAMESPACE_START
	
goMultiPolygon::goMultiPolygon(void)
{
}

goMultiPolygon::goMultiPolygon( geom::Geometry* geometry, CoordinateTransform* pCT /*= 0*/, Layer* pL /*= 0*/ )
	: goGeometry(geometry, pCT, pL)
{
	m_pCoordinateSequence = m_pGeosGeomtry->getCoordinates();
}


goMultiPolygon::~goMultiPolygon(void)
{
}

void goMultiPolygon::CreateOsgFromGeos(osg::Node* pNode)
{
#if 1
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

		(const_cast<geom::Geometry*>(geom))->setUserData(pOsgGeometry);

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
		(*pOsgGeometry)->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,csSize)); 

		osg::ref_ptr<osgUtil::Tessellator> tes = new osgUtil::Tessellator();  
		tes->setBoundaryOnly(false);  
		tes->setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);  
		tes->setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);  
		tes->retessellatePolygons(**pOsgGeometry);  

		geode->addDrawable((*pOsgGeometry).get());
	}
#endif
}

void goMultiPolygon::SetCoordinateTransform( CoordinateTransform* pCT )
{
	m_pCorrdinateTransform = pCT;
}

void goMultiPolygon::TransformGeometry()
{
#if 0
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

		for (int scs = 0; scs<csSize; scs++)
		{
			srcVec3.set((*pCoordinateSequence)[scs].x,(*pCoordinateSequence)[scs].y, (*pCoordinateSequence)[scs].z);

			double x = srcVec3.x();
			double y = srcVec3.y();
			double z = m_pParentLayer->LayerZ();
			srcVec3.set(x,y,z);

			m_pCorrdinateTransform->Transform(srcVec3,destVec3);

			pyramidVertices->push_back(destVec3);
		}
		pOsgGeometry->dirtyDisplayList();
		pOsgGeometry->dirtyBound();
	}
#endif
	osg::Vec3 srcVec3, destVec3;
	std::size_t  fcsSize  =  m_pGeosGeomtry->getNumGeometries();
	osg::Geode* pGeode = m_pParentLayer->GetLayerNode()->asGeode();
	if(pGeode == 0) return;

	for(std::size_t fscs = 0; fscs < fcsSize; fscs++)
	{
		const geom::Geometry* geom = m_pGeosGeomtry->getGeometryN(fscs);
		geom::CoordinateSequence* pCoordinateSequence = geom->getCoordinates();
		std::size_t csSize = pCoordinateSequence->getSize();

		osg::Geometry* pOsgGeometry = (osg::Geometry*)(const_cast<geom::Geometry*>(geom)->getUserData());
		if(!pOsgGeometry)
			continue;

		osg::Vec3Array* pyramidVertices  = dynamic_cast<osg::Vec3Array*>(pOsgGeometry->getVertexArray());
		pyramidVertices->clear();

		for (size_t scs = 0; scs<csSize; scs++)
		{
			srcVec3.set((*pCoordinateSequence)[scs].x,(*pCoordinateSequence)[scs].y, (*pCoordinateSequence)[scs].z);
			TransormPoint(srcVec3, destVec3);
			pyramidVertices->push_back(destVec3);
		}
		osg::PrimitiveSet* pPrimitiveSet = pOsgGeometry->getPrimitiveSet(0);
		pPrimitiveSet->setNumInstances(pyramidVertices->size());

		osg::ref_ptr<osgUtil::Tessellator> tes = new osgUtil::Tessellator();  
		tes->setBoundaryOnly(false);  
		tes->setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);  
		tes->setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);  
		tes->retessellatePolygons(*pOsgGeometry);  

		pOsgGeometry->dirtyBound();
		pOsgGeometry->dirtyDisplayList();
	}
}

void goMultiPolygon::operator=( const goMultiPolygon& other )
{
		this->m_pGeosGeomtry.reset(other.m_pGeosGeomtry->clone());
}

GOTO_NAMESPACE_END
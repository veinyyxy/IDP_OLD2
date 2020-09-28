#include <osgUtil/Tessellator>
#include <osg/TriangleFunctor>
#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/LinearRing.h>
//#include <ogr_geometry.h>

#include "CoordinateTransform.h"
#include "goPolygon.h"
#include "Layer.h"
GOTO_NAMESPACE_START

goPolygon::goPolygon(void)
{
}

goPolygon::goPolygon( geom::Geometry* geometry, CoordinateTransform* pCT /*= 0*/, Layer* pL /*= 0*/ ) 
	: goGeometry(geometry, pCT, pL)
{
	//m_pGeosGeomtry.reset(geometry);
	//const geos::geom::GeometryFactory* pGeoFa = m_pGeosGeomtry->getFactory();
	//const geos::geom::CoordinateSequenceFactory* pCSF = pGeoFa->getCoordinateSequenceFactory();
	//m_pCoordinateSequence = m_pGeosGeomtry->getCoordinates();
}

goPolygon::~goPolygon(void)
{
}

void goPolygon::CreateOsgFromGeos(osg::Node* pNode)
{
	osg::Vec3 srcVec3, destVec3;
	if(!m_pCorrdinateTransform) return;
	osg::Geode* geode = pNode->asGeode();
	if(geode == 0) return;

	////const geos::geom::GeometryFactory* pGeoFa = m_pGeosGeomtry->getFactory();
	////const geos::geom::CoordinateSequenceFactory* pCSF = pGeoFa->getCoordinateSequenceFactory();
	//m_pCoordinateSequence = m_pGeosGeomtry->getCoordinates();
	//const geos::geom::Geometry* pGeom = m_pGeosGeomtry->getGeometryN(0);
	const geos::geom::Polygon* pPolygon = dynamic_cast<geos::geom::Polygon*>(m_pGeosGeomtry.get());
	//std::size_t csSize = m_pCoordinateSequence->getSize();
	//if(!pPolygon) return;
	/////////////////////////////////////获得外边框///////////////////////////////////////
	//const geos::geom::LineString* pExterRing = pPolygon->getExteriorRing();
	//geom::CoordinateSequence* pExterRingCoordinateSequence = pExterRing->getCoordinates();
	//
	/////////////////////////////////////获得内洞/////////////////////////////////////////
	//for(size_t i = 0; i < pPolygon->getNumInteriorRing(); i++)
	//{
	//	const geos::geom::LineString* pInnerRing = pPolygon->getInteriorRingN(i);
	//	geom::CoordinateSequence* pInnerRingCoordinateSequence = pInnerRing->getCoordinates();
	//}
	/////////////////////////////////////创建OSG图形///////////////////////////////////////
	osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
	////OutputDebugStringA("----------------------------------/r/n");
	//for(std::size_t scs = 0; scs < csSize; scs++)
	//{
	//	srcVec3.set((*m_pCoordinateSequence)[scs].x,
	//	(*m_pCoordinateSequence)[scs].y, (*m_pCoordinateSequence)[scs].z);

	//	//std::ostringstream ostr;
	//	//ostr<<srcVec3.x()<<", "<<srcVec3.y()<<std::endl;

	//	//OutputDebugStringA(ostr.str().data());

	//	m_pCorrdinateTransform->Transform(srcVec3, destVec3);
	//	double x = destVec3.x();
	//	double y = destVec3.y();
	//	double z = 0;
	//	if(m_bUserLayerZ)
	//		z = m_pParentLayer->LayerZ();
	//	else
	//		z = srcVec3.z();

	//	destVec3.set(x,y,z);
	//	pyramidVertices->push_back(destVec3);
	//}
	{
		const geos::geom::LineString* pExterRing = pPolygon->getExteriorRing();
		for(size_t i = 0; i < pExterRing->getNumPoints(); i++)
		{
			geos::geom::Point* point = pExterRing->getPointN(i);
			srcVec3 = osg::Vec3(point->getX(), point->getY(), point->getCoordinate()->z);
			m_pCorrdinateTransform->Transform(srcVec3, destVec3);
			double x = destVec3.x();
			double y = destVec3.y();
			double z = destVec3.z();
			if(m_bUserLayerZ)
				y = m_pParentLayer->LayerZ();
			/*else
				y = srcVec3.y();*/

			destVec3.set(x,y,z);
			pyramidVertices->push_back(destVec3);
		}
		m_pOsgGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, pyramidVertices->size()));
	}


		for (size_t i = 0; i < pPolygon->getNumInteriorRing(); i++)
		{
			const geos::geom::LineString *ring = pPolygon->getInteriorRingN(i);
			for (size_t j = 0; j < ring->getNumPoints(); j++) 
			{
				geos::geom::Point* point = ring->getPointN(j);
				srcVec3 = osg::Vec3(point->getX(), point->getY(), point->getCoordinate()->z);

				TransormPoint(srcVec3, destVec3);

				pyramidVertices->push_back(destVec3);
			}
			m_pOsgGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, pyramidVertices->size()-ring->getNumPoints() , ring->getNumPoints()));
		}


	m_pOsgGeometry->setVertexArray(pyramidVertices);
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
	//m_pOsgGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,csSize)); 
	osgUtil::Tessellator tsl;
	tsl.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
	tsl.setBoundaryOnly(false);
	tsl.retessellatePolygons(*m_pOsgGeometry);

	osg::Vec3Array* array = triangulizeGeometry(m_pOsgGeometry);
	m_pOsgGeometry->setVertexArray(array);
	m_pOsgGeometry->removePrimitiveSet(0,m_pOsgGeometry->getNumPrimitiveSets());
	m_pOsgGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, array->size()));
	//osg::ref_ptr<osgUtil::Tessellator> tes = new osgUtil::Tessellator();  
	//tes->setBoundaryOnly(false);  
	//tes->setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);  
	//tes->setTessellationType(osgUtil::Tessellator::TESS_TYPE_POLYGONS);  
	//tes->retessellatePolygons(*m_pOsgGeometry);


	geode->addDrawable(m_pOsgGeometry);
}

struct TriangulizeFunctor 
{
	osg::Vec3Array* _vertexes;

	// do nothing
	void operator ()(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool treatVertexDataAsTemporary) {
		_vertexes->push_back(v1);
		_vertexes->push_back(v2);
		_vertexes->push_back(v3);
	}
};
osg::Vec3Array* goPolygon::triangulizeGeometry(osg::Geometry* src)
{
	if (src->getNumPrimitiveSets() == 1 && 
		src->getPrimitiveSet(0)->getType() == osg::PrimitiveSet::DrawArraysPrimitiveType &&
		src->getVertexArray() &&
		src->getVertexArray()->getType() == osg::Array::Vec3ArrayType)
		return static_cast<osg::Vec3Array*>(src->getVertexArray());

	osg::TriangleFunctor<TriangulizeFunctor> functor;
	osg::Vec3Array* array = new osg::Vec3Array;
	functor._vertexes = array;
	src->accept(functor);
	return array;
}

void goPolygon::SetCoordinateTransform( CoordinateTransform* pCT )
{
	m_pCorrdinateTransform = pCT;
}

void goPolygon::TransformGeometry()
{
	osg::Vec3 srcVec3, destVec3;
	geom::CoordinateSequence* pCoordinateSequence = m_pGeosGeomtry->getCoordinates();
	std::size_t csSize = pCoordinateSequence->getSize();

	osg::Vec3Array* pyramidVertices  = dynamic_cast<osg::Vec3Array*>(m_pOsgGeometry->getVertexArray());
	pyramidVertices->clear();

	for (size_t scs = 0; scs<csSize; scs++)
	{
		srcVec3.set((*pCoordinateSequence)[scs].x,	(*pCoordinateSequence)[scs].y, (*pCoordinateSequence)[scs].z);

		TransormPoint(srcVec3, destVec3);

		pyramidVertices->push_back(destVec3);
	}

	m_pOsgGeometry->dirtyDisplayList();
	m_pOsgGeometry->dirtyBound();
	
	/*osg::ref_ptr<osgUtil::Tessellator> tes = new osgUtil::Tessellator();  
	tes->setBoundaryOnly(false);  
	tes->setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);  
	tes->setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);  
	tes->retessellatePolygons(*m_pOsgGeometry); */
}

void goPolygon::operator=( const goPolygon& other )
{
	this->m_pGeosGeomtry.reset(other.m_pGeosGeomtry->clone());
}

GOTO_NAMESPACE_END
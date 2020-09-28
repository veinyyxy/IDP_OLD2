#include <geos/io/WKTReader.h>
#include <ogr_geometry.h>

#include <osg/Geometry>
#include <osg/TriangleFunctor>
#include <osg/Point>
#include <osg/LineWidth>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>

#include <osgUtil/Tessellator>

#include "goGeometryFactory.h"

GOTO_NAMESPACE_START
goGeometryFactory* goGeometryFactory::g_goGeometryFactory = 0;

goGeometryFactory::goGeometryFactory(void)
{
	m_pGeometryFactory = geos::geom::GeometryFactory::getDefaultInstance();
	m_pPrecisionModel = m_pGeometryFactory->getPrecisionModel();
	m_pCoordinateSequenceFactory = geos::geom::CoordinateArraySequenceFactory::instance();
}


goGeometryFactory::~goGeometryFactory(void)
{
}

goGeometryFactory* goGeometryFactory::Instance()
{
	if(!g_goGeometryFactory)
	{
		g_goGeometryFactory = new goGeometryFactory();
	}
	return g_goGeometryFactory;
}

geos::geom::Geometry* goGeometryFactory::CreateGeosGeometry( const osg::Vec3Array* value , geos::geom::GeometryTypeId geosTypeId )
{
	switch(geosTypeId)
	{
	case geos::geom::GEOS_POINT:
		{
			if (value->size() == 1 ) 
			{
				geos::geom::Coordinate coord = getCoordinate(value->at(0));
				return m_pGeometryFactory->createPoint(coord);
			}
			else if (value->size() > 1)
			{
				return m_pGeometryFactory->createMultiPoint(*getCoordinates(value));
			}
		}break;
	case geos::geom::GEOS_LINESTRING:
		{
			if (value->size() != 0) 
			{
				return m_pGeometryFactory->createLineString(getCoordinates(value));
			}
			return m_pGeometryFactory->createLineString();
		}break;
	case  geos::geom::GEOS_LINEARRING:
		{
			geos::geom::CoordinateSequence* coordinates = getCoordinates(value);
			return m_pGeometryFactory->createLinearRing(coordinates);
		}break;
	case  geos::geom::GEOS_POLYGON:
		{
			int length = value->size();
			if (length > 0) 
			{
				geos::geom::CoordinateSequence* coordinates = getCoordinates(value);
				geos::geom::LinearRing* pLinearRing =  m_pGeometryFactory->createLinearRing(coordinates);
				return m_pGeometryFactory->createPolygon(pLinearRing, 0);
			}
			return m_pGeometryFactory->createPolygon();
		}break;
	default:
		std::cout<<"geos::geom::GeometryTypeId Error!";
		break;
	}
	return 0;
}

osg::Geometry* goGeometryFactory::CreateOsgGeometryFromGeosGeomerty( const geos::geom::Geometry* pGeometry ,const osg::Vec4f& color ,  int size/*= 1*/)
{
	osg::Geometry* geometry = new osg::Geometry();
	osg::Vec3Array* v = GetVertexArray2GeosGemetry(pGeometry);
	geometry->setVertexArray(v);
	osg::Vec4 c[] = { color };
	geometry->setColorArray(new osg::Vec4Array(1, c));
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	geos::geom::GeometryTypeId geosTypeId= pGeometry->getGeometryTypeId();
	osg::PrimitiveSet::Mode psMode;
	switch(geosTypeId)
	{
	case geos::geom::GEOS_POINT:
		{
			psMode = osg::PrimitiveSet::POINTS;
		}break;
	case geos::geom::GEOS_LINESTRING:
		{
		psMode = osg::PrimitiveSet::LINE_STRIP;	
		}break;
	case  geos::geom::GEOS_LINEARRING:
		{
			psMode = osg::PrimitiveSet::LINE_LOOP;
		}break;
	case  geos::geom::GEOS_POLYGON:
		{
			psMode = osg::PrimitiveSet::POLYGON;
		}break;
	}
	if(size != 1)
	{
		if (psMode == osg::PrimitiveSet::POINTS)
		{
			osg::StateSet* ss = geometry->getOrCreateStateSet();
			osg::Point* p = new osg::Point();
			p->setSize(size);         
			ss->setAttributeAndModes(p, osg::StateAttribute::ON); 
		}
		else if (psMode == osg::PrimitiveSet::LINES || psMode == osg::PrimitiveSet::LINE_STRIP || psMode == osg::PrimitiveSet::LINE_LOOP)
		{
			osg::StateSet* ss = geometry->getOrCreateStateSet();
			osg::LineWidth* lw = new osg::LineWidth(); 
			lw->setWidth(size); 
			ss->setAttributeAndModes(lw, osg::StateAttribute::ON); 
		}
	}
	geometry->addPrimitiveSet(new osg::DrawArrays(psMode, 0, v->size())); 
	return geometry;
}

osg::Geometry* goGeometryFactory::CreateOsgGeometry( const osg::Vec3Array* value,const osg::Vec4f& color ,osg::PrimitiveSet::Mode psMode, int size /*= 1*/ )
{
	osg::Geometry* geometry = new osg::Geometry();

	geos::geom::Geometry*  pGeosGeomtry = CreateGeosGeometry(value,geos::geom::GEOS_POINT);
	osg::Vec3Array* v = GetVertexArray2GeosGemetry(pGeosGeomtry);
	geometry->setVertexArray(v);

	osg::Vec4 c[] = { color };
	geometry->setColorArray(new osg::Vec4Array(1, c));
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	
	if(size != 1)
	{
		if (psMode == osg::PrimitiveSet::POINTS)
		{
			osg::StateSet* ss = geometry->getOrCreateStateSet();
			osg::Point* p = new osg::Point();
			p->setSize(size);         
			ss->setAttributeAndModes(p, osg::StateAttribute::ON); 
		}
		else if (psMode == osg::PrimitiveSet::LINES || psMode == osg::PrimitiveSet::LINE_STRIP || psMode == osg::PrimitiveSet::LINE_LOOP)
		{
			osg::StateSet* ss = geometry->getOrCreateStateSet();
			osg::LineWidth* lw = new osg::LineWidth(); 
			lw->setWidth(size); 
			ss->setAttributeAndModes(lw, osg::StateAttribute::ON); 
		}
	}
	geometry->addPrimitiveSet(new osg::DrawArrays(psMode, 0, v->size())); 
	return geometry;
}


geos::geom::Coordinate goGeometryFactory::getCoordinate(osg::Vec3f value) 
{
	using geos::geom::Dimension;
	geos::geom::Coordinate coord;
	coord.x = value.x();
	coord.y = value.y();
	coord.z = value.z();
	return coord;
}

geos::geom::CoordinateSequence* goGeometryFactory::getCoordinates( const osg::Vec3Array* value )
{
	int length = value->size();
	geos::geom::CoordinateSequence* sequence = m_pCoordinateSequenceFactory->create(length, 3);
	for (int i = 0; i < length; i++) 
	{
		sequence->setAt(getCoordinate(value->at(i)), (std::size_t)i);
	}
	return sequence;
}

geos::geom::Geometry* goGeometryFactory::ReadWtkString(const std::string& wtk )
{
	geos::io::WKTReader wktReader;
	return wktReader.read(wtk);
}

osg::Vec3Array* goGeometryFactory::GetVertexArray2GeosGemetry( const geos::geom::Geometry* geometry )
{
	const geos::geom::Geometry*  pGeosGeomtry = geometry;
	std::size_t  fcsSize  =  pGeosGeomtry->getNumGeometries();

	osg::Vec3 srcVec3;
	osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
	for(std::size_t fscs = 0; fscs < fcsSize; fscs++)
	{
		const geos::geom::Geometry* geom = pGeosGeomtry->getGeometryN(fscs);
		geos::geom::CoordinateSequence* pCoordinateSequence = geom->getCoordinates();
		std::size_t  csSize =  pCoordinateSequence->getSize();
		for(std::size_t scs = 0; scs < csSize; scs++)
		{
			srcVec3.set((*pCoordinateSequence)[scs].x,(*pCoordinateSequence)[scs].y, (*pCoordinateSequence)[scs].z);
			pyramidVertices->push_back(srcVec3);
		}
	}
	return pyramidVertices;
}
//////////////////////////////////////////////////////////////////////////
osg::Vec3Array* goGeometryFactory::TriangulizeGeometry( osg::Geometry* src )
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

osg::Geometry* goGeometryFactory::OGRGeometryToGeometry(OGRGeometry* ogrGeometry ,CoordinateTransform* coord /*= 0*/, int z /*= 0*/, osg::Vec4f color /*= osg::Vec4f(1.0,1.0,1.0,1.0)*/ )
{
	osg::Geometry* osgGeometry = 0;
	 bool disableCulling = false;
	switch(ogrGeometry->getGeometryType()) 
	{
	case wkbPoint:
	case wkbPoint25D:
		// point to drawable
		osgGeometry = OGRPointsToGeometry(static_cast<OGRPoint *>(ogrGeometry) , coord,z,color);
		disableCulling = true;
		break; 

	case wkbLinearRing:
		osgGeometry = OGRLinearRingToGeometry(static_cast<OGRLinearRing *>(ogrGeometry) ,coord,z,color);
		break;

	case wkbLineString:
	case wkbLineString25D:
		osgGeometry = OGRLineStringToGeometry(static_cast<OGRLineString*>(ogrGeometry) ,coord,z,color );
		break;

	case wkbPolygon:
	case wkbPolygon25D:
		osgGeometry = OGRPolygonToGeometry(static_cast<OGRPolygon*>(ogrGeometry) ,coord,z,color );
		break;

	case wkbMultiPoint:
	case wkbMultiPoint25D:
		osgGeometry = OGRMultiPointToGeometry(static_cast<OGRMultiPoint*>(ogrGeometry) ,coord,z,color);
		disableCulling = true;
		break;

	case wkbMultiLineString:
	case wkbMultiLineString25D:
		osgGeometry = OGRMultiLineStringToGeometry(static_cast<OGRMultiLineString*>(ogrGeometry) ,coord,z,color);
		break;

	case wkbMultiPolygon:
	case wkbMultiPolygon25D:
		osgGeometry = OGRMultiPolygonToGeometry(static_cast<OGRMultiPolygon*>(ogrGeometry),coord,z,color);
		break;

	case wkbUnknown:
	default:
		OSG_WARN << "Unknown WKB Geometry " << std::endl;
		break;
	}
	return osgGeometry;
}
//点
osg::Geometry* goGeometryFactory::OGRPointsToGeometry(const OGRPoint* points ,CoordinateTransform* coord /*= 0*/, int z /*= 0*/, osg::Vec4f color /*= osg::Vec4f(1.0,1.0,1.0,1.0)*/ )
{
	osg::Geometry* pointGeom = new osg::Geometry();
	osg::Vec3Array* vertices = new osg::Vec3Array();
	vertices->push_back(osg::Vec3(points->getX(), points->getY(), points->getZ()));
	if (coord)
	{
		vertices = VertexArrayTransform(coord ,vertices,z);
	}
	pointGeom->setVertexArray(vertices);
	pointGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, 1));
	return pointGeom;
}

//多点
osg::Geometry* goGeometryFactory::OGRMultiPointToGeometry(OGRMultiPoint* mpoint ,CoordinateTransform* coord /*= 0*/, int z /*= 0*/, osg::Vec4f color /*= osg::Vec4f(1.0,1.0,1.0,1.0)*/ )
{
	osg::Geometry* geom = new osg::Geometry;

	osg::Geometry* pointGeom = new osg::Geometry();
	osg::Vec3Array* vertices = new osg::Vec3Array();

	vertices->reserve(mpoint->getNumGeometries());
	for (int i = 0; i < mpoint->getNumGeometries(); i++ ) 
	{
		OGRGeometry* ogrGeom = mpoint->getGeometryRef(i);
		OGRwkbGeometryType ogrGeomType = ogrGeom->getGeometryType();

		if (wkbPoint != ogrGeomType && wkbPoint25D != ogrGeomType)
			continue; // skip

		OGRPoint* points = static_cast<OGRPoint*>(ogrGeom);

		vertices->push_back(osg::Vec3(points->getX(), points->getY(), points->getZ()));
	}
	if (coord)
	{
		vertices = VertexArrayTransform(coord ,vertices,z);
	}
	pointGeom->setVertexArray(vertices);
	pointGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, vertices->size()));

	if (pointGeom->getVertexArray())
	{
		OSG_INFO << "osgOgrFeature::multiPointToDrawable " << geom->getVertexArray()->getNumElements() << " vertexes"<< std::endl;
	}
	return pointGeom;
}

//linearRing
osg::Geometry* goGeometryFactory::OGRLinearRingToGeometry(OGRLinearRing* ring ,CoordinateTransform* coord /*= 0*/, int z /*= 0*/, osg::Vec4f color /*= osg::Vec4f(1.0,1.0,1.0,1.0)*/ )
{
	osg::Geometry* contourGeom = new osg::Geometry();
	osg::Vec3Array* vertices = new osg::Vec3Array();
	OGRPoint point;
	for(int j = 0; j < ring->getNumPoints(); j++)
	{
		ring->getPoint(j, &point);
		vertices->push_back(osg::Vec3(point.getX(), point.getY(),point.getZ()));
	}
	if (coord)
	{
		vertices = VertexArrayTransform(coord ,vertices,z);
	}
	contourGeom->setVertexArray(vertices);
	contourGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vertices->size()));
	return contourGeom;
}

//lineString
osg::Geometry* goGeometryFactory::OGRLineStringToGeometry(OGRLineString* lineString ,CoordinateTransform* coord /*= 0*/, int z /*= 0*/, osg::Vec4f color /*= osg::Vec4f(1.0,1.0,1.0,1.0)*/ )
{
	osg::Geometry* contourGeom = new osg::Geometry();
	osg::Vec3Array* vertices = new osg::Vec3Array();
	OGRPoint point;
	for(int j = 0; j < lineString->getNumPoints(); j++) 
	{
		lineString->getPoint(j, &point);
		vertices->push_back(osg::Vec3(point.getX(), point.getY(), point.getZ()));
	}
	if (coord)
	{
		vertices = VertexArrayTransform(coord ,vertices,z);
	}
	contourGeom->setVertexArray(vertices);
	contourGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices->size()));
	return contourGeom;
}

//多线
osg::Geometry* goGeometryFactory::OGRMultiLineStringToGeometry(OGRMultiLineString* mlineString,CoordinateTransform* coord /*= 0*/, int z /*= 0*/, osg::Vec4f color /*= osg::Vec4f(1.0,1.0,1.0,1.0)*/ )
{
	osg::Geometry* geom = new osg::Geometry;

	for (int i = 0; i < mlineString->getNumGeometries(); i++ ) 
	{
		OGRGeometry* ogrGeom = mlineString->getGeometryRef(i);
		OGRwkbGeometryType ogrGeomType = ogrGeom->getGeometryType();

		if (wkbLineString != ogrGeomType && wkbLineString25D != ogrGeomType)
			continue; // skip

		OGRLineString* lineString = static_cast<OGRLineString*>(ogrGeom);
		osg::ref_ptr<osg::Geometry> geometry = OGRLineStringToGeometry(lineString);
		if (geometry.valid() && 
			geometry->getVertexArray() && 
			geometry->getNumPrimitiveSets() &&
			geometry->getVertexArray()->getType() == osg::Array::Vec3ArrayType) 
		{

			if (!geom->getVertexArray())
			{
				geom->setVertexArray(geometry->getVertexArray());
				geom->setPrimitiveSetList(geometry->getPrimitiveSetList());

			} 
			else 
			{
				int size = geom->getVertexArray()->getNumElements();

				osg::Vec3Array* arraySrc = static_cast<osg::Vec3Array*>(geometry->getVertexArray());
				osg::Vec3Array* arrayDst = static_cast<osg::Vec3Array*>(geom->getVertexArray());
				arrayDst->insert(arrayDst->end(), arraySrc->begin(), arraySrc->end());
				// shift index 
				if (coord)
				{
					osg::Vec3Array* array = (osg::Vec3Array* )geom->getVertexArray();
					array = VertexArrayTransform(coord ,array,z);
					geom->setVertexArray(array);
				}
				geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, size, arraySrc->size()));
			}
		}
	}
	return geom;
}

//多边形
osg::Geometry* goGeometryFactory::OGRPolygonToGeometry(OGRPolygon* polygon,CoordinateTransform* coord /*= 0*/, int z /*= 0*/, osg::Vec4f color /*= osg::Vec4f(1.0,1.0,1.0,1.0)*/ )
{
	osg::Geometry* geom = new osg::Geometry();
	osg::Vec3Array* vertices = new osg::Vec3Array();
	geom->setVertexArray(vertices);
	{
		OGRLinearRing *ring = polygon->getExteriorRing();
		OGRPoint point;
		for(int i = 0; i < ring->getNumPoints(); i++)
		{
			ring->getPoint(i, &point);
			vertices->push_back(osg::Vec3(point.getX(), point.getY(), point.getZ()));
		}
		if (coord)
		{
			vertices = VertexArrayTransform(coord ,vertices,z);
		}
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vertices->size()));
	}

	if (polygon->getNumInteriorRings()) 
	{
		for (int i = 0; i < polygon->getNumInteriorRings(); i++)
		{
			OGRLinearRing *ring = polygon->getInteriorRing(i);
			OGRPoint point;
			for (int j = 0; j < ring->getNumPoints(); j++) 
			{
				ring->getPoint(j, &point);
				vertices->push_back(osg::Vec3(point.getX(), point.getY(), point.getZ()));
			}
			if (coord)
			{
				vertices = VertexArrayTransform(coord ,vertices,z);
			}
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, vertices->size()-ring->getNumPoints() , ring->getNumPoints()));
		}
	}
	osgUtil::Tessellator tsl;
	tsl.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
	tsl.setBoundaryOnly(false);
	tsl.retessellatePolygons(*geom);

	osg::Vec3Array* array = TriangulizeGeometry(geom);
	geom->setVertexArray(array);
	geom->removePrimitiveSet(0,geom->getNumPrimitiveSets());
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, array->size()));

	return geom;
}
//多 多边形
osg::Geometry* goGeometryFactory::OGRMultiPolygonToGeometry(OGRMultiPolygon* mpolygon ,CoordinateTransform* coord /*= 0*/, int z /*= 0*/, osg::Vec4f color /*= osg::Vec4f(1.0,1.0,1.0,1.0)*/ )
{
	osg::Geometry* geom = new osg::Geometry;

	for (int i = 0; i < mpolygon->getNumGeometries(); i++ ) 
	{
		OGRGeometry* ogrGeom = mpolygon->getGeometryRef(i);
		OGRwkbGeometryType ogrGeomType = ogrGeom->getGeometryType();

		if (wkbPolygon != ogrGeomType && wkbPolygon25D != ogrGeomType)
			continue; // skip

		OGRPolygon* polygon = static_cast<OGRPolygon*>(ogrGeom);
		osg::ref_ptr<osg::Drawable> drw = OGRPolygonToGeometry(polygon);
		osg::ref_ptr<osg::Geometry> geometry = drw->asGeometry();
		if (geometry.valid() && geometry->getVertexArray() && 
			geometry->getVertexArray()->getNumElements() && 
			geometry->getNumPrimitiveSets() &&
			geometry->getVertexArray()->getType() == osg::Array::Vec3ArrayType ) 
		{

			if (!geom->getVertexArray()) 
			{ // no yet data we put the first in
				geom->setVertexArray(geometry->getVertexArray());
				geom->setPrimitiveSetList(geometry->getPrimitiveSetList());

			} 
			else 
			{ // already a polygon then append
				int size = geom->getVertexArray()->getNumElements();
				osg::Vec3Array* arrayDst = static_cast<osg::Vec3Array*>(geom->getVertexArray());
				osg::ref_ptr<osg::Vec3Array> triangulized = TriangulizeGeometry(geometry.get());
				if (triangulized.valid()) 
				{
					arrayDst->insert(arrayDst->end(), triangulized->begin(), triangulized->end());
					// shift index 
					if (coord)
					{
						triangulized = VertexArrayTransform(coord ,triangulized,z);
					}
					geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, size, triangulized->size()));
				}
			}
		} 
		else
		{
			OSG_WARN << "Warning something wrong with a polygon in a multi polygon" << std::endl;
		}
	}

	if (geom->getVertexArray())
	{
		OSG_INFO << "osgOgrFeature::multiPolygonToDrawable " << geom->getVertexArray()->getNumElements() << " vertexes"<< std::endl;
	}

	return geom;
}

osg::Vec3Array* goGeometryFactory::VertexArrayTransform(CoordinateTransform* coord , osg::Vec3Array* srcArray , int z)
{
	osg::Vec3Array* vertices = new osg::Vec3Array;
	for(size_t i = 0 ; i< vertices->size(); i++)
	{
		osg::Vec3 _srcVec3, _destVec3;
		_srcVec3 = vertices->at(i);
		if (coord)
		{
			coord->Transform(_srcVec3, _destVec3);
		}
		else
		{
			_destVec3 = vertices->at(i);
		}
		double x = _destVec3.x();
		double y = _destVec3.y();
		double _z = _srcVec3.z();
		if(z != 0) //false
			_z = z;
		_destVec3.set(x,y,_z);
		vertices->push_back(_destVec3);
	}
	return vertices;
}



//////////////////////////////////////////////////////////////////////////

GOTO_NAMESPACE_END
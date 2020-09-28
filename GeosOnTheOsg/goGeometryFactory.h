#pragma once
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/PrecisionModel.h>

#include <geos/geom/Point.h>
#include <geos/geom/MultiPoint.h>

#include <geos/geom/LineString.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Polygon.h>

#include "goConfig.h"
#include "CoordinateTransform.h"

GOTO_NAMESPACE_START

class GEOSONTHEOSG_EXPORT goGeometryFactory
{
public:

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

	virtual ~goGeometryFactory(void);

	static goGeometryFactory* Instance();
public:
	//直接新建 osg::Geometry
	osg::Geometry* CreateOsgGeometry(const osg::Vec3Array* value,const osg::Vec4f& color ,osg::PrimitiveSet::Mode psMode , int size = 1);
	//直接新建 从 geos::geom::Geometry 中的到顶点数组 然后给颜色和大小 建立osg::Geometry
	osg::Geometry* CreateOsgGeometryFromGeosGeomerty(const geos::geom::Geometry* pGeometry,const osg::Vec4f& color ,  int size = 1);

	//从Wtk字符串 建立 geos::geom::Geometry
	geos::geom::Geometry* ReadWtkString(const std::string& wtk);

	//直接新建 从 geos::geom::Geometry 中的到顶点数组 然后给定图元类型
	geos::geom::Geometry* CreateGeosGeometry(const osg::Vec3Array* value , geos::geom::GeometryTypeId geosTypeId);

private:
	geos::geom::Coordinate getCoordinate(const osg::Vec3f value);
	geos::geom::CoordinateSequence* getCoordinates(const osg::Vec3Array* value);

	//顶点数组  从 geos::geom::Geometry 获得
	osg::Vec3Array* GetVertexArray2GeosGemetry(const geos::geom::Geometry* geometry);

	/////////////////////////////////////osg 官方/////////////////////////////////////
	osg::Geometry* OGRGeometryToGeometry(OGRGeometry* ogrGeometry,CoordinateTransform* coord = 0 , int z = 0, osg::Vec4f color = osg::Vec4f(1.0,1.0,1.0,1.0)) ;
private:
	goGeometryFactory(void);
	goGeometryFactory(goGeometryFactory&){;}
	void operator=(goGeometryFactory&){}
	//////////////////////////////////////////////////////////////////////////
	osg::Geometry* OGRPointsToGeometry(const OGRPoint* points ,CoordinateTransform* coord = 0, int z = 0, osg::Vec4f color = osg::Vec4f(1.0,1.0,1.0,1.0)) ;
	osg::Geometry* OGRMultiPointToGeometry(OGRMultiPoint* mpoint ,CoordinateTransform* coord = 0, int z = 0, osg::Vec4f color = osg::Vec4f(1.0,1.0,1.0,1.0)) ;
	
	osg::Geometry* OGRLineStringToGeometry(OGRLineString* lineString ,CoordinateTransform* coord = 0, int z = 0, osg::Vec4f color = osg::Vec4f(1.0,1.0,1.0,1.0)) ;
	osg::Geometry* OGRLinearRingToGeometry(OGRLinearRing* ring ,CoordinateTransform* coord = 0, int z = 0, osg::Vec4f color = osg::Vec4f(1.0,1.0,1.0,1.0)) ;
	osg::Geometry* OGRMultiLineStringToGeometry(OGRMultiLineString* mlineString,CoordinateTransform* coord = 0, int z = 0, osg::Vec4f color = osg::Vec4f(1.0,1.0,1.0,1.0)) ;

	
	osg::Geometry* OGRPolygonToGeometry(OGRPolygon* polygon,CoordinateTransform* coord = 0, int z = 0, osg::Vec4f color = osg::Vec4f(1.0,1.0,1.0,1.0)) ;
	osg::Geometry* OGRMultiPolygonToGeometry(OGRMultiPolygon* mpolygon,CoordinateTransform* coord = 0, int z = 0, osg::Vec4f color = osg::Vec4f(1.0,1.0,1.0,1.0)) ;
	
	osg::Vec3Array* TriangulizeGeometry(osg::Geometry* src);
	osg::Vec3Array* VertexArrayTransform(CoordinateTransform* coord , osg::Vec3Array* srcArray , int z);
	//////////////////////////////////////////////////////////////////////////

	const geos::geom::GeometryFactory* m_pGeometryFactory;
	const geos::geom::PrecisionModel* m_pPrecisionModel;
	const geos::geom::CoordinateSequenceFactory* m_pCoordinateSequenceFactory;

	static goGeometryFactory* g_goGeometryFactory;
};

GOTO_NAMESPACE_END
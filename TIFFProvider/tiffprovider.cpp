#include "tiffprovider.h"
#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>
#include <osgUtil/LineSegmentIntersector>
#include <osgEarth/MapNode>
#include <osgEarth/TerrainEngineNode>
#include <osgEarth/ElevationQuery>
#include <osgEarth/StringUtils>
#include <osgEarth/Terrain>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/Controls>
#include <osgEarthUtil/LatLongFormatter>
#include <iomanip>
#include <osgDB/WriteFile>

using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;
using namespace osgDB;

goto_gis::DataProvider* g_TIFFProvider = 0;
extern "C"
{
	DataProvider Q_DECL_EXPORT * DataProviderPluginMain()
	{
		g_TIFFProvider = new TIFFProvider;
		return g_TIFFProvider;
	}
}

TIFFProvider::TIFFProvider()
{

}

TIFFProvider::~TIFFProvider()
{

}

bool TIFFProvider::LoadFile( const std::string& filename )
{
	ReaderWriter::ReadResult result = readimage.local_readImage(filename,NULL);
	osg::Image* img = result.getImage();
	GeoRef ref = readimage.GetGeoRef();
	int width = ref.m_width;
	int height = ref.m_height;
	//osg::Image* subimg = GetSubImage(img,ref,osg::Vec3(80,20,0),osg::Vec3(90,30,0));
	//osgDB::writeImageFile(*subimg,"e:\abc2.jpg");
	osg::Geode* node = DumpImageWithGeoRef(m_pParentLayer,img,readimage.GetGeoRef());
	if (m_pParentLayer)
	{
		osg::Group* pGroup = m_pParentLayer->GetLayerNode()->asGroup();
		if(!pGroup)
		{
			pGroup = new osg::Group;
			m_pParentLayer->SetLayerNode(pGroup);
		}
	    pGroup->addChild(node);
		return true;
	}
	else
	{
		return false;
	}
}

osg::Image* TIFFProvider::GetSubImage(osg::Image* img,GeoRef ref,osg::Vec3 posBegin,osg::Vec3 PosEnd)
{
	osg::Image* subimg = new osg::Image;
	if (posBegin.x()>=ref.flonmin && PosEnd.x() < ref.flonmax && 
		posBegin.y()>=ref.flatmin && PosEnd.y() < ref.flatmax &&
		PosEnd.x()>posBegin.x() && PosEnd.y()>posBegin.y())
	{
	//	int index
	//	subimg->copySubImage(500,500,0,img);	
		double DistanceperPixelX = (ref.flonmax-ref.flonmin)/ref.m_width;
		double DistanceperPixelY = (ref.flatmax-ref.flatmin)/ref.m_height;
		unsigned int width,height;
		width = DistanceperPixelX * (PosEnd.x()-posBegin.x());
		height = DistanceperPixelY * (PosEnd.y()-posBegin.y());
		//unsigned char* imageData = new unsigned char[width * height * 4/*RGBA*/];
	}

	return subimg;
}

osg::Geode* TIFFProvider::DumpImageWithGeoRef(goto_gis::Layer *layer,osg::Image* img,GeoRef ref)
{
	osg::Geode* geode = new osg::Geode();

	float dx,dy;
	dx = (ref.flonmax - ref.flonmin);
	dy = (ref.flatmax - ref.flatmin);
	osg::Vec3 src1,src2,dst1,dst2,origin;
	origin = src1 = osg::Vec3(ref.flonmin,ref.flatmin,layer->LayerZ());
	LonLatToGL(layer, src1, dst1);

	unsigned int numColumns = 100;
	unsigned int numRows = 100;
	unsigned int r;
	unsigned int c;

	// ---------------------------------------
	// Set up a StateSet to texture the objects
	// ---------------------------------------
	osg::StateSet* stateset = new osg::StateSet(); 

	if (img)
	{
		//unsigned char* pdata = pimage->data(10,10);
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(img);
		stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON); 
	}

	geode->setStateSet( stateset );	

	// compute z range of z values of grid data so we can scale it.
	float min_z = FLT_MAX;
	float max_z = -FLT_MAX;

	{
		osg::Geometry* geometry = new osg::Geometry;

		osg::Vec3Array& v = *(new osg::Vec3Array(numColumns*numRows));
		osg::Vec2Array& t = *(new osg::Vec2Array(numColumns*numRows));
		osg::Vec4ubArray& color = *(new osg::Vec4ubArray(1));

		color[0].set(255,255,255,255);

		//	float rowCoordDelta = size.y()/(float)(numRows-1);
		//	float columnCoordDelta = size.x()/(float)(numColumns-1);

		float rowTexDelta = 1.0f/(float)(numRows-1);
		float columnTexDelta = 1.0f/(float)(numColumns-1);

		//	osg::Vec3 pos = origin;
		osg::Vec3 SrcPos,DesPos;

		osg::Vec2 tex(0.0f,0.0f);
		int vi=0;
		SrcPos = origin;
		DesPos = dst1;
		for(r=0;r<numRows;++r)
		{
			//pos.x() = origin.x();
			SrcPos.x() = origin.x();
			tex.x() = 0.0f;
			for(c=0;c<numColumns;++c)
			{
				LonLatToGL(layer, SrcPos, DesPos);
				v[vi].set(DesPos.x(),DesPos.y(),DesPos.z()); //+(vertex[r+c*numRows][2]-min_z)*scale_z
				t[vi].set(tex.x(),tex.y());
				//pos.x()+=columnCoordDelta;	
				SrcPos.x() += dx/float(numColumns-1);
				tex.x()+=columnTexDelta;
				++vi;
			}
			//pos.y() += rowCoordDelta;
			SrcPos.y() += dy/float(numRows-1);
			tex.y() += rowTexDelta;
		}

		geometry->setVertexArray(&v);
		geometry->setColorArray(&color);
		geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
		geometry->setTexCoordArray(0,&t);

		for(r=0;r<numRows-1;++r)
		{
			osg::DrawElementsUShort& drawElements = *(new osg::DrawElementsUShort(GL_QUAD_STRIP,2*numColumns));
			geometry->addPrimitiveSet(&drawElements);
			int ei=0;
			for(c=0;c<numColumns;++c)
			{
				drawElements[ei++] = (r+1)*numColumns+c;
				drawElements[ei++] = (r)*numColumns+c;
			}
		}

		geode->addDrawable(geometry);

		//osgUtil::SmoothingVisitor sv;
		//sv.smooth(*geometry);
	}

	return geode;
}

osg::Vec3 TIFFProvider::LonLat2GL(goto_gis::Layer* layer, osg::Vec3& in,float zshipft)
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ() + zshipft;

	return osg::Vec3(x,y,z);
}

void TIFFProvider::LonLatToGL(goto_gis::Layer* layer, osg::Vec3& in, osg::Vec3& out )
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ();
	out.set(x,y,z);
}
#ifndef TIFFPROVIDER_H
#define TIFFPROVIDER_H

#include "tiffprovider_global.h"
#include "DataProvider.h"
#include "Layer.h"
#include "ReaderWriterGDAL.h"

using namespace goto_gis;
class ReaderWriterGDAL;

class TIFFPROVIDER_EXPORT TIFFProvider: public DataProvider
{
public:
	TIFFProvider();
	~TIFFProvider();
	bool LoadFile(const std::string& filename);
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p) {}
	virtual QString GetDataUnit(){return "";};//获得数据计量单位
	virtual float GetPositionValue(double lon,double lat,double height){return 0.0;};//获取鼠标位置的数据数值
	
private:
	osg::Geode* DumpImageWithGeoRef(goto_gis::Layer *pLayer,osg::Image* img,GeoRef ref);
	osg::Image* GetSubImage(osg::Image* img,GeoRef ref,osg::Vec3 posBegin,osg::Vec3 PosEnd);
	osg::Vec3 LonLat2GL(goto_gis::Layer* layer, osg::Vec3& in,float zshift=0.0f);
	 void LonLatToGL(goto_gis::Layer* layer, osg::Vec3& in, osg::Vec3& out);
private:
	ReaderWriterGDAL readimage;

};

#endif // TIFFPROVIDER_H

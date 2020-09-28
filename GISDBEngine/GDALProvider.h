#pragma once
#include <stdio.h>
#include <vector>
#include <map>
#include <boost/Any.hpp>
#include <windows.h>   

#include "ogrsf_frmts.h"
#include "geos.h"

#include "DataProvider.h"
#include "Layer.h"

#include "goConfig.h"

#include "goGeometry.h"
#include "goPoint.h"
#include "goLineString.h"
#include "goPolygon.h"
#include "goLinearRing.h"
#include "goMultiPoint.h"
#include "goMultiLineString.h"
#include "goMultiPolygon.h"

#include "gisdbengine_global.h"

using namespace goto_gis;
using namespace std;


class  GDALProvider : public DataProvider
{
public:
	GDALProvider();
	~GDALProvider();

	static GDALProvider *Instance();//得到实例

	bool LoadFile(const std::string& filename);
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p) {}
	bool CreateFile(const std::string& filename,OGRwkbGeometryType geomType);
	virtual QString GetDataUnit(){return "";};//获得数据计量单位
	virtual float GetPositionValue(double lon,double lat,double height){return 0.0;};//获取鼠标位置的数据数值
private:
	string Getymdhms(string format);
	void LoadField(OGRLayer *poLayer , OGRFeature *poFeature ,geom::Geometry* goGeom, goGeometry* goGeo);
	//wchar_t* CharToWchar(const char* c);
private:
	static GDALProvider * m_pGDALProvider;
	static int gcount;
};
 

GDALProvider* sqliteProvider = 0;


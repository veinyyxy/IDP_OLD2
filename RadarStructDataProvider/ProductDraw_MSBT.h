/////////////////////////////////////////////////////
// class for drawing  MSBT procuct
//create by youruibang
////////////////////////////////////////////////////
#pragma once

#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"


#define PI 3.1415927
using namespace goto_gis;

//add by ljg 20080826 for new interface for draw.
class CProductStructDraw_MSBT : public RadarStructProductionDraw
{
public:
	CProductStructDraw_MSBT();
	virtual ~CProductStructDraw_MSBT();
public:
	STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
	virtual bool      DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//画产品图片，指定某张图片
	void DrawPoint( Layer* layer , osg::Vec3 pointArray , osg::Vec4 color);
};

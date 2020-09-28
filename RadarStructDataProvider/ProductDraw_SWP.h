#pragma once

#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"


#define PI 3.1415927
using namespace goto_gis;

class CProductStructDraw_SWP :  public RadarStructProductionDraw
{
public:
	CProductStructDraw_SWP();
	virtual ~CProductStructDraw_SWP();

public:

	virtual STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	virtual bool DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//画产品图片，指定某张图片
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
	osg::Drawable* DrawText(const std::string& text,osg::Vec3 positionOffset,float size);
};


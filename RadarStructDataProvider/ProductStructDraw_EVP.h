#pragma once
#include "Struct_285_EVP_269_VWP_268_VAD.h"
#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"

#define PI 3.1415927
using namespace goto_gis;

class CProductStructDraw_EVP : public RadarStructProductionDraw
{
public:
	CProductStructDraw_EVP(void);
	~CProductStructDraw_EVP(void);

public:
	list<EVP_ELEMENTS> point_register_list;
	//osg::Vec3Array* m_pPointArray;
public:
	void Draw_EVP_Background(Layer* layer);
	void Register_Point(EVP_ELEMENTS &p);
	void DrawLine(Layer* layer , osg::Vec3 strPostion , osg::Vec3 destPostion);
	void DrawPoints(Layer* layer , osg::Vec3Array* pointArray , osg::Vec4 color);
	void DrawText(Layer* layer , QString text, osg::Vec3 postion);

	STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	virtual bool  DumpAll(Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//画产品图片，指定某张图片
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
};


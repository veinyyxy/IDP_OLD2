#pragma once

#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"


#define PI 3.1415927
using namespace goto_gis;


class CProductStructDraw_SCI :  public RadarStructProductionDraw
{
public:
	CProductStructDraw_SCI(void);
	~CProductStructDraw_SCI(void);


	virtual STRUCT_DATA_TYPE GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	virtual bool DumpAll(Layer* layer ,CProductRead* lpProduct,  P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
	void DrawText(Layer* layer, const std::string& text,osg::Vec3 positionOffset,float size );
};


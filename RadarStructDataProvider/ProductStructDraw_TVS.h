#pragma once
#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"

#define PI 3.1415927
using namespace goto_gis;

class CProductStructDraw_TVS : public RadarStructProductionDraw
{
public:
	CProductStructDraw_TVS(void);
public:
	virtual ~CProductStructDraw_TVS(void);

	public:
		STRUCT_DATA_TYPE GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
		virtual bool  DumpAll(Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
		virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
		void DrawText(Layer* layer , QString _text, osg::Vec3 postion );
};

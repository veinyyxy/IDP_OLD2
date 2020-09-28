/////////////////////////////////////////////////////
//Draw_M_Product.h 
////////////////////////////////////////////////////
#pragma once
#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"

#define PI 3.1415927
using namespace goto_gis;

//add by ljg 20080826 for new interface for draw.
class CProductStructDraw_M : public RadarStructProductionDraw
{
	public:
		CProductStructDraw_M();
		virtual ~CProductStructDraw_M();

	public:
		STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
		//virtual int  GetCountOfLayer(CProductRead* lpProduct = NULL);//判断产品绘图数量，共几张图
		virtual bool  DumpAll(Layer* layer,CProductRead* lpProduct,/* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//画产品图片，指定某张图片
		virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
		void CreatAttributeList(Layer* layer,CProductRead * pProductData,QString fileName);
		void GetStationNameAndPName(QString &fileName,QString &stationName,QString &productName);
		QStringList FileNameAnalyes( QString fName );
		osg::Drawable* DrawText(const std::string& text,osg::Vec3 positionOffset,float size);
};

#ifndef PRODUCTDRAW_TITAN_H
#define PRODUCTDRAW_TITAN_H

#include "Struct_296_TITAN.h"

#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"

using namespace goto_gis;


class CProductStructDraw_TITAN : public RadarStructProductionDraw
{
public:
	CProductStructDraw_TITAN();
	virtual ~CProductStructDraw_TITAN();

	LonLatSPCE* m_LonlatData;

	osg::Vec4 m_ClrStorm;        //识别出的风暴体信息颜色
	osg::Vec4 m_ClrStormForecast;//预报出的风暴体信息颜色
	osg::Vec4 m_ClrTrackStorm;   //风暴体追踪轨迹信息颜色
	osg::Vec4 m_ClrTrackForecast;//预报风暴体追踪轨迹信息颜色

	double m_CenterLon,m_CenterLat,m_CenterAltitude;
	//double m_ProductNumber;
	int    m_BitmapWidth,m_BitmapHeight;
	//int    m_ProductNO;
	double m_Elevation;
	double m_Resolution;
	float  m_fR_km;
	float  m_fKmPerPixel;
public:
	void DrawTitan(ProductBase* pProduct ,Layer* layer);

	void DrawOutputStorm(STITAN_OutputStorm* OutputData,Layer* layer);
	void DrawStormForecast(STITAN_StormForecast* ForecastData,Layer* layer);
	void DrawTrackStorm(STITAN_Track *TrackData,int num,Layer* layer);//追踪识别出的风暴体信息
	void DrawTrackForecast(STITAN_TrackForecast* TrackForecastData,int num,Layer* layer);//追踪预测的一小时的风暴体信息

	//int	 xWorld2DC(double x, double y);       // 经度转化为屏幕坐标  产品叠加时根据要叠加到的产品选择投影算法
	//int	 yWorld2DC(double y,double x);        // 纬度转化为屏幕坐标
	//void CaculateLayout(gFloat fRMax);
public:
	virtual STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	//virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//判断产品绘图数量，共几张图
	virtual bool DumpAll(Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable,QString filename ,int requestLayer = 0);//画产品图片，指定某张图片
	QImage DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize);

};
#endif

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

	osg::Vec4 m_ClrStorm;        //ʶ����ķ籩����Ϣ��ɫ
	osg::Vec4 m_ClrStormForecast;//Ԥ�����ķ籩����Ϣ��ɫ
	osg::Vec4 m_ClrTrackStorm;   //�籩��׷�ٹ켣��Ϣ��ɫ
	osg::Vec4 m_ClrTrackForecast;//Ԥ���籩��׷�ٹ켣��Ϣ��ɫ

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
	void DrawTrackStorm(STITAN_Track *TrackData,int num,Layer* layer);//׷��ʶ����ķ籩����Ϣ
	void DrawTrackForecast(STITAN_TrackForecast* TrackForecastData,int num,Layer* layer);//׷��Ԥ���һСʱ�ķ籩����Ϣ

	//int	 xWorld2DC(double x, double y);       // ����ת��Ϊ��Ļ����  ��Ʒ����ʱ����Ҫ���ӵ��Ĳ�Ʒѡ��ͶӰ�㷨
	//int	 yWorld2DC(double y,double x);        // γ��ת��Ϊ��Ļ����
	//void CaculateLayout(gFloat fRMax);
public:
	virtual STRUCT_DATA_TYPE GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	//virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//�жϲ�Ʒ��ͼ������������ͼ
	virtual bool DumpAll(Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable,QString filename ,int requestLayer = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
	QImage DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize);

};
#endif

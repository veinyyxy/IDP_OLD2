#pragma once
#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"

using namespace goto_gis;
class  CProductStructDraw_VAD :
	public RadarStructProductionDraw
{
public:
	CProductStructDraw_VAD(void);
	~CProductStructDraw_VAD(void);
	virtual STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	virtual bool DumpAll(Layer* layer,CProductRead* lpProduct, 
		/*CProductDrawParam* lpParam,*/P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer = 0);//画产品图片，指定某张图片
	void CreatAttributeList(Layer* layer,CProductRead * pProductData,QString fileName);
	QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);

private:
	void Draw_VAD_Background(void *p,Layer* layer,int iAbscissa[8]);//画VAD背景 
	void Draw_VAD_Line(void *p,Layer* layer, int NumbersOfStruct,
	P_COLORTABLE_VECTOR pColorTable/*,CProductDrawParam* lpParam*/);//画点和曲线
	void GetStationNameAndPName(QString &fileName,QString &stationName,QString &productName);
	QStringList FileNameAnalyes( QString fName );
	osg::Drawable* DrawText(const std::string& text,osg::Vec3 positionOffset,float size);
	//void DumpRadarProperties( CProductStructDraw_VAD * pDataProvier );
private:
	float Max_Vel;  //记录多普勒速度的最大值m/s
	int m_ID;
	//QgsFeature *feature;
	//QgsVectorLayer* qgsLayer;
};

//class Radar_STRUCT_EXPORT_IMPORT CProductStructDraw_SBT :
//	public RadarStructProductionDraw
//{
//public:
//	CProductStructDraw_SBT(void);
//	~CProductStructDraw_SBT(void);
//	virtual STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
//	virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//判断产品绘图数量，共几张图
//	virtual bool DumpAll(QRenderable* container,CProductRead* lpProduct,
//		P_COLORTABLE_VECTOR pColorTable,int requestLayer = 0);//画产品图片，指定某张图片
//};

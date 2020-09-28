///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////

//#ifndef PRODUCTSTRUCTDRAW_HI_H
//#define PRODUCTSTRUCTDRAW_HI_H

#pragma once
#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"
#include "RadProjTransform.h"
//#include"SingleProductBase.h"
//#include <wx/wx.h>

//class wxDC;
//class wxColor;
//class wxPoint;
//class wxBitmap;

//class CProductDraw_HI;
class CProductStructDraw_HI;

//class CProductDraw_HI
//{
//public:
//	CProductDraw_HI();
//	~CProductDraw_HI();
//
//public:
//	QVector4D m_ColorHail;             //可能发生冰雹 的颜色
//	QVector4D m_ColorHail_Big;        //发生冰雹的可能性很大
//	QVector4D m_ColorStrongHail;      //可能发生强冰雹
//	QVector4D m_ColorStrongHail_Big;  //发生强冰雹的可能性很大
//	//wxColor m_ColorHail;             //可能发生冰雹 的颜色
//	//wxColor m_ColorHail_Big;        //发生冰雹的可能性很大
//	//wxColor m_ColorStrongHail;      //可能发生强冰雹
//	//wxColor m_ColorStrongHail_Big;  //发生强冰雹的可能性很大
// 
//	//CProductDrawParam* m_pParam;
//
//private:
//	
//	gFloat m_POH_MinThreshold;		//冰雹最小显示阈值 （10%—100%）
//	gFloat m_POH_Threshold;			//冰雹填充阈值 （10%—100%）
//	gFloat m_POSH_MinThreshold;		//强冰雹最小显示阈值 （10%—100%）
//	gFloat m_POSH_Threshold;	    //强冰雹填充阈值 （10%—100%）
//
//	int m_size1;  //冰雹尺寸
//	int m_size2;	
//	int m_size3;
//	int m_size4;
//
//public:
////	void SetParam(CProductDrawParam* lpParam);
////	CProductDrawParam* GetParam();
//
//	void DrawHI(ProductBase* pProduct , QRenderable* container);
//	void DrawLayer(const void* pBlockHead, const void* pBlockData/*, wxDC* pDC*/,QRenderable* container);
//	
//	void DrawHail           (gFloat r, gFloat az/*, wxDC* pDC*/,QRenderable* container);
//	void DrawHail_Big       (gFloat r, gFloat az/*, wxDC* pDC*/,QRenderable* container);
//	void DrawStrongHail     (gFloat r, gFloat az/*, wxDC* pDC*/,QRenderable* container);
//	void DrawStrongHail_Big (gFloat r, gFloat az/*, wxDC* pDC*/,QRenderable* container);
//
//    void DrawTriangle(/*wxPoint*/QVector3D pt_center, int size, /*wxColor*/QVector4D BrushColor/*, wxDC* pDC*/,QRenderable* container);
//	//wxPoint TransARtoXy(gFloat fA,gFloat fR);
//    QVector3D TransARtoXy(gFloat fA,gFloat fR);
//};
using namespace goto_gis;

class CProductStructDraw_HI : public RadarStructProductionDraw
{
public:
	CProductStructDraw_HI();
	virtual ~CProductStructDraw_HI();
public:
   STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//判断产品绘图数量，共几张图

	virtual bool DumpAll(/*wxBitmap* lpBitmap,*/Layer* layer,CProductRead* lpProduct, 
		/*CProductDrawParam* lpParam,*/P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer = 0);
	void CreatAttributeList(Layer* layer,CProductRead * pProductData,QString fileName);
	void GetStationNameAndPName(QString &fileName,QString &stationName,QString &productName);
	QStringList FileNameAnalyes( QString fName );
	QImage DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize);

	//////////////////////////////////////////////////////////////////////////////////////////
public:
	osg::Vec4 m_ColorHail;             //可能发生冰雹 的颜色
	osg::Vec4 m_ColorHail_Big;        //发生冰雹的可能性很大
	osg::Vec4 m_ColorStrongHail;      //可能发生强冰雹
	osg::Vec4 m_ColorStrongHail_Big;  //发生强冰雹的可能性很大
	//wxColor m_ColorHail;             //可能发生冰雹 的颜色
	//wxColor m_ColorHail_Big;        //发生冰雹的可能性很大
	//wxColor m_ColorStrongHail;      //可能发生强冰雹
	//wxColor m_ColorStrongHail_Big;  //发生强冰雹的可能性很大

	//CProductDrawParam* m_pParam;

private:

	gFloat m_POH_MinThreshold;		//冰雹最小显示阈值 （10%—100%）
	gFloat m_POH_Threshold;			//冰雹填充阈值 （10%—100%）
	gFloat m_POSH_MinThreshold;		//强冰雹最小显示阈值 （10%—100%）
	gFloat m_POSH_Threshold;	    //强冰雹填充阈值 （10%—100%）

	double m_size1;  //冰雹尺寸
	double m_size2;	
	double m_size3;
	double m_size4;

	int m_ID;

	RadProjTransform *proj;
public:
	//	void SetParam(CProductDrawParam* lpParam);
	//	CProductDrawParam* GetParam();

	void DrawHI(ProductBase* pProduct , Layer* layer);
	void DrawLayer(const void* pBlockHead, const void* pBlockData/*, wxDC* pDC*/,Layer* lay);

	void DrawHail           (gFloat r, gFloat az/*, wxDC* pDC*/,Layer* layer);
	void DrawHail_Big       (gFloat r, gFloat az/*, wxDC* pDC*/,Layer* layer);
	void DrawStrongHail     (gFloat r, gFloat az/*, wxDC* pDC*/,Layer* layer);
	void DrawStrongHail_Big (gFloat r, gFloat az/*, wxDC* pDC*/,Layer* layer);

	void DrawTriangle(/*wxPoint*/osg::Vec3 pt_center, double size, /*wxColor*/osg::Vec4 BrushColor/*, wxDC* pDC*/,Layer* layer);
	//wxPoint TransARtoXy(gFloat fA,gFloat fR);
	osg::Vec3 TransARtoXy(gFloat fA,gFloat fR);
};
//#endif

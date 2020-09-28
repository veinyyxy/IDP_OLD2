#pragma once

#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"


#define PI 3.1415927
using namespace goto_gis;

class CProductStructDraw_SCT :  public RadarStructProductionDraw
{
public:
	CProductStructDraw_SCT();
	virtual ~CProductStructDraw_SCT();
	gFloat m_fR_km;//图像覆盖半径，千米
	gFloat m_fKmPerPixel;//每图像点千米数,gatelength.
	gFloat m_nSizeOfStormTarg;
	gFloat m_fLenghUnit;//长度单位,此数值单位为千米，即0.01km-->0.01,1km-->1.

public:
	int		m_nSizeOfStormTargHistory;//风暴过去位置图标的大小，半径
	int		m_nSizeOfStormTargCurrent;//风暴当前位置图标的大小，半径
	int		m_nSizeOfStormTargForeCast;//风暴预报位置图标的大小，半径
	//int		m_nSizeOfStormTargTrackLine;//风暴踪轨迹线图标的大小,线宽
	osg::Vec4 m_nColorHistory;//风暴点过去位置颜色
	osg::Vec4 m_nColorCurrent;//风暴点当前位置颜色
	osg::Vec4 m_nColorForeCast;//风暴点预报位置颜色
	osg::Vec4 m_nColorTrackLine;//风暴跟踪轨迹线颜色

public:
	//void SetParam(CProductDrawParam* lpParam);
	//CProductDrawParam* GetParam();
	//int GetCountOfLayers(ProductBase* pProduct);
	int GetCountOfStormCells(ProductBase* pProduct ,int nRequestLayer = -1);
public:
	void DrawLayer(const void* pBlockHead,const void* pBlockData,/*gInt16 nScale,*/int nCurLayer,int nCountOfLayer ,Layer* layer);
	void DrawSCT(ProductBase* pProduct_278_SCT ,Layer* layer);
	void Draw(ProductBase* pProduct ,Layer* layer);
private:
	/*CProductDrawParam* m_pParam;*/
	bool VerifyPosExist(const void* pBlockHead,const void* pBlockData,gFloat fA,gFloat fR);

	void DrawLayer(const void* pBlockHead,const void* pBlockData,int nCurLayer,int nCountOfLayer ,Layer* layer,bool bFlagCheck = true);
	void DrawHisPoint(gFloat fA,gFloat fR,Layer* layer);
	void DrawCurPoint(gFloat fA,gFloat fR,Layer* layer);
	void DrawCastPoint(gFloat fA,gFloat fR,Layer* layer);
	void DrawLine(gFloat fA1,gFloat fR1,gFloat fA2,gFloat fR2,Layer* layer);
	osg::Vec2 TransARtoXy(gFloat fA,gFloat fR);//度，km
	void CaculateLayout(gFloat fRMax);//km
public:

	virtual STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	//virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//判断产品绘图数量，共几张图
	virtual bool DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//画产品图片，指定某张图片
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
};

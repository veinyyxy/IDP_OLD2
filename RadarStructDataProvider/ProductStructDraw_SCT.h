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
	gFloat m_fR_km;//ͼ�񸲸ǰ뾶��ǧ��
	gFloat m_fKmPerPixel;//ÿͼ���ǧ����,gatelength.
	gFloat m_nSizeOfStormTarg;
	gFloat m_fLenghUnit;//���ȵ�λ,����ֵ��λΪǧ�ף���0.01km-->0.01,1km-->1.

public:
	int		m_nSizeOfStormTargHistory;//�籩��ȥλ��ͼ��Ĵ�С���뾶
	int		m_nSizeOfStormTargCurrent;//�籩��ǰλ��ͼ��Ĵ�С���뾶
	int		m_nSizeOfStormTargForeCast;//�籩Ԥ��λ��ͼ��Ĵ�С���뾶
	//int		m_nSizeOfStormTargTrackLine;//�籩�ٹ켣��ͼ��Ĵ�С,�߿�
	osg::Vec4 m_nColorHistory;//�籩���ȥλ����ɫ
	osg::Vec4 m_nColorCurrent;//�籩�㵱ǰλ����ɫ
	osg::Vec4 m_nColorForeCast;//�籩��Ԥ��λ����ɫ
	osg::Vec4 m_nColorTrackLine;//�籩���ٹ켣����ɫ

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
	osg::Vec2 TransARtoXy(gFloat fA,gFloat fR);//�ȣ�km
	void CaculateLayout(gFloat fRMax);//km
public:

	virtual STRUCT_DATA_TYPE GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	//virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//�жϲ�Ʒ��ͼ������������ͼ
	virtual bool DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
};

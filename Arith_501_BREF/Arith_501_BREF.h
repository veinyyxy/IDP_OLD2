//////////////////////////////////////////////////////////////////////////
// Arith_501_BREF.h
// created by ChenXiang 20080424
// 501号产品,组网算法,BREF
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_501_BREF_H
#define ARITH_501_BREF_H

#include "IArith.h"
#include "GXList.h"
#include "Product_501_BREF.h"

typedef struct 
{
	gUint8  n;							//层数标记 0=不计算，1=取下层，2=上下层插值， 3=取上层

	gUint8  be;							//下层PPI序号///就是仰角号啦
	gInt16   bR; 						//斜距(前库位置)///下层仰角///库数
	gUint8  bh1; 						//下层斜距前库插值系数，放大100倍
	gUint8  bh2;						//下层斜距后库插值系数，放大100倍
	gUint8  bf; 						//下垂直系数，放大100倍
	
	gUint8  te; 						//上层PPI序号///就是仰角号啦
	gInt16   tR;						//斜距(前库位置)///上层仰角///库数
	gUint8  th1;						//上层斜距前库插值系数，放大100倍
	gUint8  th2;						//上层斜距后库插值系数，放大100倍
	gUint8  tf; 						//上垂直系数 放大100倍

	gInt16  mR;							//目标点离中心的距离库数(斜的)
} BREFCAPSTRUCT;

class DLLEXP_GSYS Arith_501_BREF: public IArith
{
private:
	gInt32	m_StartBin;					//CAPPI的起始距离库
	gInt32	m_EndBin;					//CAPPI的结束距离库
	gInt16	m_SiteNum;					//雷达站点数量
	bool	m_Valid;					//判断执行是否成功
	BREFCAPSTRUCT *m_pCappiCoefficient;	//结构体数组
	gFloat	m_Escale;					//角度转弧度系数
	gUint16 *m_pMultifWeight;			//权重大数组    层
	gInt32	*m_pMultiRW;				//反射率乘权重 大数组 层
	gInt16	***m_pRadarDataR;			//储存反射率数据径向指针的二位数组
	gInt16	**m_el_tilts;				//各站仰角值二维数组

	gInt32	*m_SiteLatNBorderline;		//站点北边界数组
	gInt32	*m_SiteLatSBorderline;		//站点南边界数组
	gInt32	*m_SiteLonWBorderline;		//站点西边界数组
	gInt32	*m_SiteLonEBorderline;		//站点东边界数组
	gInt32	*m_pSiteLon;				//站点经度数组指针
	gInt32	*m_pSiteLat;				//站点纬度数组指针
	gInt16	*m_pSiteHigh;				//站点高度数组指针
	gUint8	*m_pPPI_L;					//各站的仰角数
	gInt16	*m_pCap_nrays;				//各站的方位角数
	gInt32	(*m_pCap_gates)[20];		//各站的库数
	gInt32	*m_pGateLen;				//各站的库长
	gInt32	**m_ppLonLatData;			//存储站点极坐标转经纬度数据指针
	gInt32  m_ResLonLat;				//经纬网格分辨率

	gInt32	m_StartLon;					//开始经度 西//0.00001度 (单层用)
	gInt32	m_EndLon;					//结束经度 东//0.00001度 (单层用)
	gInt32	m_StartLat;					//开始纬度 北//0.00001度 (单层用)
	gInt32	m_EndLat;					//结束纬度 南//0.00001度 (单层用)
	gInt32 m_M;							//数组行数 纵向长度 对应y 各层都一样
	gInt32 m_N;							//数组列数 横向长度 对应x 各层都一样
	bool    *m_ErrorData;				//无效数据数组
	gInt16 m_ScaleOfBaseData;			//基数据缩放倍数
	gInt16 m_ScaleOfMultiData;			//产品数据缩放倍数

	GXList< GenerationData *> * m_pGenerateProductList;
	ReadParam * m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//用来输出数据的列表
	Product_501_BREF* m_pOut;

public:
	Arith_501_BREF(void);
	~Arith_501_BREF(void);

	SYSMETHOD(Initialize)();				//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);
	SYSMETHOD(OutPutData)(void * pValue);	//设置输出结果数据到pValue
	SYSMETHOD(Execute)();					//执行算法
	SYSMETHOD(UnInitialize)();				//反初始化

protected:	
	GHRESULT SingleProduce(	gInt16 CAPPI_High,gInt16 *PolCappiLayerR,gUint8	*PolCappiLayerWeight,gInt16 pos);//单站处理存极坐标
	GHRESULT LayerMultiProduce(gInt16 CAPPI_High,gInt16 Layer);				 //层组网处理 存经纬网格 文件和权重数组并最终到文件

	gInt32 Max(gInt32* p,gInt16 Num);		//取整型数组最大数函数
	gInt32 Min(gInt32* p,gInt16 Num);		//取整型数组最小数函数
	gInt16 Min(gInt16* p,gInt16 Num);		//取16位整型数组最小数函数
	GHRESULT CalculateCrossRange(gInt32 SiteCompCappi_H,gInt16 *el_tilts,gInt16 pos,gInt32* EI_Distance);
	GHRESULT CreateCappiTab(gInt32 SiteCompCappi_H,gInt16 *el_tilts,gInt16 pos,gInt32* EI_Distance);

};

#endif //ARITH_501_BREF_H

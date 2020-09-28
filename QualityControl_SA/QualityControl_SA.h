/////////////////////////////////////////
//QualityControls.h 
//modifyed by zsc 
//基本反射率的第一层孤立点回波去除
////////////////////////////////////////

#pragma once
#ifndef QualityControl_SA_H
#define QualityControl_SA_H
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "../GDataTypes/IArith.h"
#include "../GDataTypes/GDef.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/StandardRadarDataX.h"
//#define max(a,b)(((a)>(b))?(a):(b))
//#define min(a,b)(((a)<(b))?(b):(a))
#define NAZIM 360
#define NGATE 1000
//#define VALUE_INVALID -9990  //实际值中的特殊标记,表示无有效观测数
//#define VALUE_RANFOLD -9991  //实际值中的特殊标记,表示有距离模糊

//#define RADIAN 3.14159/180

typedef struct mem_para 
{
	gInt32 mdz[3];
	gInt32 mdve[3];
	gInt32 mdsw[3];
	gInt32 tve[3];
	gInt32 sdve[3];
	gInt32 tdbz[3];
	gInt32 spin[3];
	gInt32 sign[3];
	gInt32 gdz[3];
}mem_para;


class QualityControl_SA : public IArith
{
public:
	QualityControl_SA();
	~QualityControl_SA();

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter) ;     //读入数据
	SYSMETHOD(Execute)();             //执行算法
	SYSMETHOD(OutPutData)(void *) ;   //取出结果数据
	SYSMETHOD(UnInitialize)();      //反初始化
	
protected:
	void ap(StandardRadarData* pInData,gInt16 iElvIndex,gInt16 ** ref,gInt16 ** ref2,gInt16 ** vel, gInt16 ** spw);
	void median(gInt16** pData,gInt16 nr,gInt16 np,gInt16** med_fld);
	void sdt_dev_ve(gInt16** pVData,gInt16** fld_mdv,gInt16 nr,gInt16 np,gInt16 iminrg,gInt16 mgate,gInt16 mazim,gInt16 numthresh,gInt16** sdevv);
	void ttve(gInt16** pData,gFloat** tve,gInt16 nr,gInt16 np,gInt16 iminrg,gInt16 mgate,gInt16 mazim,gInt16 numthresh);
	void dbzpara(gInt16** pData,gInt16 nr,gInt16 np,gInt16 iminrg,gInt32 mgate,gInt32 mazim,gInt32 dbzthresh,gInt32 numthresh,gFloat** tdbz,gInt16** sign,gInt16** spin);
	gInt32 mfun(gInt32 para[],gInt16 x);
	gInt32 mfun_r(gInt32 para[],gFloat x);
	//void noise_filter(StandardRadarData* pData);
	GHRESULT writedata(StandardRadarData* pInData,StandardRadarData* pOutData,gInt16 ** ref[],gInt16 ** vel[], gInt16 ** spw[]);
	GHRESULT writedata(StandardRadarData* pInData,StandardRadarData* pOutData,gInt16 layerIndex,gInt16 ** ref,gInt16 ** vel, gInt16 ** spw);
	GHRESULT writeHead(StandardRadarData* pInData,StandardRadarData* pOutData);
	void sun_beam_remove(short **bref,short **v,short **w,	int nrays,	int ngates,int nVgates);
	void noise_remove(short **bref,short **v,short **w, int nrays, int ngates, int start, int end,  float threshold ,int gatelen,int REF_SCALE);
private:
	GXList< GenerationData > * m_pGenerateProductList;
	GXList< GenerationData > * m_pOutputProductList;
	ReadParam * m_pParameter;
	mem_para mem_para_ap,mem_para_cl,mem_para_sea;
	float tempscale ;
	char RadarType[4] ;
	ofstream logfile;
};
//GHRESULT DLLEXP_GSYS ExecuteArithQuality_SA( GXList< GenerationData>  *Datas, GXList< GenerationData > * pOutputList, ReadParam* pParam);  
GHRESULT DLLEXP_GSYS ExecuteArithQuality_SA( GXList< GenerationData>  *Datas, GXList< GenerationData > * pOutputList, ReadParam* pParam);  

#endif

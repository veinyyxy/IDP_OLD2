#pragma once
#ifndef QualityControl_CC_H
#define QualityControl_CC_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include "../GDataTypes/IArith.h"
#include "../GDataTypes/GDef.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/StandardRadarDataX.h"

#define NAZIM 512
#define NGATE 500

#define RADIAN 3.14159/180
enum DatType
{
	R,
	V,
	W,
};
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

class QualityControl_CC : public IArith
{
public:
	QualityControl_CC();
	~QualityControl_CC();  

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;     //读入数据
	SYSMETHOD(Execute) ();             //执行算法
	SYSMETHOD(OutPutData)(void *) ;   //取出结果数据
	SYSMETHOD(UnInitialize)	() ;       //反初始化

private:
	GHRESULT TurnTO360(gInt16 **pValue,StandardRadarData *pGData,gInt16 laynum, DatType datType);
	//GHRESULT median(gInt16 ** Data,gInt16 ** Data2,gInt16 gates,gInt16 binnum);
	GHRESULT median(gInt16** pData,gInt16 nr,gInt16 np,gInt16** med_fld);
	GHRESULT sdt_dev_ve(gInt16** pVData,gInt16** fld_mdv,gInt16 np,gInt16 nrang,gInt16 mgate,gInt16 mazim,gInt16 numthresh,gInt16** sdevv);
	GHRESULT dbzpara(gInt16** pData,gInt16 np,gInt16 nrang,gInt32 mgate,gInt32 mazim,gInt32 dbzthresh,gInt32 numthresh,gFloat** tdbz,gInt16** sign,gInt16** spin);
	
	GHRESULT ttve(gInt16** pData,gFloat** tve,gInt16 np,gInt16 iminrg,gInt16 nrang,gInt16 mgate,gInt16 mazim,gInt16 numthresh);
	gInt32 mfun(gInt32 para[],gInt16 x);
	gInt32 mfun_r(gInt32 para[],gFloat x);
	GHRESULT ap_CC(StandardRadarData* pInData,gInt16 iElvIndex,gInt16 ** ref,gInt16 ** ref2,gInt16 ** vel, gInt16 ** spw);
	GHRESULT noise_filter(StandardRadarData* pData);
	GHRESULT writedata(StandardRadarData* pInData,StandardRadarData* pOutData,gInt16 ** ref[],gInt16 ** vel[], gInt16 ** spw[]);
	void sun_beam_remove(short **bref,	short ** v, short** w,int nrays,	int ngates );
	void noise_remove(short **bref, short ** v, short** w,int nrays,   int ngates, int start, int end,  short threshold ,int gatelen);

private:
	GXList< GenerationData > * m_pGenerateProductList;
	GXList< GenerationData > * m_pOutputProductList;
	ReadParam * m_pParameter;
};
#endif
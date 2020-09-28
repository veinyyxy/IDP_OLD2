/////////////////////////////////////////
//QualityControls.h
//modifyed by zsc
//基本反射率的第一层孤立点回波去除
////////////////////////////////////////

#pragma once
#ifndef QualityControl_FirstOfRef_H
#define QualityControl_FirstOfRef_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "../GDataTypes/IArith.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDef.h"
#include "../GDataTypes/StandardRadarDataX.h"
//#define max(a,b)(((a)>(b))?(a):(b))
//#define min(a,b)(((a)<(b))?(b):(a))

class DLLEXP_GSYS QualityControl_FirstOfRef : public IArith
{
public:
	QualityControl_FirstOfRef();
	~QualityControl_FirstOfRef();

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;     //读入数据
	SYSMETHOD(Execute) ();             //执行算法
	SYSMETHOD(OutPutData)(void *pValue) ;   //取出结果数据
	SYSMETHOD(UnInitialize)	() ;       //反初始化

private:
	bool CalcFeature(int a,int p,float * AFeature,int MaxAzimuth,gInt16 ***RData);
	void Finalproc(StandardRadarData *pGData,StandardRadarData* pOutData, gInt16 **mask,int EchoPointRateThreshold, gInt16 MaxAzimuth, gInt16 MaxGates);

	//static const int  MaxAzimuth=360;
	//static const int  MaxGates=460;
	//static const int  EchoPointRateThreshold=35;
private:
	GXList< GenerationData > * m_pGenerateProductList;;
	GXList< GenerationData > * m_pOutputProductList;
	ReadParam * m_pParameter;
};
#endif

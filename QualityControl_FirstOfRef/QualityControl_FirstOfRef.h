/////////////////////////////////////////
//QualityControls.h
//modifyed by zsc
//���������ʵĵ�һ�������ز�ȥ��
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

	SYSMETHOD(Initialize)() ;			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;     //��������
	SYSMETHOD(Execute) ();             //ִ���㷨
	SYSMETHOD(OutPutData)(void *pValue) ;   //ȡ���������
	SYSMETHOD(UnInitialize)	() ;       //����ʼ��

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

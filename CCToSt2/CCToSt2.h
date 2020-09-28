#pragma once
#ifndef CCTOST2_H
#define CCTOST2_H

#include "CCProductPretreatment2.h"
#include "IArith.h"
#include "GXList.h"
#include "GData.h"
#include "GDef.h"
#include <math.h>
#include <string.h>
class DLLEXP_GSYS CCToSt2: public IArith
{
public:
	CCToSt2(){};
	~CCToSt2(){};
	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;

	
private:
int DateToJulian(int year, int mon, int day);
	
int TimeToSeconds(short hour, short minute, short second);
};

GHRESULT DLLEXP_GSYS ExecuteArithCCToSt2( GXList< GenerationData>  *Datas,  ReadParam* pParam,  GXList< GenerationData > * pOutputList);

#endif


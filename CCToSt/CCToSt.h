#pragma once
#ifndef CCTOST_H
#define CCTOST_H

#include "CCProductPretreatment.h"
#include "../GDataTypes/IArith.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDef.h"
#include <math.h>
#include <string.h>
class CCToSt: public IArith
{
public:
	CCToSt(){};
	~CCToSt(){};
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

#endif

